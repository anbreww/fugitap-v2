/**
 * hardware.h
 * 
 * Contains pin definitions for the project
 */


/**
 * LCD with PWM control of backlight
 */
#define PIN_LCD_RESET   2
#define PIN_LCD_DC      4
#define PIN_LCD_CS      15
#define PIN_LCD_MISO    17
#define PIN_LCD_SCK     18
#define PIN_LCD_MOSI    23
#define PIN_LCD_LIGHT   25  // LCD backlight


/**
 * Neopixel LED output
 */
#define PIN_WS2812      13

/** 
 *  board mode pin - read with weak pull-up
 *  floating (high) : tap mode (LCD + flow meters)
 *  1k pulldown (low) : tower mode (LED + fans)
 */
#define PIN_BOARD_MODE  14 

/**
 * I2C for reading current sensor
 */
#define PIN_SCL         19
#define PIN_SDA         21

/**
 * 1-Wire bus to read temperature/humidity sensors
 */
#define PIN_1WIRE       22

/**
 * Flow sensor interaction
 * LED : LED to indicate beer is being poured
 * PULSE : input from flow sensor to count pulses
 */
#define PIN_FLOW_LED    26
#define PIN_FLOW_PULSE  27

/**
 * Tower fans
 * PWM control (28kHz standard)
 * RPM feedback from fans
 */
#define PIN_FAN1_PWM    32
#define PIN_FAN2_PWM    33
#define PIN_FAN1_RPM    34
#define PIN_FAN2_RPM    35
