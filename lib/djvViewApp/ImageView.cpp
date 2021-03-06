// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvViewApp/ImageView.h>

#include <djvViewApp/Annotate.h>
#include <djvViewApp/ImageSettings.h>
#include <djvViewApp/ImageViewPrivate.h>
#include <djvViewApp/ViewSettings.h>

#include <djvUI/Action.h>
#include <djvUI/ImageWidget.h>
#include <djvUI/SettingsSystem.h>
#include <djvUI/Style.h>

#include <djvAV/AVSystem.h>
#include <djvAV/Image.h>
#include <djvAV/OCIOSystem.h>
#include <djvAV/Render2D.h>

#include <djvCore/Animation.h>
#include <djvCore/Context.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace djv::Core;

namespace djv
{
    namespace ViewApp
    {
        namespace
        {
            //! \todo Should this be configurable?
            const size_t zoomAnimation = 200;
            
        } // namespace
        
        GridOptions::GridOptions()
        {}

        bool GridOptions::operator == (const GridOptions& other) const
        {
            return enabled == other.enabled &&
                size == other.size &&
                color == other.color &&
                labels == other.labels &&
                labelsColor == other.labelsColor;
        }

        struct ImageView::Private
        {
            std::shared_ptr<ValueSubject<std::shared_ptr<AV::Image::Image> > > image;
            std::shared_ptr<ValueSubject<AV::Render2D::ImageOptions> > imageOptions;
            AV::OCIO::Config ocioConfig;
            std::string outputColorSpace;
            std::shared_ptr<ValueSubject<glm::vec2> > imagePos;
            std::shared_ptr<ValueSubject<float> > imageZoom;
            std::shared_ptr<ValueSubject<UI::ImageRotate> > imageRotate;
            std::shared_ptr<ValueSubject<UI::ImageAspectRatio> > imageAspectRatio;
            ImageViewLock lock = ImageViewLock::None;
            BBox2f lockFrame = BBox2f(0.F, 0.F, 0.F, 0.F);
            std::shared_ptr<ValueSubject<GridOptions> > gridOptions;
            std::shared_ptr<ValueSubject<AV::Image::Color> > backgroundColor;
            std::vector<std::shared_ptr<AnnotatePrimitive> > annotations;
            glm::vec2 pressedImagePos = glm::vec2(0.F, 0.F);
            bool viewInit = true;
            std::shared_ptr<ImageViewGridOverlay> gridOverlay;
            std::shared_ptr<ValueObserver<ImageViewLock> > lockObserver;
            std::shared_ptr<ValueObserver<AV::OCIO::Config> > ocioConfigObserver;
            std::shared_ptr<Animation::Animation> zoomAnimation;
        };

        void ImageView::_init(const std::shared_ptr<Context>& context)
        {
            Widget::_init(context);
            DJV_PRIVATE_PTR();

            setClassName("djv::ViewApp::ImageView");

            auto avSystem = context->getSystemT<AV::AVSystem>();
            auto settingsSystem = context->getSystemT<UI::Settings::System>();
            auto imageSettings = settingsSystem->getSettingsT<ImageSettings>();
            auto viewSettings = settingsSystem->getSettingsT<ViewSettings>();
            p.image = ValueSubject<std::shared_ptr<AV::Image::Image> >::create();
            AV::Render2D::ImageOptions imageOptions;
            imageOptions.alphaBlend = avSystem->observeAlphaBlend()->get();
            imageOptions.colorEnabled = true;
            imageOptions.levelsEnabled = true;
            imageOptions.softClipEnabled = true;
            p.imageOptions = ValueSubject<AV::Render2D::ImageOptions>::create(imageOptions);
            p.imagePos = ValueSubject<glm::vec2>::create();
            p.imageZoom = ValueSubject<float>::create(1.F);
            p.imageRotate = ValueSubject<UI::ImageRotate>::create(imageSettings->observeRotate()->get());
            p.imageAspectRatio = ValueSubject<UI::ImageAspectRatio>::create(imageSettings->observeAspectRatio()->get());
            p.gridOptions = ValueSubject<GridOptions>::create(viewSettings->observeGridOptions()->get());
            p.backgroundColor = ValueSubject<AV::Image::Color>::create(viewSettings->observeBackgroundColor()->get());

            p.gridOverlay = ImageViewGridOverlay::create(context);
            p.gridOverlay->setOptions(p.gridOptions->get());
            p.gridOverlay->setImagePosAndZoom(p.imagePos->get(), p.imageZoom->get());
            p.gridOverlay->setImageRotate(p.imageRotate->get());
            p.gridOverlay->setImageAspectRatio(p.imageAspectRatio->get(), 1.F, 1.F);
            p.gridOverlay->setImageBBox(getImageBBox());
            p.gridOverlay->setImageFrame(p.lockFrame);
            addChild(p.gridOverlay);

            auto weak = std::weak_ptr<ImageView>(std::dynamic_pointer_cast<ImageView>(shared_from_this()));
            p.lockObserver = ValueObserver<ImageViewLock>::create(
                viewSettings->observeLock(),
                [weak](ImageViewLock value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->lock = value;
                        if (widget->isVisible() && !widget->isClipped())
                        {
                            widget->_resize();
                        }
                    }
                });

