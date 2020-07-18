#include <Arduino.h>

class FanPWM {
  public:
    FanPWM(uint8_t pwm_pin, uint8_t pwm_ledc_chan, uint8_t rpm_pin);
    void init(void);
    void SetOutput(uint8_t level);

  private:
    uint8_t _pwm_pin;
    uint8_t _pwm_ledc_ch;
    uint8_t _rpm_pin;
};