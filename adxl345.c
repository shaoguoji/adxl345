#include <rtthread.h>
#include <rtdevice.h>

#include "adxl345.h"
#include "board.h"

int adxl345_spi_device_init(void)
{
    
    struct rt_spi_device *spi_dev;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    rt_hw_spi_device_attach(ADXL345_SPI_BUS_NAME, ADXL345_SPI_DEVICE_NAME, GPIOB, GPIO_PIN_6);
    spi_dev = (struct rt_spi_device *)rt_device_find(ADXL345_SPI_DEVICE_NAME);

	
	struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB;
    cfg.max_hz = 5000000;      /* 5M */

    rt_spi_configure(spi_dev, &cfg);
	
}
INIT_COMPONENT_EXPORT(adxl345_spi_device_init);

int adxl345_init(void)
{
    rt_uint8_t id;

    rt_thread_mdelay(10);

    adxl345_reg_read(ADXL345_SPI_DEVICE_NAME, ADXL345_DEVID, &id, 1);
    rt_kprintf("read device ID: 0x%02x\n", id);

    adxl345_reg_write(ADXL345_SPI_DEVICE_NAME, ADXL345_DATA_FORMAT, 0x0B); // ±16g, 13-BIT MODE
    adxl345_reg_write(ADXL345_SPI_DEVICE_NAME, ADXL345_POWER_CTL, 0x08); // START MEASUREMENT
    adxl345_reg_write(ADXL345_SPI_DEVICE_NAME, ADXL345_INT_ENABLE, 0x80); // ENABLE DATA_READY INTERRUPT
}
INIT_APP_EXPORT(adxl345_init);

void adxl345_reg_read(char *device_name, rt_uint8_t addr, rt_uint8_t *rev_buf, rt_uint32_t len)
{
    struct rt_spi_device *spi_dev_adxl345;     /* SPI 设备句柄 */
    struct rt_spi_message msg1, msg2;

    addr |= 0x80;

    if (len > 1)
    {
        addr |= 0x40;
    }

    /* 查找 spi 设备获取设备句柄 */
    spi_dev_adxl345 = (struct rt_spi_device *)rt_device_find(device_name);

    msg1.send_buf   = &addr;
    msg1.recv_buf   = RT_NULL;
    msg1.length     = 1;
    msg1.cs_take    = 1;
    msg1.cs_release = 0;
    msg1.next       = &msg2;

    msg2.send_buf   = RT_NULL;
    msg2.recv_buf   = rev_buf;
    msg2.length     = len;
    msg2.cs_take    = 0;
    msg2.cs_release = 1;
    msg2.next       = RT_NULL;

    rt_spi_transfer_message(spi_dev_adxl345, &msg1);
}

void adxl345_reg_write(char *device_name, rt_uint8_t addr, rt_uint8_t value)
{
    struct rt_spi_device *spi_dev_adxl345;     /* SPI 设备句柄 */
    struct rt_spi_message msg1;
    rt_uint8_t send_buf[2];

    send_buf[0] = addr;
    send_buf[1] = value;

    /* 查找 spi 设备获取设备句柄 */
    spi_dev_adxl345 = (struct rt_spi_device *)rt_device_find(device_name);

    msg1.send_buf   = send_buf;
    msg1.recv_buf   = RT_NULL;
    msg1.length     = 2;
    msg1.cs_take    = 1;
    msg1.cs_release = 1;
    msg1.next       = RT_NULL;

    rt_spi_transfer_message(spi_dev_adxl345, &msg1);
}

static rt_err_t adxl345_get_accel_raw(char *device_name, struct adxl345_3axes *accel)
{
    rt_uint8_t buffer[6];

    adxl345_reg_read(device_name, ADXL345_DATAX0, buffer, 6);

    accel->x = ((rt_uint16_t)buffer[1] << 8) + buffer[0];
    accel->y = ((rt_uint16_t)buffer[3] << 8) + buffer[2];
    accel->z = ((rt_uint16_t)buffer[5] << 8) + buffer[4];

    return RT_EOK;
}

rt_err_t adxl345_get_accel(char *device_name, struct adxl345_3axes *accel)
{
    struct adxl345_3axes tmp;
    rt_err_t res;

    res = adxl345_get_accel_raw(device_name, &tmp);
    if (res != RT_EOK)
    {
        return res;
    }

    accel->x = (rt_int16_t)(tmp.x * 3.9);
    accel->y = (rt_int16_t)(tmp.y * 3.9);
    accel->z = (rt_int16_t)(tmp.z * 3.9);

    return RT_EOK;
}