            auto ocioSystem = context->getSystemT<AV::OCIO::System>();
            auto contextWeak = std::weak_ptr<Context>(context);
            p.ocioConfigObserver = ValueObserver<AV::OCIO::Config>::create(
                ocioSystem->observeCurrentConfig(),
                [weak, contextWeak](const AV::OCIO::Config& value)
                {
                    if (auto context = contextWeak.lock())
                    {
                        if (auto widget = weak.lock())
                        {
                            auto ocioSystem = context->getSystemT<AV::OCIO::System>();
                            widget->_p->ocioConfig = value;
                            widget->_p->outputColorSpace = ocioSystem->getColorSpace(value.display, value.view);
                            widget->_redraw();
                        }
                    }
                });
                
            p.zoomAnimation = Animation::Animation::create(context);
            p.zoomAnimation->setType(Animation::Type::SmoothStep);
        }

        ImageView::ImageView() :
            _p(new Private)
        {}

        ImageView::~ImageView()
        {}

        std::shared_ptr<ImageView> ImageView::create(const std::shared_ptr<Context>& context)
        {
            auto out = std::shared_ptr<ImageView>(new ImageView);
            out->_init(context);
            return out;
        }

        std::shared_ptr<IValueSubject<std::shared_ptr<AV::Image::Image> > > ImageView::observeImage() const
        {
            return _p->image;
        }

        void ImageView::setImage(const std::shared_ptr<AV::Image::Image>& value)
        {
            DJV_PRIVATE_PTR();
            if (p.image->setIfChanged(value))
            {
                auto image = p.image->get();
                p.gridOverlay->setImageAspectRatio(
                    p.imageAspectRatio->get(),
                    image ? image->getAspectRatio() : 1.F,
                    image ? image->getInfo().pixelAspectRatio : 1.F);
                p.gridOverlay->setImageBBox(getImageBBox());
                p.gridOverlay->setImageFrame(p.lockFrame);
                _resize();
            }
        }

        std::shared_ptr<IValueSubject<AV::Render2D::ImageOptions> > ImageView::observeImageOptions() const
        {
            return _p->imageOptions;
        }

        void ImageView::setImageOptions(const AV::Render2D::ImageOptions& value)
        {
            DJV_PRIVATE_PTR();
            if (p.imageOptions->setIfChanged(value))
            {
                if (isVisible() && !isClipped())
                {
                    _resize();
                }
            }
        }

        std::shared_ptr<IValueSubject<glm::vec2> > ImageView::observeImagePos() const
        {
            return _p->imagePos;
        }

        std::shared_ptr<IValueSubject<float> > ImageView::observeImageZoom() const
        {
            return _p->imageZoom;
        }

        std::shared_ptr<IValueSubject<UI::ImageRotate> > ImageView::observeImageRotate() const
        {
            return _p->imageRotate;
        }

        std::shared_ptr<IValueSubject<UI::ImageAspectRatio> > ImageView::observeImageAspectRatio() const
        {
            return _p->imageAspectRatio;
        }

        BBox2f ImageView::getImageBBox() const
        {
            return _getBBox(_getImagePoints());
        }

        void ImageView::setImagePos(const glm::vec2& value, bool animate)
        {
            DJV_PRIVATE_PTR();
            setImagePosAndZoom(value, p.imageZoom->get(), animate);
        }

