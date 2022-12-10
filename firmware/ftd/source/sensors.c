/***********************************************************
 * General includes
 ***********************************************************/

#include "sensors.h"

/***********************************************************
 * Functions
 ***********************************************************/

void sensorInit()
{
  sl_status_t sc;
  #ifdef SL_CATALOG_SENSOR_LIGHT_PRESENT
    sc = sl_sensor_light_init();
    if (sc != SL_STATUS_OK) {
        otCliOutputFormat("Ambient light and UV index sensor initialization failed.\r\n");
    }
  #endif // SL_CATALOG_SENSOR_LIGHT_PRESENT
  #ifdef SL_CATALOG_SENSOR_RHT_PRESENT
    sc = sl_sensor_rht_init();
    if (sc != SL_STATUS_OK) {
        otCliOutputFormat("Relative Humidity and Temperature sensor initialization failed.\r\n");
    }
  #endif // SL_CATALOG_SENSOR_RHT_PRESENT
  #ifdef SL_CATALOG_SENSOR_PRESSURE_PRESENT
    sc = sl_sensor_pressure_init();
    if (sc != SL_STATUS_OK) {
        otCliOutputFormat("Air Pressure sensor initialization failed.\r\n");
    }
  #endif // SL_CATALOG_SENSOR_PRESSURE_PRESENT
  #ifdef SL_CATALOG_SENSOR_GAS_PRESENT
    sc = sl_sensor_gas_init();
    if (sc != SL_STATUS_OK) {
        otCliOutputFormat("Air quality sensor initialization failed.\r\n");
    }
  #endif // SL_CATALOG_SENSOR_GAS_PRESENT
  #ifdef SL_CATALOG_SENSOR_SOUND_PRESENT
    sc = sl_sensor_sound_init();
    if (sc != SL_STATUS_OK) {
        otCliOutputFormat("Sound level sensor initialization failed.\r\n");
    }
  #endif // SL_CATALOG_SENSOR_SOUND_PRESENT
}

void sensorDeInit()
{
  #ifdef SL_CATALOG_SENSOR_LIGHT_PRESENT
    sl_sensor_light_deinit();
  #endif // SL_CATALOG_SENSOR_LIGHT_PRESENT
  #ifdef SL_CATALOG_SENSOR_RHT_PRESENT
    sl_sensor_rht_deinit();
  #endif // SL_CATALOG_SENSOR_RHT_PRESENT
  #ifdef SL_CATALOG_SENSOR_PRESSURE_PRESENT
    sl_sensor_pressure_deinit();
  #endif // SL_CATALOG_SENSOR_PRESSURE_PRESENT
  #ifdef SL_CATALOG_SENSOR_GAS_PRESENT
    sl_sensor_gas_deinit();
  #endif // SL_CATALOG_SENSOR_GAS_PRESENT
  #ifdef SL_CATALOG_SENSOR_SOUND_PRESENT
    sl_sensor_sound_deinit();
  #endif // SL_CATALOG_SENSOR_SOUND_PRESENT
}


sl_status_t sl_service_light_get(float *lux, float *uvi)
{
  sl_status_t sc;
  sc = sl_sensor_light_get(lux, uvi);
  if (SL_STATUS_OK == sc) {
  } else if (SL_STATUS_NOT_INITIALIZED == sc) {
      otCliOutputFormat("Ambient light and UV index sensor is not initialized.\r\n");
  } else {
      otCliOutputFormat("Light sensor measurement failed.\r\n");
  }
  return sc;
}

sl_status_t sl_service_pressure_get(float *pressure)
{
  sl_status_t sc;
  sc = sl_sensor_pressure_get(pressure);
  if (SL_STATUS_OK == sc) {
  } else if (SL_STATUS_NOT_INITIALIZED == sc) {
      otCliOutputFormat("Air Pressure sensor is not initialized.\r\n");
  } else {
      otCliOutputFormat("Air Pressure measurement failed.\r\n");
  }
  return sc;
}

sl_status_t sl_service_rht_get(uint32_t *rh, int32_t *t)
{
  sl_status_t sc;
  sc = sl_sensor_rht_get(rh, t);
  if (SL_STATUS_OK == sc) {
  } else if (SL_STATUS_NOT_INITIALIZED == sc) {
      otCliOutputFormat("Relative Humidity and Temperature sensor is not initialized.\r\n");
  } else {
      otCliOutputFormat("RHT sensor measurement failed.\r\n");
  }
  return sc;
}

sl_status_t sl_service_gas_get(uint16_t *eco2, uint16_t *tvoc)
{
  sl_status_t sc;
  sc = sl_sensor_gas_get(eco2, tvoc);
  if (SL_STATUS_OK == sc) {
  } else if (SL_STATUS_NOT_INITIALIZED == sc) {
      otCliOutputFormat("Air quality sensor is not initialized.\r\n");
  } else if (SL_STATUS_NOT_READY == sc) {
      //otCliOutputFormat("Air quality sensor measurement failed.\r\n");
  }
  return sc;
}

sl_status_t sl_service_sound_get(float *sound_level)
{
  sl_status_t sc;
  sc = sl_sensor_sound_get(sound_level);
  if (SL_STATUS_OK == sc) {
  } else if (SL_STATUS_NOT_INITIALIZED == sc) {
      otCliOutputFormat("Sound level sensor is not initialized.\r\n");
  } else {
      otCliOutputFormat("Sound level measurement failed.\r\n");
  }
  return sc;
}
