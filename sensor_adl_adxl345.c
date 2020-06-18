#include "sensor_adl_adxl345.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME "ADXL345"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

static rt_size_t adxl345_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    struct rt_sensor_data *data = (struct rt_sensor_data *)buf;
    struct adxl345_3axes acce;

    adxl345_get_accel(ADXL345_SPI_DEVICE_NAME, &acce);

    data->type = RT_SENSOR_CLASS_ACCE;
    data->data.acce.x = acce.x;
    data->data.acce.y = acce.y;
    data->data.acce.z = acce.z;
    data->timestamp = rt_sensor_get_ts();

    return 1;
}

static rt_err_t adxl345_control(struct rt_sensor_device *sensor, int cmd, void *arg)
{
    rt_err_t result = RT_EOK;

    switch (cmd)
    {
    case RT_SENSOR_CTRL_GET_ID:
        adxl345_reg_read(sensor->config.intf.dev_name, ADXL345_DEVID, arg, 1);
        break;
    default:
        return -RT_ERROR;
    }

    return result;
}

static struct rt_sensor_ops sensor_ops =
{
    adxl345_fetch_data,
    adxl345_control
};


int rt_hw_adxl345_init(const char *name, struct rt_sensor_config *cfg)
{
    rt_sensor_t sensor_acce = RT_NULL;
    int result;

    sensor_acce = rt_calloc(1, sizeof(struct rt_sensor_device));
    if (sensor_acce == RT_NULL)
        return -1;

    sensor_acce->info.type       = RT_SENSOR_CLASS_ACCE;
    sensor_acce->info.vendor     = RT_SENSOR_VENDOR_UNKNOWN;
    sensor_acce->info.model      = "adxl345_acc";
    sensor_acce->info.unit       = RT_SENSOR_UNIT_MG;
    sensor_acce->info.intf_type  = RT_SENSOR_INTF_SPI;
    sensor_acce->info.range_max  = 16000;
    sensor_acce->info.range_min  = -16000;
    sensor_acce->info.period_min = 10;

    rt_memcpy(&sensor_acce->config, cfg, sizeof(struct rt_sensor_config));
    sensor_acce->ops = &sensor_ops;

    result = rt_hw_sensor_register(sensor_acce, name, RT_DEVICE_FLAG_RDWR, RT_NULL);
    if (result != RT_EOK)
    {
        LOG_E("device register err code: %d", result);
    }

    LOG_I("acce sensor init success!");

    return RT_EOK;
}
