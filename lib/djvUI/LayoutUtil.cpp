// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvUI/LayoutUtil.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        namespace Layout
        {
            BBox2f getPopupGeometry(
                Popup value,
                const glm::vec2& pos,
                const glm::vec2& minimumSize)
            {
                BBox2f out;
                switch (value)
                {
                case Popup::BelowRight:
                    out = BBox2f(
                        glm::vec2(pos.x, pos.y),
                        glm::vec2(pos.x + minimumSize.x, pos.y + minimumSize.y));
                    break;
                case Popup::BelowLeft:
                    out = BBox2f(
                        glm::vec2(pos.x - minimumSize.x, pos.y),
                        glm::vec2(pos.x, pos.y + minimumSize.y));
                    break;
                case Popup::AboveRight:
                    out = BBox2f(
                        glm::vec2(pos.x, pos.y - minimumSize.y),
                        glm::vec2(pos.x + minimumSize.x, pos.y));
                    break;
                case Popup::AboveLeft:
                    out = BBox2f(
                        glm::vec2(pos.x - minimumSize.x, pos.y - minimumSize.y),
                        glm::vec2(pos.x, pos.y));
                    break;
                default: break;
                }
                return out;
            }

            BBox2f getPopupGeometry(
                Popup value,
                const BBox2f& button,
                const glm::vec2& minimumSize)
            {
                BBox2f out;
                switch (value)
                {
                case Popup::BelowLeft:
                    out = BBox2f(
                        glm::vec2(
                            std::min(button.max.x - minimumSize.x, button.min.x),
                            button.max.y),
                        glm::vec2(
                            button.max.x,
                            button.max.y + minimumSize.y));
                    break;
                case Popup::BelowRight:
                    out = BBox2f(
                        glm::vec2(
                            button.min.x,
                            button.max.y),
                        glm::vec2(
                            std::max(button.min.x + minimumSize.x, button.max.x),
                            button.max.y + minimumSize.y));
                    break;
                case Popup::AboveRight:
                    out = BBox2f(
                        glm::vec2(
                            button.min.x,
                            button.min.y - minimumSize.y),
                        glm::vec2(
                            std::max(button.min.x + minimumSize.x, button.max.x),
                            button.min.y));
                    break;
                case Popup::AboveLeft:
                    out = BBox2f(
                        glm::vec2(
                            std::min(button.max.x - minimumSize.x, button.min.x),
                            button.min.y - minimumSize.y),
                        glm::vec2(
                            button.max.x,
                            button.min.y));
                    break;
                default: break;
                }
                return out;
            }

            Popup getPopup(
                Popup value,
                const BBox2f& area,
                const glm::vec2& pos,
                const glm::vec2& minimumSize)
            {
                Popup out = Popup::First;

                std::vector<Popup> popupList;
                popupList.push_back(value);
                auto popupEnums = getPopupEnums();
                const auto i = std::find(popupEnums.begin(), popupEnums.end(), value);
                if (i != popupEnums.end())
                {
                    popupEnums.erase(i);
                }
                for (auto j : popupEnums)
                {
                    popupList.push_back(j);
                }

                std::map<float, Popup> popupAreas;
                size_t j = 0;
                const size_t popupListSize = popupList.size();
                for (; j < popupListSize; ++j)
                {
                    Popup popup = popupList[j];
                    const BBox2f g = getPopupGeometry(popup, pos, minimumSize);
                    const BBox2f clipped = g.intersect(area);
                    if (g == clipped)
                    {
                        out = popup;
                        break;
                    }
                    popupAreas[clipped.getArea()] = popup;
                }
                if (popupListSize == j)
                {
                    out = popupAreas.rbegin()->second;
                }
                return out;
            }

            Popup getPopup(
                Popup value,
                const BBox2f& area,
                const BBox2f& button,
                const glm::vec2& minimumSize)
            {
                Popup out = Popup::First;

                std::vector<Popup> popupList;
                popupList.push_back(value);
                auto popupEnums = getPopupEnums();
                const auto i = std::find(popupEnums.begin(), popupEnums.end(), value);
                if (i != popupEnums.end())
                {
                    popupEnums.erase(i);
                }
                for (auto j : popupEnums)
                {
                    popupList.push_back(j);
                }

                std::map<float, Popup> popupAreas;
                size_t j = 0;
                const size_t popupListSize = popupList.size();
                for (; j < popupListSize; ++j)
                {
                    Popup popup = popupList[j];
                    const BBox2f g = getPopupGeometry(popup, button, minimumSize);
                    const BBox2f clipped = g.intersect(area);
                    const float area = clipped.getArea();
                    if (popupAreas.find(area) == popupAreas.end())
                    {
                        popupAreas[area] = popup;
                    }
                }
                if (popupAreas.size())
                {
                    out = popupAreas.rbegin()->second;
                }
                return out;
            }

        } // namespace Layout
    } // namespace UI
} // namespace djv

