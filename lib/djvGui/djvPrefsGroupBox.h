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

//! \file djvPrefsGroupBox.h

#ifndef DJV_PREFS_GROUP_BOX_H
#define DJV_PREFS_GROUP_BOX_H

#include <djvGuiExport.h>

#include <djvUtil.h>

#include <QWidget>

class QFormLayout;

//! \addtogroup djvGuiWidget
//@{

//------------------------------------------------------------------------------
//! \class djvPrefsGroupBox
//!
//! This class provides a group box for preferences.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT djvPrefsGroupBox : public QWidget
{
    Q_OBJECT
    
public:

    //! Constructor.

    explicit djvPrefsGroupBox(
        const QString & title,
        const QString & text   = QString(),
        QWidget *       parent = 0);

    //! Destructor.

    virtual ~djvPrefsGroupBox();

    //! Get the text.

    const QString & text() const;
    
    //! Set the text.
    
    void setText(const QString &);
    
    //! Create a new layout to add widgets to the group box.
    
    QFormLayout * createLayout();
    
private:
    
    DJV_PRIVATE_COPY(djvPrefsGroupBox);
    DJV_PRIVATE_IMPLEMENTATION();
};

//@} // djvGuiWidget

#endif // DJV_PREFS_GROUP_BOX_H

