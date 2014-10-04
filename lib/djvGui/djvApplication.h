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

//! \file djvApplication.h

#ifndef DJV_APPLICATION_H
#define DJV_APPLICATION_H

#include <djvGuiExport.h>

#include <djvImageApplication.h>

#include <QApplication>

class djvPrefsDialog;

//! \addtogroup djvGuiMisc
//@{

//------------------------------------------------------------------------------
//! \class djvAbstractApplication
//!
//! This class provides the base functionality for applications.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT djvAbstractApplication :
    public djvAbstractImageApplication
{
public:

    //! Constructor.

    djvAbstractApplication(const QString & name, int & argc, char ** argv)
        throw (djvError);

    //! Destructor.

    virtual ~djvAbstractApplication();

    //! Get whether the user-interface has started.

    bool isValid() const;

    //! Set whether the user-interface has started.

    void setValid(bool);
    
    //! The default tool tips.
    
    static bool toolTipsDefault();

    //! Get whether tool tips are enabled.

    bool hasToolTips() const;

    //! Set whether tool tips are enabled.

    void setToolTips(bool);

    //! Show the global message dialog.
    
    void messageDialog();
    
    //! Get the global references dialog.
    
    djvPrefsDialog * prefsDialog() const;
    
    virtual int run();

    virtual QString info() const;

    virtual void printMessage(const QString &) const;

    virtual void printError(const djvError &) const;

    virtual void help() const;

    virtual QString commandLineHelp() const;

protected:

    void resetPreferencesCommandLine(QStringList &) throw (djvError);

private:

    void toolTipsUpdate();

    DJV_PRIVATE_COPY(djvAbstractApplication);
    DJV_PRIVATE_IMPLEMENTATION();
};

//------------------------------------------------------------------------------
//! \class djvApplication
//!
//! This class provides the base functionality for applications.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT djvApplication :
    public QApplication,
    public djvAbstractApplication
{
public:

    //! Constructor.

    djvApplication(const QString & name, int & argc, char ** argv) throw (djvError);
    
    virtual int run();
};

//! Get the global application instance.

#define DJV_APP \
    static_cast<djvApplication*>(djvApplication::global())

//@} // djvGuiMisc

#endif // DJV_APPLICATION_H
