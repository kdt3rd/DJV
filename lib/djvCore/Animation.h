// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvCore/Enum.h>
#include <djvCore/ISystem.h>
#include <djvCore/Time.h>

#include <chrono>
#include <functional>

namespace djv
{
    namespace Core
    {
        //! This namespace provides animation functionality.
        namespace Animation
        {
            class System;

            //! This enumeration provides the animation types.
            enum class Type
            {
                Linear,
                EaseIn,
                EaseOut,
                SmoothStep,
                Sine,

                Count,
                First = Linear,
                Last = Sine
            };
            DJV_ENUM_HELPERS(Type);

            //! This typedef provides an animation function.
            typedef std::function<float(float)> Function;

            //! Get an animation function.
            Function getFunction(Type);

            //! This class provides an animated value.
            class Animation : public std::enable_shared_from_this<Animation>
            {
                DJV_NON_COPYABLE(Animation);
                void _init(const std::shared_ptr<Context>&);
                Animation();

            public:
                //! Create a new animation.
                static std::shared_ptr<Animation> create(const std::shared_ptr<Context>&);

                //! \name Animation Type
                ///@{

                Type getType() const;
                void setType(Type);

                ///@}

                //! \name Animation Options
                ///@{

                bool isRepeating() const;
                void setRepeating(bool);

                ///@}

                //! Get whether the animation is active.
                bool isActive() const;

                typedef std::function<void(float)> Callback;

                //! Start the animation.
                void start(
                    float            begin,
                    float            end,
                    Time::Duration   duration,
                    const Callback & callback,
                    const Callback & endCallback = nullptr);

                //! Stop the animation.
                void stop();

            private:
                void _tick();

                Type                      _type        = Type::Linear;
                Function                  _function;
                bool                      _repeating   = false;
                bool                      _active      = false;
                float                     _begin       = 0.F;
                float                     _end         = 0.F;
                Time::Duration            _duration    = Time::Duration::zero();
                Callback                  _callback;
                Callback                  _endCallback;
                std::chrono::time_point<std::chrono::steady_clock> _time;
                std::chrono::time_point<std::chrono::steady_clock> _start;

                friend class System;
            };

            //! This class provides an animation system.
            class System : public ISystem
            {
                DJV_NON_COPYABLE(System);
                void _init(const std::shared_ptr<Context>&);
                System();

            public:
                virtual ~System();

                static std::shared_ptr<System> create(const std::shared_ptr<Context>&);

                void tick() override;

            private:
                void _addAnimation(const std::weak_ptr<Animation> &);

                DJV_PRIVATE();

                friend class Animation;
            };

        } // namespace Animation
    } // namespace Core

    DJV_ENUM_SERIALIZE_HELPERS(Core::Animation::Type);
    DJV_ENUM_SERIALIZE_HELPERS(Core::Animation::Type);

} // namespace djv

#include <djvCore/AnimationInline.h>

