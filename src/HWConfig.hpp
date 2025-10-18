#pragma once
#include "HWConfigCommon.hpp"

namespace HW {
namespace Pin {
    using ws2812led      = decltype(makePinLocation(Kvasir::Io::port0, Kvasir::Io::pin21));
    using ws2812led_test = decltype(makePinLocation(Kvasir::Io::port0, Kvasir::Io::pin24));
    using led_r          = decltype(makePinLocation(Kvasir::Io::port0, Kvasir::Io::pin7));
}   // namespace Pin

struct Fault_CleanUpAction {
    void operator()() {}
};

struct PinConfig {
    static constexpr auto powerClockEnable = list(
      clear(Kvasir::Peripheral::RESETS::Registers<>::RESET::io_bank0),
      clear(Kvasir::Peripheral::RESETS::Registers<>::RESET::pads_bank0));

    /*static constexpr auto initStepPinConfig = list(
      makeOutput(HW::Pin::led_r{}),
      makeOutput(HW::Pin::led_g{}),
      makeOutput(HW::Pin::led_b{}));*/
};

struct PwmConfig {
    static constexpr auto clockSpeed = ClockSpeed;

    static constexpr auto top       = 4096;
    static constexpr auto frequency = 10000;
    static constexpr auto invert    = true;
};

}   // namespace HW