        void ImageView::setImageZoom(float value, bool animate)
        {
            DJV_PRIVATE_PTR();
            setImagePosAndZoom(p.imagePos->get(), value, animate);
        }

        void ImageView::setImageZoomFocus(float value, const glm::vec2& mouse, bool animate)
        {
            DJV_PRIVATE_PTR();
            setImagePosAndZoom(
                mouse + (p.imagePos->get() - mouse) * (value / p.imageZoom->get()),
                value,
                animate);
        }

        void ImageView::setImageZoomFocus(float value, bool animate)
        {
            const BBox2f& g = getGeometry();
            const glm::vec2& c = g.getCenter();
            setImageZoomFocus(value, c, animate);
        }

        void ImageView::setImagePosAndZoom(const glm::vec2& pos, float zoom, bool animate)
        {
            if (animate)
            {
                _animatePosAndZoom(pos, zoom);
            }
            else
            {
                _setPosAndZoom(pos, zoom);
            }
        }

        void ImageView::setImageRotate(UI::ImageRotate value)
        {
            DJV_PRIVATE_PTR();
            if (p.imageRotate->setIfChanged(value))
            {
                p.gridOverlay->setImageRotate(value);
                p.gridOverlay->setImageBBox(getImageBBox());
                p.gridOverlay->setImageFrame(p.lockFrame);
                _resize();
            }
        }

        void ImageView::setImageAspectRatio(UI::ImageAspectRatio value)
        {
            DJV_PRIVATE_PTR();
            if (p.imageAspectRatio->setIfChanged(value))
            {
                p.gridOverlay->setImageAspectRatio(
                    p.imageAspectRatio->get(),
                    p.image ? p.image->get()->getAspectRatio() : 1.F,
                    p.image ? p.image->get()->getInfo().pixelAspectRatio : 1.F);
                p.gridOverlay->setImageBBox(getImageBBox());
                p.gridOverlay->setImageFrame(p.lockFrame);
                _resize();
            }
        }

        void ImageView::imageFill(bool animate)
        {
            DJV_PRIVATE_PTR();
            if (p.image->get())
            {
                const BBox2f& g = getGeometry();
                const auto pts = _getImagePoints();
                const glm::vec2 c = _getCenter(pts);
                const BBox2f bbox = _getBBox(pts);
                float zoom = g.w() / static_cast<float>(bbox.w());
                if (zoom * bbox.h() > g.h())
                {
                    zoom = g.h() / static_cast<float>(bbox.h());
                }
                setImagePosAndZoom(
                    glm::vec2(
                        g.w() / 2.F - c.x * zoom,
                        g.h() / 2.F - c.y * zoom),
                    zoom,
                    animate);
            }
        }

        void ImageView::setImageFrame(const BBox2f& value)
        {
            _p->lockFrame = value;
        }

        void ImageView::imageFrame(bool animate)
        {
            DJV_PRIVATE_PTR();
            if (p.image->get())
            {
                const BBox2f& g = getGeometry();
                const auto pts = _getImagePoints();
                const glm::vec2 c = _getCenter(pts);
                const BBox2f bbox = _getBBox(pts);
                float zoom = p.lockFrame.w() / static_cast<float>(bbox.w());
                if (zoom * bbox.h() > p.lockFrame.h())
                {
                    zoom = p.lockFrame.h() / static_cast<float>(bbox.h());
                }
                setImagePosAndZoom(
                    glm::vec2(
                        (p.lockFrame.min.x - g.min.x) + p.lockFrame.w() / 2.F - c.x * zoom,
                        (p.lockFrame.min.y - g.min.y) + p.lockFrame.h() / 2.F - c.y * zoom),
                    zoom,
                    animate);
            }
        }

        void ImageView::imageCenter(bool animate)
        {
            DJV_PRIVATE_PTR();
            if (p.image->get())
            {
                const BBox2f& g = getGeometry();
                const glm::vec2 c = _getCenter(_getImagePoints());
                setImagePosAndZoom(
                    glm::vec2(
                        g.w() / 2.F - c.x,
                        g.h() / 2.F - c.y),
                    1.F,
                    animate);
            }
        }

        std::shared_ptr<IValueSubject<GridOptions> > ImageView::observeGridOptions() const
        {
            return _p->gridOptions;
        }

