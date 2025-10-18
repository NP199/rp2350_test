#include "ApplicationConfig.hpp"
//

#include "kvasir/Util/using_literals.hpp"

#include <cmake_git_version/version.hpp>
#include <cmath>
#include <kvasir/Util/StaticVector.hpp>
#include <numbers>

using namespace sc::literals;

// 255
// on 255
// off 0
//
// 125
// on 125
// off 255 - 125 = 130

void pwm(std::uint8_t brightness) {
    std::uint8_t onTime{brightness};
    std::uint8_t offTime{static_cast<std::uint8_t>(255 - brightness)};
    static auto  next     = Clock::time_point{};
    static bool  ledState = false;

    auto const now = Clock::now();
    if(now > next) {
        if(ledState) {
            apply(clear(HW::Pin::led_r{}));
            next += std::chrono::milliseconds{onTime / 10};
        } else {
            apply(set(HW::Pin::led_r{}));
            next += std::chrono::milliseconds{offTime / 10};
        }
        ledState = !ledState;

        UC_LOG_D("Led: {}", ledState);
    }
}

int main() {
    UC_LOG_D("{}", CMakeGitVersion::FullVersion);
    UC_LOG_D("Reset cause: {}", Kvasir::PM::reset_cause());

    auto next = Clock::time_point{};
    //bool ledState = false;

    using Sinus = uc_log::Metric<double, "sinus"_sc, "V/m"_sc, "Global"_sc>;

    double start        = 0.0;
    double schrittweite = 1.0;

    double winkel = start;

    apply(set(HW::Pin::led_g{}));
    apply(set(HW::Pin::led_b{}));
    std::uint16_t Duty = (Pwm_r::getTop() * (100 * 255)) / (255 * 255);
    UC_LOG_D("Duty: {} getTop:{}", Duty, Pwm_r::getTop());
    Pwm_r::setDuty(Duty);
    while(true) {
        auto const now   = Clock::now();
        double     rad   = winkel * std::numbers::pi / 180.0;
        double     sinus = std::sin(rad);
        rad              = (winkel + 120) * std::numbers::pi / 180.0;
        double sinus_120 = std::sin(rad);
        rad              = (winkel + 240) * std::numbers::pi / 180.0;
        double sinus_240 = std::sin(rad);
        if(now > next) {
            UC_LOG_D("Sinus: {}", Sinus{sinus});
            next += std::chrono::milliseconds{50};
            winkel += schrittweite;
        }
        //pwm(static_cast<std::uint8_t>(255 * sinus));
        Pwm_r::setDuty(static_cast<std::uint16_t>(Duty * sinus));
        Pwm_g::setDuty(static_cast<std::uint16_t>(Duty * sinus_120));
        Pwm_b::setDuty(static_cast<std::uint16_t>(Duty * sinus_240));
        StackProtector::handler();
    }
}

KVASIR_START(Startup)
