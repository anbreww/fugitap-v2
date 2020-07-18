#include "FanPWM.h"

#define PWM_FREQ 28000 //!< Standard PWM frequency for computer fans : 28kHz
#define PWM_RESOLUTION  8 //!< PWM resolution : 8 bits

// limit the minimum fan speed under which it doesn't actually start
static const uint8_t min_fan_speed = 25;

FanPWM::FanPWM(uint8_t pwm_pin, uint8_t pwm_ledc_chan, uint8_t rpm_pin) {
  _pwm_pin = pwm_pin;
  _pwm_ledc_ch = pwm_ledc_chan;
  _rpm_pin = rpm_pin;
}

void FanPWM::init(void) {
  ledcAttachPin(_pwm_pin, _pwm_ledc_ch);
  ledcSetup(_pwm_ledc_ch, PWM_FREQ, 8);
  SetOutput(0);
}

void FanPWM::SetOutput(uint8_t level) {
  // below a certain level, the fan doesn't run
  if (level > 0 && level < min_fan_speed) {
    level = min_fan_speed;
  }

  ledcWrite(_pwm_ledc_ch, 255 - level);
}


