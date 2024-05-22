// Basic Config
#include "globals.h"
#include "sensor.h"

#include "driver/gpio.h"
#include "esp_log.h"

#if (HAS_VOLTAGE_SENSOR)

#include <driver/adc.h>
#include <driver/gpio.h>
double voltage;

#endif // HAS_VOLTAGE_SENSOR

#define SENSORBUFFER 10 // max. size of user sensor data buffer in bytes [default=20]

void sensor_init(void) {
  // this function is called during device startup
  // put your user sensor initialization routines here
  #if (HAS_VOLTAGE_SENSOR)
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11); // GPIO32 -> ADC1_CHANNEL_4

  // Configure GPIO33 as an output pin for the relay signal
  gpio_pad_select_gpio(GPIO_NUM_33);
  gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);

  ESP_LOGI(TAG, "Voltage sensor initialized.");
  #endif
}

uint8_t sensor_mask(uint8_t sensor_no) {
  switch (sensor_no) {
  case 0:
    return (uint8_t)COUNT_DATA;
  case 1:
    return (uint8_t)SENSOR1_DATA;
  case 2:
    return (uint8_t)SENSOR2_DATA;
  case 3:
    return (uint8_t)SENSOR3_DATA;
  case 4:
    return (uint8_t)BATT_DATA;
  case 5:
    return (uint8_t)GPS_DATA;
  case 6:
    return (uint8_t)MEMS_DATA;
  case 7:
    return (uint8_t)RESERVED_DATA;
  default:
    return 0;
  }
}

uint8_t *sensor_read(uint8_t sensor) {
  static uint8_t buf[SENSORBUFFER] = {0};
  uint8_t length = 3;

  switch (sensor) {
  case 1:
    // insert user specific sensor data frames here
    buf[0] = length;
    buf[1] = 0x01;
    buf[2] = 0x02;
    buf[3] = 0x03;
    break;
  case 2:
    buf[0] = length;
    buf[1] = 0x01;
    buf[2] = 0x02;
    buf[3] = 0x03;
    break;
  case 3:
    #if (HAS_VOLTAGE_SENSOR)

    ESP_LOGI(TAG, "Reading Sensor 3, Voltage Sensor");

    int adc_reading = adc1_get_raw(ADC1_CHANNEL_4);
    ESP_LOGI(TAG, "Raw reading: %i", adc_reading);

    double V_measured = (adc_reading / 4095.0) * 3.9; // Convert ADC reading to voltage
    double V_original = V_measured * 11; // Correct the voltage using the voltage divider ratio

    ESP_LOGI(TAG, "Measured Voltage: %f V", V_measured);
    ESP_LOGI(TAG, "Original Voltage: %f V", V_original);

    int16_t voltage_int = (int16_t)(V_original * 1000); // Convert to millivolts and cast to int

    buf[0] = 4; // length of data: 1 byte for length + 3 bytes for voltage data
    buf[1] = highByte(voltage_int);
    buf[2] = lowByte(voltage_int);
    payload.addVoltage(voltage_int);
    #endif // HAS_VOLTAGE_SENSOR
    break;
  }

  return buf;
}