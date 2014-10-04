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

//! \file djvTime.h

#ifndef DJV_TIME_H
#define DJV_TIME_H

#include <djvSpeed.h>

#include <QtGlobal>

#if ! defined(DJV_WINDOWS)
#include <sys/time.h>
#else
#include <time.h>
#endif

//! \addtogroup djvCoreMisc
//@{

//------------------------------------------------------------------------------
//! \struct djvTime
//!
//! This struct provides time utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT djvTime
{
    //! Get the current time.

    static ::time_t current();

    //! Go to sleep.

    static void sleep(double seconds);

    //! Convert seconds to a time.

    static void secondsToTime(
        double,
        int &    hours,
        int &    minutes,
        double & seconds);

    //! Convert a time to a string.

    static QString timeToString(::time_t);

    //! Get a time label.

    static QString labelTime(double seconds);

    //! Convert timecode to time.

    static void timecodeToTime(
        quint32,
        int & hour,
        int & minute,
        int & second,
        int & frame);

    //! Convert time to timecode.

    static quint32 timeToTimecode(
        int hour,
        int minute,
        int second,
        int frame);

    //! Convert timecode to a frame.

    static qint64 timecodeToFrame(quint32 timecode, const djvSpeed &);

    //! Convert a frame to timecode.

    static quint32 frameToTimecode(qint64 frame, const djvSpeed &);

    //! Convert timecode to a string.

    static QString timecodeToString(quint32);

    //! Convert a string to timecode.

    static quint32 stringToTimecode(const QString &, bool * ok = 0);

    //! Convert a keycode to a string.

    static QString keycodeToString(
        int id,
        int type,
        int prefix,
        int count,
        int offset);

    //! Convert a string to a keycode.

    static bool stringToKeycode(
        const QString &,
        int & id,
        int & type,
        int & prefix,
        int & count,
        int & offset);

    //! This enumeration provides the time units.

    enum UNITS
    {
        UNITS_TIMECODE,
        UNITS_FRAMES,

        UNITS_COUNT
    };

    //! Get the time units labels.

    static const QStringList & unitsLabels();

    //! Convert a frame to a string.

    static QString unitsToString(qint64 frame, const djvSpeed &);

    //! Convert a string to a frame.

    static qint64 stringToUnits(const QString &, const djvSpeed &, bool * ok = 0);

    //! Get the default time units.

    static UNITS unitsDefault();

    //! Set the default time units.

    static void setUnitsDefault(UNITS);
    
    //! Reset the default time units.
    
    static void resetUnitsDefault();
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT QStringList & operator >> (QStringList &, djvTime::UNITS &)
    throw (QString);

DJV_CORE_EXPORT QStringList & operator << (QStringList &, djvTime::UNITS);

//@} // djvCoreMisc

#endif // DJV_TIME_H