        void ImageView::setGridOptions(const GridOptions& value)
        {
            DJV_PRIVATE_PTR();
            if (p.gridOptions->setIfChanged(value))
            {
                p.gridOverlay->setOptions(value);
            }
        }

        std::shared_ptr<IValueSubject<AV::Image::Color> > ImageView::observeBackgroundColor() const
        {
            return _p->backgroundColor;
        }

        void ImageView::setBackgroundColor(const AV::Image::Color& value)
        {
            DJV_PRIVATE_PTR();
            if (p.backgroundColor->setIfChanged(value))
            {
                _redraw();
            }
        }
        
        void ImageView::setAnnotations(const std::vector<std::shared_ptr<AnnotatePrimitive> >& value)
        {
            _p->annotations = value;
            _redraw();
        }

        void ImageView::_preLayoutEvent(Event::PreLayout & event)
        {
            const auto& style = _getStyle();
            const float sa = style->getMetric(UI::MetricsRole::ScrollArea);
            _setMinimumSize(glm::vec2(sa, sa));
        }

        void ImageView::_layoutEvent(Event::Layout &)
        {
            DJV_PRIVATE_PTR();
            switch (p.lock)
            {
            case ImageViewLock::Fill:   imageFill();   break;
            case ImageViewLock::Frame:  imageFrame();  break;
            case ImageViewLock::Center: imageCenter(); break;
            default:
                if (p.image && p.viewInit)
                {
                    imageFill();
                }
                break;
            }
            if (p.image && p.viewInit)
            {
                p.viewInit = false;
            }
            p.gridOverlay->setGeometry(getGeometry());
            p.gridOverlay->setImageBBox(getImageBBox());
            p.gridOverlay->setImageFrame(p.lockFrame);
        }

        void ImageView::_paintEvent(Event::Paint &)
        {
            DJV_PRIVATE_PTR();

            const auto& style = _getStyle();
            const BBox2f & g = getMargin().bbox(getGeometry(), style);
            auto render = _getRender();
            render->setFillColor(p.backgroundColor->get());
            render->drawRect(g);

            const float zoom = p.imageZoom->get();
            if (auto image = p.image->get())
            {
                glm::mat3x3 m(1.F);
                m = glm::translate(m, g.min + p.imagePos->get());
                m *= UI::ImageWidget::getXForm(image, p.imageRotate->get(), glm::vec2(zoom, zoom), p.imageAspectRatio->get());
                render->pushTransform(m);
                render->setFillColor(AV::Image::Color(1.F, 1.F, 1.F));
                AV::Render2D::ImageOptions options(p.imageOptions->get());
                auto i = p.ocioConfig.fileColorSpaces.find(image->getPluginName());
                if (i != p.ocioConfig.fileColorSpaces.end())
                {
                    options.colorSpace.input = i->second;
                }
                else
                {
                    i = p.ocioConfig.fileColorSpaces.find(std::string());
                    if (i != p.ocioConfig.fileColorSpaces.end())
                    {
                        options.colorSpace.input = i->second;
                    }
                }
                options.colorSpace.output = p.outputColorSpace;
                options.cache = AV::Render2D::ImageCache::Dynamic;
                render->drawImage(image, glm::vec2(0.F, 0.F), options);
                render->popTransform();
            }

            if (p.annotations.size())
            {
                const glm::vec2& pos = p.imagePos->get();
                glm::mat3x3 m(1.F);
                m = glm::translate(m, g.min + pos);
                m = glm::scale(m, glm::vec2(zoom, zoom));
                render->pushTransform(m);
                for (const auto& i : p.annotations)
                {
                    i->draw(render);
                }
                render->popTransform();
            }
        }

        std::vector<glm::vec3> ImageView::_getImagePoints() const
        {
            DJV_PRIVATE_PTR();
            std::vector<glm::vec3> out;
            if (auto image = p.image->get())
            {
                const AV::Image::Size& size = image->getSize();
                out.resize(4);
                out[0].x = 0.F;
                out[0].y = 0.F;
                out[0].z = 1.F;
                out[1].x = 0.F + size.w;
                out[1].y = 0.F;
                out[1].z = 1.F;
                out[2].x = 0.F + size.w;
                out[2].y = 0.F + size.h;
                out[2].z = 1.F;
                out[3].x = 0.F;
                out[3].y = 0.F + size.h;
                out[3].z = 1.F;
                glm::mat3x3 m(1.F);
                m *= UI::ImageWidget::getXForm(image, p.imageRotate->get(), glm::vec2(1.F, 1.F), p.imageAspectRatio->get());
                for (auto& i : out)
                {
                    i = m * i;
                }
            }
            return out;
        }

