#pragma once
#include "HWConfigCommon.hpp"

namespace HW {
namespace Pin {
    using led_r = decltype(makePinLocation(Kvasir::Io::port0, Kvasir::Io::pin16));
    using led_g = decltype(makePinLocation(Kvasir::Io::port0, Kvasir::Io::pin17));
    using led_b = decltype(makePinLocation(Kvasir::Io::port0, Kvasir::Io::pin18));
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
