// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvCore/Path.h>
#include <djvCore/Time.h>

#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace djv
{
    namespace Core
    {
        class IObject;
        class ISystemBase;
        class LogSystem;
        class ResourceSystem;
        class TextSystem;

        namespace Time
        {
            class Timer;
            class TimerSystem;

        } // namespace Time

        //! This class provides core functionality.
        class Context : public std::enable_shared_from_this<Context>
        {
            DJV_NON_COPYABLE(Context);

        protected:
            void _init(const std::string& argv0);
            Context();

        public:
            virtual ~Context();

            //! Throws:
            //! - std::exception
            static std::shared_ptr<Context> create(const std::string& argv0);

            //! Get the context name.
            const std::string & getName() const;

            //! \name Systems
            ///@{

            //! Get the list of systems.
            std::vector<std::shared_ptr<ISystemBase> > getSystems() const;

            //! Get the list of systems of the given type.
            template<typename T>
            std::vector<std::shared_ptr<T> > getSystemsT() const;

            //! Get a system of the given type. If the system is not found a
            //! null pointer is returned.
            template<typename T>
            std::shared_ptr<T> getSystemT() const;
            
            //! Remove a system.
            void removeSystem(const std::shared_ptr<ISystemBase>&);

            ///@}

            //! This function is called by the application event loop.
            virtual void tick();

            //! This function is called by the application event loop. This can
            //! be called in addition to tick() to increase the accuracy of the
            //! timers.
            void tickTimers();

            //! Get the average tick FPS.
            float getFPSAverage() const;

            //! Get the system tick times.
            const std::vector<std::pair<std::string, Time::Duration> >& getSystemTickTimes() const;

        protected:
            void _addSystem(const std::shared_ptr<ISystemBase> &);

        private:
            std::string _name;
            std::shared_ptr<Time::TimerSystem> _timerSystem;
            std::shared_ptr<ResourceSystem> _resourceSystem;
            std::shared_ptr<LogSystem> _logSystem;
            std::shared_ptr<TextSystem> _textSystem;
            std::vector<std::shared_ptr<ISystemBase> > _systems;
            std::vector<std::pair<std::string, Time::Duration> > _systemTickTimes;
            std::vector<std::pair<std::string, Time::Duration> > _systemTickTimesTemp;
            std::chrono::time_point<std::chrono::steady_clock> _fpsTime = std::chrono::steady_clock::now();
            std::list<float> _fpsSamples;
            float _fpsAverage = 0.F;
            std::shared_ptr<Time::Timer> _fpsTimer;

            friend class ISystemBase;
        };

    } // namespace Core
} // namespace djv

#include <djvCore/ContextInline.h>

