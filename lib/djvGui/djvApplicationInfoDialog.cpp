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

//! \file djvApplicationInfoDialog.cpp

#include <djvApplicationInfoDialog.h>

#include <djvApplication.h>

#include <QDialogButtonBox>
#include <QTextEdit>
#include <QVBoxLayout>

//------------------------------------------------------------------------------
// djvApplicationInfoDialog::P
//------------------------------------------------------------------------------

struct djvApplicationInfoDialog::P
{
};

//------------------------------------------------------------------------------
// djvApplicationInfoDialog
//------------------------------------------------------------------------------

djvApplicationInfoDialog::djvApplicationInfoDialog() :
    _p(new P)
{
    //DJV_DEBUG("djvApplicationInfoDialog::djvApplicationInfoDialog");

    // Create the widgets.
    
    QTextEdit * textLabel = new QTextEdit;
    textLabel->setText(DJV_APP->info());
    textLabel->setReadOnly(true);
    
    QDialogButtonBox * buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Close);
    
    // Layout the widgets.

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);

    // Initialize.
    
    setWindowTitle("Information Dialog");
    
    resize(500, 400);
    
    // Setup callbacks.
    
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

djvApplicationInfoDialog::~djvApplicationInfoDialog()
{
    delete _p;
}

djvApplicationInfoDialog * djvApplicationInfoDialog::global()
{
    static djvApplicationInfoDialog * dialog = 0;
    
    if (! dialog)
    {
        dialog = new djvApplicationInfoDialog;
    }
    
    return dialog;
}