        glm::vec2 ImageView::_getCenter(const std::vector<glm::vec3>& value)
        {
            glm::vec2 out(0.F, 0.F);
            if (value.size())
            {
                for (const auto& i : value)
                {
                    out.x += i.x;
                    out.y += i.y;
                }
                out.x /= static_cast<float>(value.size());
                out.y /= static_cast<float>(value.size());
            }
            return out;
        }
        
        BBox2f ImageView::_getBBox(const std::vector<glm::vec3>& value)
        {
            BBox2f out(0.F, 0.F, 0.F, 0.F);
            if (value.size())
            {
                out.min = out.max = value[0];
                for (size_t i = 1; i < 4; ++i)
                {
                    out.min.x = std::min(out.min.x, value[i].x);
                    out.max.x = std::max(out.max.x, value[i].x);
                    out.min.y = std::min(out.min.y, value[i].y);
                    out.max.y = std::max(out.max.y, value[i].y);
                }
            }
            return out;
        }

        void ImageView::_animatePosAndZoom(const glm::vec2& pos, float zoom)
        {
            DJV_PRIVATE_PTR();
            const glm::vec2 posPrev = p.imagePos->get();
            const float zoomPrev = p.imageZoom->get();
            auto weak = std::weak_ptr<ImageView>(std::dynamic_pointer_cast<ImageView>(shared_from_this()));
            p.zoomAnimation->start(
                0.F,
                1.F,
                std::chrono::milliseconds(zoomAnimation),
                [weak, pos, zoom, posPrev, zoomPrev](float value)
                {
                    if (auto widget = weak.lock())
                    {
                        const glm::vec2 posTmp(
                            Math::lerp(value, posPrev.x, pos.x),
                            Math::lerp(value, posPrev.y, pos.y));
                        const float zoomTmp = Math::lerp(value, zoomPrev, zoom);
                        widget->_setPosAndZoom(posTmp, zoomTmp);
                    }
                },
                [weak, pos, zoom](float)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_setPosAndZoom(pos, zoom);
                    }
                });
        }

        void ImageView::_setPosAndZoom(const glm::vec2& pos, float zoom)
        {
            DJV_PRIVATE_PTR();
            if (p.imagePos->setIfChanged(pos))
            {
                _resize();
            }
            if (p.imageZoom->setIfChanged(std::max(0.F, zoom)))
            {
                _resize();
            }
            p.gridOverlay->setImagePosAndZoom(p.imagePos->get(), p.imageZoom->get());
            p.gridOverlay->setImageBBox(getImageBBox());
            p.gridOverlay->setImageFrame(p.lockFrame);
        }

    } // namespace ViewApp

    picojson::value toJSON(const ViewApp::GridOptions& value)
    {
        picojson::value out(picojson::object_type, true);
        out.get<picojson::object>()["Enabled"] = toJSON(value.enabled);
        out.get<picojson::object>()["Size"] = toJSON(value.size);
        out.get<picojson::object>()["Color"] = toJSON(value.color);
        out.get<picojson::object>()["Labels"] = toJSON(value.labels);
        out.get<picojson::object>()["LabelsColor"] = toJSON(value.labelsColor);
        return out;
    }

    void fromJSON(const picojson::value& value, ViewApp::GridOptions& out)
    {
        if (value.is<picojson::object>())
        {
            for (const auto& i : value.get<picojson::object>())
            {
                if ("Enabled" == i.first)
                {
                    fromJSON(i.second, out.enabled);
                }
                else if ("Size" == i.first)
                {
                    fromJSON(i.second, out.size);
                }
                else if ("Color" == i.first)
                {
                    fromJSON(i.second, out.color);
                }
                else if ("Labels" == i.first)
                {
                    fromJSON(i.second, out.labels);
                }
                else if ("LabelsColor" == i.first)
                {
                    fromJSON(i.second, out.labelsColor);
                }
            }
        }
        else
        {
            //! \todo How can we translate this?
            throw std::invalid_argument(DJV_TEXT("error_cannot_parse_the_value"));
        }
    }

} // namespace djv

