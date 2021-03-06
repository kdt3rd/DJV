// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUI/Enum.h>

#include <functional>
#include <memory>

namespace djv
{
    namespace UI
    {
        class Action;

        //! This class manages a group of actions.
        class ActionGroup : public std::enable_shared_from_this<ActionGroup>
        {
            DJV_NON_COPYABLE(ActionGroup);

        protected:
            ActionGroup();

        public:
            ~ActionGroup();

            static std::shared_ptr<ActionGroup> create(ButtonType);

            const std::vector<std::shared_ptr<Action> > & getActions() const;
            size_t getActionCount() const;
            int getActionIndex(const std::shared_ptr<Action> &) const;
            void addAction(const std::shared_ptr<Action> &);
            void removeAction(const std::shared_ptr<Action> &);
            void clearActions();

            ButtonType getButtonType() const;
            void setButtonType(ButtonType);

            void setChecked(int, bool = true, Callback = Callback::Suppress);

            void setPushCallback(const std::function<void(int)> &);
            void setToggleCallback(const std::function<void(int, bool)> &);
            void setRadioCallback(const std::function<void(int)> &);
            void setExclusiveCallback(const std::function<void(int)> &);

        private:
            DJV_PRIVATE();
        };

    } // namespace UI
} // namespace djv
