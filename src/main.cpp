#include "ApplicationConfig.hpp"
//

#include "kvasir/Util/using_literals.hpp"
#include "ws2812Pio/ws2812.hpp"

#include <cmake_git_version/version.hpp>
#include <cmath>
#include <kvasir/Util/StaticVector.hpp>
#include <numbers>

using namespace sc::literals;

struct RGB {
    std::array<std::uint8_t, 3> data{};
    constexpr RGB() = default;

    constexpr RGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) : data{g, r, b} {}
};

int main() {
    UC_LOG_D("{}", CMakeGitVersion::FullVersion);
    UC_LOG_D("Reset cause: {}", Kvasir::PM::reset_cause());

    auto next = Clock::time_point{};

    Kvasir::StaticVector<RGB, 1024> leds{};
    leds.resize(1);

    /*
    for(auto& led : leds) {
        led = RGB{0, 255, 0};
        UC_LOG_D("WS2812 set");
    }
    */

    std::uint16_t Duty = (Pwm_r::getTop() * (100 * 255)) / (255 * 255);
    UC_LOG_D("Duty: {} getTop:{}", Duty, Pwm_r::getTop());
    Pwm_r::setDuty(Duty);

    apply(set(HW::Pin::ws2812led{}));
    apply(set(HW::Pin::ws2812led_test{}));

    double start        = 0.0;
    double schrittweite = 1.0;

    double winkel = start;

    std::size_t colorState{0};
    while(true) {
        auto const now   = Clock::now();
        double     rad   = winkel * std::numbers::pi / 180.0;
        double     sinus = std::sin(rad);

        if(now > next) {
            next += std::chrono::milliseconds{500};
            for(auto& led : leds) {
                if(colorState == 0) {
                    led        = RGB{0, 0, 0};
                    colorState = 1;
                } else if(colorState == 1) {
                    led        = RGB{255, 0, 0};
                    colorState = 2;
                } else if(colorState == 2) {
                    led        = RGB{0, 255, 0};
                    colorState = 3;
                } else if(colorState == 3) {
                    led        = RGB{0, 0, 255};
                    colorState = 0;
                }
                UC_LOG_D("WS2812 set: {}", colorState);
            }

            if(WS2812::ready()) {
                WS2812::send(std::span{leds});
                UC_LOG_D("WS2812 send");
            }
            if(WS2812_test::ready()) {
                WS2812_test::send(std::span{leds});
                UC_LOG_D("WS2812 test send");
            }
        }
        Pwm_r::setDuty(static_cast<std::uint16_t>(Duty * sinus));

        WS2812::handler();
        WS2812_test::handler();
        StackProtector::handler();
    }
}

KVASIR_START(Startup)
