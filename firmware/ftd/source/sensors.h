#ifndef SOURCE_SENSORS_H_
#define SOURCE_SENSORS_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <common/code_utils.hpp>
#include <openthread/cli.h>

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT
#ifdef SL_CATALOG_SENSOR_LIGHT_PRESENT
#include "sl_sensor_light.h"
#endif // SL_CATALOG_SENSOR_LIGHT_PRESENT
#ifdef SL_CATALOG_SENSOR_RHT_PRESENT
#include "sl_sensor_rht.h"
#endif // SL_CATALOG_SENSOR_RHT_PRESENT
#ifdef SL_CATALOG_SENSOR_PRESSURE_PRESENT
#include "sl_sensor_pressure.h"
#endif // SL_CATALOG_SENSOR_PRESSURE_PRESENT
#ifdef SL_CATALOG_SENSOR_GAS_PRESENT
#include "sl_sensor_gas.h"
#endif // SL_CATALOG_SENSOR_GAS_PRESENT
#ifdef SL_CATALOG_SENSOR_SOUND_PRESENT
#include "sl_sensor_sound.h"
#endif // SL_CATALOG_SENSOR_SOUND_PRESENT

void sensorInit(void);
void sensorDeInit(void);

sl_status_t sl_service_light_get(float *lux, float *uvi);
sl_status_t sl_service_pressure_get(float * pressure);
sl_status_t sl_service_rht_get(uint32_t *rh, int32_t *t);
sl_status_t sl_service_gas_get(uint16_t *eco2, uint16_t *tvoc);
sl_status_t sl_service_sound_get(float *sound_level);


#endif /* SOURCE_SENSORS_H_ */
