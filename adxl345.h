#ifndef __ADXL2345_H__
#define __ADXL2345_H__

#define ADXL345_SPI_DEVICE_NAME     "spi10"

/*************************** REGISTER MAP ***************************/
#define ADXL345_DEVID							0x00		// Device ID
#define ADXL345_RESERVED1						0x01		// Reserved. Do Not Access. 
#define ADXL345_THRESH_TAP						0x1D		// Tap Threshold. 
#define ADXL345_OFSX							0x1E		// X-Axis Offset. 
#define ADXL345_OFSY							0x1F		// Y-Axis Offset.
#define ADXL345_OFSZ							0x20		// Z- Axis Offset.
#define ADXL345_DUR								0x21		// Tap Duration.
#define ADXL345_LATENT							0x22		// Tap Latency.
#define ADXL345_WINDOW							0x23		// Tap Window.
#define ADXL345_THRESH_ACT						0x24		// Activity Threshold
#define ADXL345_THRESH_INACT					0x25		// Inactivity Threshold
#define ADXL345_TIME_INACT						0x26		// Inactivity Time
#define ADXL345_ACT_INACT_CTL					0x27		// Axis Enable Control for Activity and Inactivity Detection
#define ADXL345_THRESH_FF						0x28		// Free-Fall Threshold.
#define ADXL345_TIME_FF							0x29		// Free-Fall Time.
#define ADXL345_TAP_AXES						0x2A		// Axis Control for Tap/Double Tap.
#define ADXL345_ACT_TAP_STATUS				    0x2B		// Source of Tap/Double Tap
#define ADXL345_BW_RATE							0x2C		// Data Rate and Power mode Control
#define ADXL345_POWER_CTL						0x2D		// Power-Saving Features Control
#define ADXL345_INT_ENABLE						0x2E		// Interrupt Enable Control
#define ADXL345_INT_MAP							0x2F		// Interrupt Mapping Control
#define ADXL345_INT_SOURCE						0x30		// Source of Interrupts
#define ADXL345_DATA_FORMAT						0x31		// Data Format Control
#define ADXL345_DATAX0							0x32		// X-Axis Data 0
#define ADXL345_DATAX1							0x33		// X-Axis Data 1
#define ADXL345_DATAY0							0x34		// Y-Axis Data 0
#define ADXL345_DATAY1							0x35		// Y-Axis Data 1
#define ADXL345_DATAZ0							0x36		// Z-Axis Data 0
#define ADXL345_DATAZ1							0x37		// Z-Axis Data 1
#define ADXL345_FIFO_CTL						0x38		// FIFO Control
#define ADXL345_FIFO_STATUS						0x39		// FIFO Status

/* 3-axis data structure */
struct adxl345_3axes
{
    rt_int16_t x;
    rt_int16_t y;
    rt_int16_t z;
};

void adxl345_reg_read(char * device_name, rt_uint8_t addr, rt_uint8_t *rev_buf, rt_uint32_t len);
void adxl345_reg_write(char *device_name, rt_uint8_t addr, rt_uint8_t value);
rt_err_t adxl345_get_accel(char *device_name, struct adxl345_3axes *accel);

#endif /* #ifndef __ADXL2345_H__ */
