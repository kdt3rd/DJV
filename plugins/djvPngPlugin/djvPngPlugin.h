//------------------------------------------------------------------------------
// Copyright (c) 2004-2014 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djvPngPlugin.h

#ifndef DJV_PNG_PLUGIN_H
#define DJV_PNG_PLUGIN_H

#include <djvImageIo.h>

#include <djvStringUtil.h>

#include <png.h>

//! \addtogroup plugins
//@{

//! \defgroup djvPngPlugin djvPngPlugin
//!
//! This plugin provides support for the Portable Network Graphics (PNG) image
//! file format.
//!
//! Requires:
//!
//! - libpng - http://www.libpng.org
//!
//! File extensions: .png
//!
//! Supported features:
//!
//! - 8-bit, 16-bit, Luminance, RGB, RGBA
//! - File compression

//@} // plugins

//! \addtogroup djvPngPlugin
//@{

//------------------------------------------------------------------------------
//! \class djvPngPlugin
//!
//! This class provides a PNG plugin.
//------------------------------------------------------------------------------

class djvPngPlugin : public djvImageIo
{
public:

    //! The plugin name.
    
    static const QString staticName;
    
    virtual djvPlugin * copyPlugin() const;

    virtual QString pluginName() const;

    virtual QStringList extensions() const;

    virtual djvImageLoad * createLoad() const;
    
    virtual djvImageSave * createSave() const;
};

//------------------------------------------------------------------------------

//! This struct provides libpng error handling.

struct djvPngErrorStruct
{
    char msg[djvStringUtil::cStringLength];
};

extern "C" {

void djvPngError(png_structp, png_const_charp);
void djvPngWarning(png_structp, png_const_charp);

}

//@} // djvPngPlugin

#endif // DJV_PNG_H

