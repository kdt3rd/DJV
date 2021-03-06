// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUIComponents/ISettingsWidget.h>

namespace djv
{
    namespace ViewApp
    {
        //! This class provides the playback settings widget.
        class PlaybackSettingsWidget : public UI::ISettingsWidget
        {
            DJV_NON_COPYABLE(PlaybackSettingsWidget);

        protected:
            void _init(const std::shared_ptr<Core::Context>&);
            PlaybackSettingsWidget();

        public:
            static std::shared_ptr<PlaybackSettingsWidget> create(const std::shared_ptr<Core::Context>&);

            std::string getSettingsName() const override;
            std::string getSettingsGroup() const override;
            std::string getSettingsSortKey() const override;

        protected:
            void _initEvent(Core::Event::Init&) override;

        private:
            DJV_PRIVATE();
        };

        //! This class provides the timeline settings widget.
        class TimelineSettingsWidget : public UI::ISettingsWidget
        {
            DJV_NON_COPYABLE(TimelineSettingsWidget);

        protected:
            void _init(const std::shared_ptr<Core::Context>&);
            TimelineSettingsWidget();

        public:
            static std::shared_ptr<TimelineSettingsWidget> create(const std::shared_ptr<Core::Context>&);

            std::string getSettingsName() const override;
            std::string getSettingsGroup() const override;
            std::string getSettingsSortKey() const override;

        protected:
            void _initEvent(Core::Event::Init&) override;

        private:
            DJV_PRIVATE();
        };

    } // namespace ViewApp
} // namespace djv

