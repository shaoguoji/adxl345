#ifndef __SENSOR_ADL_ADXL345_H__
#define __SENSOR_ADL_ADXL345_H__

#include <rtthread.h>

#include "adxl345.h"
#include "sensor.h"

int rt_hw_adxl345_init(const char *name, struct rt_sensor_config *cfg);

#endif /* #ifndef __SENSOR_ADL_ADXL345_H__ */
