// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvCoreTest/TextSystemTest.h>

#include <djvCore/Context.h>
#include <djvCore/TextSystem.h>

using namespace djv::Core;

namespace djv
{
    namespace CoreTest
    {
        TextSystemTest::TextSystemTest(const std::shared_ptr<Core::Context>& context) :
            ITickTest("djv::CoreTest::TextSystemTest", context)
        {}
                
        void TextSystemTest::run()
        {
            if (auto context = getContext().lock())
            {
                auto system = context->getSystemT<TextSystem>();
                
                for (const auto& i : system->getLocales())
                {
                    std::stringstream ss;
                    ss << "locale: " << i;
                    _print(ss.str());
                }

                auto localeObserver = ValueObserver<std::string>::create(
                    system->observeCurrentLocale(),
                    [this](const std::string& value)
                    {
                        std::stringstream ss;
                        ss << "current locale: " << value;
                        _print(ss.str());
                    });
                
                {
                    system->setCurrentLocale("zh");
                    system->setCurrentLocale("zh");
                    DJV_ASSERT("zh" == system->observeCurrentLocale()->get());
                    system->setCurrentLocale("en");
                }
            }
        }
                
    } // namespace CoreTest
} // namespace djv

