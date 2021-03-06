// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvCmdLineApp/CmdLineApp.h>

#include <djvCore/Context.h>

struct GLFWwindow;

namespace djv
{
    namespace CmdLine
    {
        struct Arguments
        {
            std::unique_ptr<Core::Time::Units> timeUnits;
            std::unique_ptr<bool> logConsole;
        };

        //! This class provides a command-line application.
        class Application : public Core::Context
        {
            DJV_NON_COPYABLE(Application);

        protected:
            void _init(std::list<std::string>&);
            Application();

        public:
            ~Application() override;

            static std::shared_ptr<Application> create(std::list<std::string>&);

            virtual void run();

            int getExitCode() const;
            void exit(int);

            static std::list<std::string> args(int, char**);

        protected:
            //! Throws:
            //! - std::runtime_error
            virtual void _parseCmdLine(std::list<std::string>&);
            virtual void _printUsage();

            bool _isRunning() const;
            void _setRunning(bool);

        private:
            void _printVersion();

            DJV_PRIVATE();
        };

    } // namespace CmdLine
} // namespace djv
