// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvAV/DPX.h>

#include <djvCore/FileIO.h>

using namespace djv::Core;

namespace djv
{
    namespace AV
    {
        namespace IO
        {
            namespace DPX
            {
                struct Write::Private
                {
                    Options options;
                };

                Write::Write() :
                    _p(new Private)
                {}

                Write::~Write()
                {
                    _finish();
                }

                std::shared_ptr<Write> Write::create(
                    const FileSystem::FileInfo& fileInfo,
                    const Info & info,
                    const WriteOptions& writeOptions,
                    const Options& options,
                    const std::shared_ptr<TextSystem>& textSystem,
                    const std::shared_ptr<ResourceSystem>& resourceSystem,
                    const std::shared_ptr<LogSystem>& logSystem)
                {
                    auto out = std::shared_ptr<Write>(new Write);
                    out->_p->options = options;
                    out->_init(fileInfo, info, writeOptions, textSystem, resourceSystem, logSystem);
                    return out;
                }

                Image::Type Write::_getImageType(Image::Type) const
                {
                    return Image::Type::RGB_U10;
                }

                Image::Layout Write::_getImageLayout() const
                {
                    Image::Layout out;
                    out.endian = Memory::Endian::MSB;
                    out.alignment = 4;
                    return out;
                }
                
                void Write::_write(const std::string & fileName, const std::shared_ptr<Image::Image> & image)
                {
                    DJV_PRIVATE_PTR();
                    auto io = FileSystem::FileIO::create();
                    io->open(fileName, FileSystem::FileIO::Mode::Write);
                    Info info;
                    info.video.push_back(image->getInfo());
                    info.tags = image->getTags();
                    write(
                        io,
                        info,
                        p.options.version,
                        p.options.endian,
                        _options.colorSpace.empty() ? Cineon::ColorProfile::Raw : Cineon::ColorProfile::FilmPrint);
                    io->write(image->getData(), image->getDataByteCount());
                    writeFinish(io);
                }

            } // namespace DPX
        } // namespace IO
    } // namespace AV
} // namespace djv

