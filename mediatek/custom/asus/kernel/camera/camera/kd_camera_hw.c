#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/xlog.h>

#include "kd_camera_hw.h"

#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_camera_feature.h"

/******************************************************************************
 * Debug configuration
******************************************************************************/
#define PFX "[kd_camera_hw]"
#define PK_DBG_NONE(fmt, arg...)    do {} while (0)
#define PK_DBG_FUNC(fmt, arg...)    printk(KERN_INFO PFX "%s: " fmt, __FUNCTION__ ,##arg)

#define DEBUG_CAMERA_HW_K
#ifdef DEBUG_CAMERA_HW_K
#define PK_DBG PK_DBG_FUNC
#define PK_ERR(fmt, arg...)         printk(KERN_ERR PFX "%s: " fmt, __FUNCTION__ ,##arg)
#define PK_XLOG_INFO(fmt, args...) \
                do {    \
                    xlog_printk(ANDROID_LOG_INFO, "kd_camera_hw", fmt, ##args); \
                } while(0)
#else
#define PK_DBG(a,...)
#define PK_ERR(a,...)
#define PK_XLOG_INFO(fmt, args...)
#endif

extern void ISP_MCLK1_EN(BOOL En); //ME173X Device is GATED

int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, BOOL On, char* mode_name)
{
	u32 pinSetIdx = 0;//default main sensor

	#define IDX_PS_CMRST 0
	#define IDX_PS_CMPDN 4

	#define IDX_PS_MODE 1
	#define IDX_PS_ON   2
	#define IDX_PS_OFF  3


	u32 pinSet[3][8] = {
		//for main sensor
		{
			GPIO_CAMERA_CMRST_PIN,
			GPIO_CAMERA_CMRST_PIN_M_GPIO,   /* mode */
			GPIO_OUT_ONE,                   /* ON state */
			GPIO_OUT_ZERO,                  /* OFF state */
			GPIO_CAMERA_CMPDN_PIN,
			GPIO_CAMERA_CMPDN_PIN_M_GPIO,
			GPIO_OUT_ONE,
			GPIO_OUT_ZERO,
		},
		//for sub sensor
		{
			GPIO_CAMERA_CMRST1_PIN,
			GPIO_CAMERA_CMRST1_PIN_M_GPIO,
			GPIO_OUT_ZERO,
			GPIO_OUT_ONE,
			GPIO_CAMERA_CMPDN1_PIN,
			GPIO_CAMERA_CMPDN1_PIN_M_GPIO,
			GPIO_OUT_ZERO,
			GPIO_OUT_ONE,
		},
		//for main_2 sensor
		{
			GPIO_CAMERA_INVALID,	//GPIO_CAMERA_2_CMRST_PIN,
			GPIO_CAMERA_2_CMRST_PIN_M_GPIO,   /* mode */
			GPIO_OUT_ONE,                   /* ON state */
			GPIO_OUT_ZERO,                  /* OFF state */
			GPIO_CAMERA_INVALID,	//GPIO_CAMERA_2_CMPDN_PIN,
			GPIO_CAMERA_2_CMPDN_PIN_M_GPIO,
			GPIO_OUT_ZERO,
			GPIO_OUT_ONE,
		}
	};

	if (DUAL_CAMERA_MAIN_SENSOR == SensorIdx){
		pinSetIdx = 0;
	}
	else if (DUAL_CAMERA_SUB_SENSOR == SensorIdx) {
		pinSetIdx = 1;
	}
	else if (DUAL_CAMERA_MAIN_2_SENSOR == SensorIdx) {
		pinSetIdx = 2;
		return 0;	//Skip Main2 camera flow
	}

	//POWER ON
	if (On) {
		//Disable active camera
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
			mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE]);
			mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT);
			mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF]);
			mdelay(1);
		}
		//~Disable active camera

		PK_DBG_FUNC("camera sensor-pinSetIdx:%d\n",pinSetIdx);

		if(pinSetIdx == 1) //MT9M114_MIPI_RAW - Sub
		{
            PK_DBG("[ON_MT9M114 case 2.8V]sensorIdx:%d \n",SensorIdx);

            ISP_MCLK1_EN(0); //Device is GATED

 			if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name)) //VGP1
			{
				PK_DBG("[CAMERA SENSOR] Fail to enable SUB digital power\n");
			}
			
			mdelay(10);
						
			if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))  //VCAMA
			{
				PK_DBG("[CAMERA SENSOR] Fail to enable SUB analog power\n");
			}
			mdelay(10);

            ISP_MCLK1_EN(1); //Device is GATED

            mdelay(2);
		}

		//enable active sensor
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMPDN]) {
			mdelay(5);
			mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE]);
			mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT);
			mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON]);
		}
	}
	else {
		if(pinSetIdx == 1) { //MT9M114_MIPI_RAW - Sub
            ISP_MCLK1_EN(0); //Device is GATED

            mdelay(2);

            if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A, mode_name)) { //VCAMA
                PK_DBG("[CAMERA SENSOR] Fail to OFF SUB digital power\n");
                //return -EIO;
			mdelay(10);
                goto _kdCISModulePowerOn_exit_;
            }

            if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D,mode_name)) { //VGP1
                PK_DBG("[CAMERA SENSOR] Fail to OFF SUB analog power\n");
                //return -EIO;
			mdelay(10);
                goto _kdCISModulePowerOn_exit_;
            }
		}
	}

	return 0;

	_kdCISModulePowerOn_exit_:
	return -EIO;
}

EXPORT_SYMBOL(kdCISModulePowerOn);


//!--
//




