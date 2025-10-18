#pragma once
#include "HWConfig.hpp"
///

#include <kvasir/Util/FaultHandler.hpp>
#include <kvasir/Util/StackProtector.hpp>

using Clock          = HW::SystickClock;
using StackProtector = Kvasir::StackProtector<>;
using FaultHandler   = Kvasir::Fault::Handler<HW::Fault_CleanUpAction>;
using Pwm_r          = Kvasir::PWM::PWM<HW::Pin::led_r, HW::PwmConfig>;
using Pwm_g          = Kvasir::PWM::PWM<HW::Pin::led_g, HW::PwmConfig>;
using Pwm_b          = Kvasir::PWM::PWM<HW::Pin::led_b, HW::PwmConfig>;

using Startup = Kvasir::Startup::Startup<
  HW::ClockSettings,
  Clock,
  HW::ComBackend,
  FaultHandler,
  StackProtector,
  HW::PinConfig,
  Pwm_r,
  Pwm_g,
  Pwm_b>;
