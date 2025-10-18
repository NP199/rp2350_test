#pragma once
#include "HWConfig.hpp"
///

#include "chip/rp_common/pio/ws2812.hpp"

#include <kvasir/Util/FaultHandler.hpp>
#include <kvasir/Util/StackProtector.hpp>

using Clock          = HW::SystickClock;
using StackProtector = Kvasir::StackProtector<>;
using FaultHandler   = Kvasir::Fault::Handler<HW::Fault_CleanUpAction>;
using Pwm_r          = Kvasir::PWM::PWM<HW::Pin::led_r, HW::PwmConfig>;

struct DmaConfig {
    static constexpr auto numberOfChannels     = 2;
    static constexpr auto callbackFunctionSize = 64;
};

using Dma = Kvasir::DMA::DmaBase<DmaConfig>;

struct Crc {
    using type = std::uint16_t;

    static type calc(std::span<std::byte const> data) {
        return Kvasir::CRC::calcCrc<Kvasir::CRC::CRC_Type::crc16, Dma, Dma::Channel::ch0>(data);
    }
};

struct WatchdogConfig {
    static constexpr auto clockSpeed  = HW::CrystalSpeed;
    static constexpr auto overrunTime = std::chrono::milliseconds(500);
};

using Watchdog = Kvasir::Watchdog<WatchdogConfig>;

struct WS2812Config {
    static constexpr auto ClockSpeed     = HW::ClockSpeed;
    static constexpr auto LedClockSpeed  = 800000;
    static constexpr auto PioInstance    = 0;
    static constexpr auto SmInstance     = 0;
    static constexpr auto ProgrammOffset = 0;
};

struct WS2812Config_test {
    static constexpr auto ClockSpeed     = HW::ClockSpeed;
    static constexpr auto LedClockSpeed  = 800000;
    static constexpr auto PioInstance    = 1;
    static constexpr auto SmInstance     = 0;
    static constexpr auto ProgrammOffset = 0;
};

using WS2812 = Kvasir::Pio::
  WS2812<Clock, HW::Pin::ws2812led, Dma, Dma::Channel::ch1, Dma::Priority::low, WS2812Config>;

using WS2812_test = Kvasir::Pio::
  WS2812<Clock, HW::Pin::ws2812led_test, Dma, Dma::Channel::ch1, Dma::Priority::low, WS2812Config_test>;

using Startup = Kvasir::Startup::Startup<
  HW::ClockSettings,
  Clock,
  HW::ComBackend,
  FaultHandler,
  StackProtector,
  HW::PinConfig,
  Dma,
  Crc,
  Pwm_r,
  WS2812,
  WS2812_test>;
