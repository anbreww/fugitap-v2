/**
 * Fugitap V2
 * 
 * Firmware for the ESP32 version of the Fugitap project
 */

#include <Arduino.h>
#include "hardware.h"
#include "settings.h"
#include "utils.h"

#include <Wifi.h>
#include <ArduinoOTA.h>

#include <Wire.h>
#include <Adafruit_INA219.h>

#include <FlowMeter.h>

#include <SPIFFS.h>

#include "Display.h"
#include "FanPWM.h"

typedef enum
{
  LEDC_CHAN_LCD = 0,
  LEDC_POUR_LED = 1,
  LEDC_CHAN_FAN1 = 2,
  LEDC_CHAN_FAN2 = 3,
} LEDC_Channel_t;

typedef enum
{
  FAN_1 = 0,
  FAN_2
} FAN_ID_t;

typedef enum
{
  VARIANT_TOWER = 0,
  VARIANT_TAP,
} VARIANT_t;

class INA219 : public Adafruit_INA219 {
  public:
    void setCalibration_16V_800mA();
};


// FanPWM fan1(PIN_FAN1_PWM, LEDC_CHAN_FAN1, PIN_FAN1_RPM);
// FanPWM fan2(PIN_FAN2_PWM, LEDC_CHAN_FAN2, PIN_FAN2_RPM);

// define the sensor characteristics here
const double cap = 20.0f;       // l/min
const double kf = 93.3333f;     // Hz per l/min

// let's provide our own sensor properties (without applying further calibration)
FlowSensorProperties SF800 = {cap, kf, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

FlowMeter flowMeter = FlowMeter(PIN_FLOW_PULSE, SF800);

TFT_eSPI tft = TFT_eSPI();
GfxUi ui = GfxUi(&tft);
Beer beer = Beer(flowMeter, PIN_FLOW_PULSE);
Display beerScreen = Display(PIN_LCD_LIGHT, LEDC_CHAN_LCD, &tft, &ui, &beer);
Adafruit_INA219 ina219;
//WiFiClient wifiClient;


/**
 * Detect board hardware variant
 *  - Tap for beer tap with screen,
 *  - Tower for LED and Fan controller
 */
VARIANT_t detectVariant()
{
  if (digitalRead(PIN_BOARD_MODE) == LOW) {
    return VARIANT_TOWER;
  } else {
    return VARIANT_TAP;
  }
}

bool connectWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Connecting to WiFi");
  Serial.println("MAC Address: " + WiFi.macAddress());
  WiFi.setHostname(build_hostname().c_str());

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }


  ArduinoOTA.setHostname(build_hostname().c_str());
  ArduinoOTA.begin();

  ArduinoOTA.onStart([]() {
    beerScreen.setBacklight(255);
    Serial.println("OTA Starting");
    beerScreen.writeStatusBar("OTA UPDATE STARTING", TFT_ORANGE, true);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("Done! Rebooting");
    beerScreen.writeStatusBar("DONE. REBOOTING!", TFT_ORANGE, true);
  });

  Serial.println("Ready");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  beerScreen.writeStatusBar("Connected successfully", TFT_WHITE);

  String status = getUniqueID() + "   " + WiFi.localIP().toString();
  beerScreen.writeStatusBar(status.c_str(), TFT_YELLOW);

  return true;
}

void setup()
{
  // LED PWM outputs
  ledcAttachPin(PIN_LCD_LIGHT, LEDC_CHAN_LCD);
  ledcSetup(LEDC_CHAN_LCD, 20000, 8); // 20kHz, 8 bit resolution
  ledcWrite(LEDC_CHAN_LCD, 80);

  // fan1.init();
  // fan2.init();

  // fan1.SetOutput(50);
  // fan2.SetOutput(50);

  Serial.begin(115200);
  Serial.println("Starting Fugidaire V2");

  Wire.begin(PIN_SDA, PIN_SCL);

  if (!ina219.begin()) {
    Serial.println("Couldn't find INA219");
  } else {
    Serial.println("INA219 initialized successfully");
    ina219.setCalibration_16V_800mA();
  }

  if (!SPIFFS.begin(true)) {
    Serial.println("Couldn't mount filesystem");
  }

  beerScreen.begin();
  beerScreen.writeStatusBar("Connecting to WiFi", TFT_WHITE);

  connectWifi();

  delay(3000);

  beer.set_tap(7);

  beerScreen.drawBeerScreen();
  beerScreen.drawFillMeter();
}

void loop()
{
  ArduinoOTA.handle();

  delay(100);

  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();

    // Compute load voltage, power, and milliamp-hours.
  float loadvoltage = busvoltage + (shuntvoltage / 1000);
  float power_mW = loadvoltage * current_mA;
  
  char status[50];
  sprintf(status, "%.2f V | %.0f mW\n", busvoltage, power_mW);
  beerScreen.writeStatusBar(status, TFT_WHITE, true);

  float chip_temp = temperatureRead();
  sprintf(status, "%.2f V | %.1f mA | %.0f mW | %.1f °C", 
                  busvoltage, current_mA, power_mW, chip_temp);
  Serial.println(status);

  // Serial.printf("Chip temperature : %.2f\n", temperatureRead());
}