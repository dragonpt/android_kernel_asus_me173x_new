/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/xlog.h>
#include <linux/kernel.h>//for printk


#include "kd_camera_hw.h"

#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_camera_feature.h"

/******************************************************************************
 * Debug configuration
******************************************************************************/
#define PFX "[kd_camera_hw]"
#define PK_DBG_NONE(fmt, arg...)    do {} while (0)
#define PK_DBG_FUNC(fmt, arg...)    xlog_printk(ANDROID_LOG_INFO, PFX , fmt, ##arg)

#define DEBUG_CAMERA_HW_K
#ifdef DEBUG_CAMERA_HW_K
#define PK_DBG PK_DBG_FUNC
#define PK_ERR(fmt, arg...)         xlog_printk(ANDROID_LOG_ERR, PFX , fmt, ##arg)
#define PK_XLOG_INFO(fmt, args...) \
                do {    \
                   xlog_printk(ANDROID_LOG_INFO, PFX , fmt, ##arg); \
                } while(0)
#else
#define PK_DBG(a,...)
#define PK_ERR(a,...)
#define PK_XLOG_INFO(fmt, args...)
#endif

#define CAMERADB(fmt, args...) printk(KERN_ERR"[CAMERADB]%s %d : "fmt, __FUNCTION__, __LINE__, ##args)


int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, BOOL On, char* mode_name)
{
u32 pinSetIdx = 0;//default main sensor

#define IDX_PS_CMRST 0
#define IDX_PS_CMPDN 4

	#define IDX_PS_MODE 1
	#define IDX_PS_ON   2
	#define IDX_PS_OFF  3
	u32 pinSet[3][8] = 
	{
	    //for main sensor 
	    {
			GPIO_CAMERA_CMRST_PIN,
	        GPIO_CAMERA_CMRST_PIN_M_GPIO,   /* mode */
	        GPIO_OUT_ONE,                   /* ON state */
	        GPIO_OUT_ZERO,                  /* OFF state */
	     	GPIO_CAMERA_CMPDN_PIN,
	        GPIO_CAMERA_CMPDN_PIN_M_GPIO,
	        GPIO_OUT_ZERO,
	        GPIO_OUT_ONE,
		
	    },
	    //for sub sensor 
	    {
			GPIO_CAMERA_CMRST1_PIN,
	     	GPIO_CAMERA_CMRST1_PIN_M_GPIO,
	        GPIO_OUT_ONE,
	        GPIO_OUT_ZERO,
	     	GPIO_CAMERA_CMPDN1_PIN,
	        GPIO_CAMERA_CMPDN1_PIN_M_GPIO,
	        GPIO_OUT_ZERO,
	        GPIO_OUT_ONE,
//			GPIO_OUT_ONE,
//			GPIO_OUT_ZERO,
			

	    },
	    //for main_2 sensor 
	    {
			GPIO_CAMERA_2_CMRST_PIN,
	        GPIO_CAMERA_2_CMRST_PIN_M_GPIO,   /* mode */
	        GPIO_OUT_ONE,                   /* ON state */
	        GPIO_OUT_ZERO,                  /* OFF state */
	     	GPIO_CAMERA_2_CMPDN_PIN,
	        GPIO_CAMERA_2_CMPDN_PIN_M_GPIO,
	        GPIO_OUT_ZERO,
	        GPIO_OUT_ONE,
	    }
	};

	//zhaoshaopeng for combo start
	u32 pinSet_mainf[3][8] = 
	{
	    //for main sensor 
	    {
			GPIO_CAMERA_CMRST_PIN,
	        GPIO_CAMERA_CMRST_PIN_M_GPIO,   /* mode */
	        GPIO_OUT_ONE,                   /* ON state */
	        GPIO_OUT_ZERO,                  /* OFF state */
	     	GPIO_CAMERA_CMPDN_PIN,
	        GPIO_CAMERA_CMPDN_PIN_M_GPIO,
	        //zhaoshaopeng for sanglaishi 4e1 20120517 start
	        GPIO_OUT_ONE,//GPIO_OUT_ZERO,
	        GPIO_OUT_ZERO,//GPIO_OUT_ONE,
	        //zhaoshaopeng end
	    },
	    //for sub sensor 
	    {
			GPIO_CAMERA_CMRST1_PIN,
	     	GPIO_CAMERA_CMRST1_PIN_M_GPIO,
	        GPIO_OUT_ONE,
	        GPIO_OUT_ZERO,
	     	GPIO_CAMERA_CMPDN1_PIN,
	        GPIO_CAMERA_CMPDN1_PIN_M_GPIO,
//	        GPIO_OUT_ZERO,
//	        GPIO_OUT_ONE,
			GPIO_OUT_ONE,
			GPIO_OUT_ZERO,
			

	    },
	    //for main_2 sensor 
	    {
			GPIO_CAMERA_2_CMRST_PIN,
	        GPIO_CAMERA_2_CMRST_PIN_M_GPIO,   /* mode */
	        GPIO_OUT_ONE,                   /* ON state */
	        GPIO_OUT_ZERO,                  /* OFF state */
	     	GPIO_CAMERA_2_CMPDN_PIN,
	        GPIO_CAMERA_2_CMPDN_PIN_M_GPIO,
	        GPIO_OUT_ZERO,
	        GPIO_OUT_ONE,
	    }
	};
	//zhaoshaopeng end

		//zhaoshaopeng for combo start
	u32 pinSet_subf[3][8] = 
	{
	    //for main sensor 
	    {
		 GPIO_CAMERA_CMRST_PIN,
	        GPIO_CAMERA_CMRST_PIN_M_GPIO,   /* mode */
	        GPIO_OUT_ONE,                   /* ON state */
	        GPIO_OUT_ZERO,                  /* OFF state */
	     	 GPIO_CAMERA_CMPDN_PIN,
	        GPIO_CAMERA_CMPDN_PIN_M_GPIO,
	        GPIO_OUT_ZERO,//GPIO_OUT_ZERO,
	        GPIO_OUT_ONE,//GPIO_OUT_ONE,
	    },
	    //for sub sensor 
	    {
		GPIO_CAMERA_CMRST1_PIN,
	     	GPIO_CAMERA_CMRST1_PIN_M_GPIO,
	       GPIO_OUT_ONE,
	       GPIO_OUT_ZERO,
	     	GPIO_CAMERA_CMPDN1_PIN,
	       GPIO_CAMERA_CMPDN1_PIN_M_GPIO,
	       GPIO_OUT_ONE,
		GPIO_OUT_ZERO,
	    },
	    //for main_2 sensor 
	    {
			GPIO_CAMERA_2_CMRST_PIN,
	        GPIO_CAMERA_2_CMRST_PIN_M_GPIO,   /* mode */
	        GPIO_OUT_ONE,                   /* ON state */
	        GPIO_OUT_ZERO,                  /* OFF state */
	     	GPIO_CAMERA_2_CMPDN_PIN,
	        GPIO_CAMERA_2_CMPDN_PIN_M_GPIO,
	        GPIO_OUT_ZERO,
	        GPIO_OUT_ONE,
	    }
	};


	CAMERADB("SensorIdx=%d,currSensorName=%s\n",SensorIdx,currSensorName);

    if (DUAL_CAMERA_MAIN_SENSOR == SensorIdx){
        pinSetIdx = 0;
    }
    else if (DUAL_CAMERA_SUB_SENSOR == SensorIdx) {
        pinSetIdx = 1;
    }
    else if (DUAL_CAMERA_MAIN_SECOND_SENSOR == SensorIdx) {
        pinSetIdx = 2;
		CAMERADB("SensorIdx==3,return error\n");
		return -EIO;
    }

if(currSensorName)
{
//zhaoshaopeng add start
//if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K5CAGX_MIPI_YUV,currSensorName)))
//{
//    //power ON
//    if (On) {
//        //in case
//	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K5CAGX_MIPI_YUV power on  \r\n");			
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
//#endif

///*
//	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
//	Power Configuration:
//		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
//		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
//*/
//  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
//		//1.

//           //disable inactive sensor
//        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//        }
//		
//       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
//   	     //reset pin
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//            mdelay(1);

//            //PDN pin
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//        }
//  
//        //zhaoshaopeng vcore
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng from VOL_1500
//        {
//             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//             //return -EIO;
//             goto _kdCISModulePowerOn_exit_;
//        }    		
//        mdelay(50);
//       
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        mdelay(50);
//        //zhaoshaopeng af
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        } 

//        // wait power to be stable 
//        mdelay(5); 
//	//zhaoshaopeng vdd_io	
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//		      
//        //enable active sensor
//        //RST pin
//        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
//            //PDN pin
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//	     mdelay(5); 		
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//	     mdelay(10); 
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
//        }
//	 mdelay(20); 
//    }
//    else {//power OFF
//        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K5CAGX_MIPI_YUV power off \r\n");	
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set GPIO 94 for power OFF\n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
//#endif

//        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
//        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//            mdelay(1);
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            mdelay(1);     
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
//        }
//       mdelay(5); 
//    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }     	
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }                    
//    }//

//}
if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K4E1GA_MIPI_RAW,currSensorName)))  //zhaoshaopeng for sanglaishi and other 4e1 20120524
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add /SENSOR_DRVNAME_S5K4E1GA_MIPI_RAW  power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
  
        //zhaoshaopeng vcore

        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng from VOL_1500
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    

        mdelay(50);
       
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(50);
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 	      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K4E1GA_MIPI_RAW power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }   

#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  

		
    }//

}
else if(currSensorName && ((0 == strcmp(SENSOR_DRVNAME_OV8825_MIPI_RAW,currSensorName))||(0 == strcmp(SENSOR_DRVNAME_OV8830_RAW,currSensorName))))  //zhaoshaopeng for sanglaishi and other 4e1 20120524
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add /SENSOR_DRVNAME_OV8825_MIPI_RAW 8820 8830  power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(50);
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
	 mdelay(50);
	 //zhaoshaopeng vcore
	 {
            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))//ov8825 8830
            {
                 PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                 //return -EIO;
                 goto _kdCISModulePowerOn_exit_;
            }
	 }
        
        // wait power to be stable 
        mdelay(5); 
	      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}	
            			
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_OV8825_MIPI_RAW   8820 8830 power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
else if(currSensorName && ((0 == strcmp(SENSOR_DRVNAME_OV12830_MIPI_RAW,currSensorName))||(0 == strcmp(SENSOR_DRVNAME_OV8830_RAW,currSensorName))))  //zhaoshaopeng for sanglaishi and other 4e1 20120524
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add /SENSOR_DRVNAME_OV12830_MIPI_RAW  power on  \r\n");			
 
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(50);
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
	 mdelay(50);
	 //zhaoshaopeng vcore
	 {
            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))//ov8825 8830
            {
                 PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                 //return -EIO;
                 goto _kdCISModulePowerOn_exit_;
            }
	 }
        
        // wait power to be stable 
        mdelay(5); 
	      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}	
            			
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_OV12830_MIPI_RAW   8820 8830 power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}

//else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_MT9P017_MIPI_RAW,currSensorName)))  //zhaoshaopeng FOR MT9P017 20120620
//{
//    //power ON
//    if (On) {
//        //in case
//	printk("\r\n zhaoshaopeng add /SENSOR_DRVNAME_MT9P017_MIPI_RAW power on  \r\n");			
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
//#endif

///*
//	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
//	Power Configuration:
//		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
//		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
//*/
//  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
//		//1.

//           //disable inactive sensor
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//        }
//		
//       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//   	     //reset pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//            mdelay(1);

//            //PDN pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//        }

//  	//zhaoshaopeng vdd_io	
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//	 mdelay(8);
//        //zhaoshaopeng vcore
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng from VOL_1500
//        {
//             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//             //return -EIO;
//             goto _kdCISModulePowerOn_exit_;
//        }    		
//        mdelay(240);
//       
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        mdelay(50);
//        //zhaoshaopeng af
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        } 

//        // wait power to be stable 
//        mdelay(5); 

//		      
//        //enable active sensor
//        //RST pin
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//            //PDN pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//	     mdelay(5); 		
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//	     mdelay(10); 
//            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
//        }
//	 mdelay(20); 
//    }
//    else {//power OFF
//        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_MT9P017_MIPI_RAW power off \r\n");	
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set GPIO 94 for power OFF\n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
//#endif

//        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//            mdelay(1);
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            mdelay(1);     
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
//        }
//       mdelay(5); 
//    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }     	
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }                    
//    }//

//}
//else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K3H2YX_MIPI_RAW,currSensorName)))  //zhaoshaopeng FOR MT9P017 20120620
//{
//    //power ON
//    if (On) {
//        //in case
//	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K3H2YX_MIPI_RAW power on  \r\n");			
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
//#endif

///*
//	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
//	Power Configuration:
//		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
//		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
//*/
//  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
//		//1.

//           //disable inactive sensor
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//        }
//		
//       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//   	     //reset pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//            mdelay(1);

//            //PDN pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//        }
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        mdelay(8);
//        //zhaoshaopeng vcore
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1200,mode_name))//zhaoshaopeng from VOL_1500
//        {
//             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//             //return -EIO;
//             goto _kdCISModulePowerOn_exit_;
//        }    		
//        mdelay(8);
// 		
//  	//zhaoshaopeng vdd_io	
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//	 mdelay(8);
//      

//        //zhaoshaopeng af
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        } 

//        // wait power to be stable 
//        mdelay(5); 

//		      
//        //enable active sensor
//        //RST pin
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//            //PDN pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//	     mdelay(5); 		
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//	     mdelay(10); 
//            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
//        }
//	 mdelay(24); 
//    }
//    else {//power OFF
//        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K3H2YX_MIPI_RAW power off \r\n");	
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set GPIO 94 for power OFF\n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
//#endif

//        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//            mdelay(1);
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            mdelay(1);     
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
//        }
//       mdelay(5); 
//    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }     	
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }                    
//    }//

//}
	else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_AR0833_MIPI_RAW,currSensorName))) // ||(0 == strcmp(SENSOR_DRVNAME_OV5650MIPI_RAW,currSensorName))))  //zhaoshaopeng for ov5647 20120524
	{
  //power ON
    if (On) {
        //in case
		printk("SENSOR_DRVNAME_AR0833_MIPI_RAW  power on  \r\n");			
           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }

        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 	mdelay(10); 
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(50);
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
		mdelay(50);
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//ov8825 8830
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }
    
        // wait power to be stable 
        mdelay(5); 
	      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     	mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     	mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}	
            			
        }
	 	mdelay(20); 
    }
    else {//power OFF
        printk("SENSOR_DRVNAME_AR0833_MIPI_RAW power off \r\n");	
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       	mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//
}

else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_A5142_MIPI_RAW,currSensorName))) // ||(0 == strcmp(SENSOR_DRVNAME_OV5650MIPI_RAW,currSensorName))))  //zhaoshaopeng for ov5647 20120524
{
  //power ON
    if (On) {
        //in case
	printk("SENSOR_DRVNAME_A5142_MIPI_RAW  power on  \r\n");			

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }

        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
		 mdelay(10); 
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(50);
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
	 	mdelay(50);
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//ov8825 8830
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }
    
        // wait power to be stable 
        mdelay(5); 
	      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}	
            			
        }
	 	mdelay(20); 
    }
    else {//power OFF
        printk("SENSOR_DRVNAME_A5142_MIPI_RAW power off \r\n");	

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//
}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_IMX111_MIPI_RAW,currSensorName))) // ||(0 == strcmp(SENSOR_DRVNAME_OV5650MIPI_RAW,currSensorName))))  //zhaoshaopeng for ov5647 20120524
{
  //power ON
    if (On) {
        //in case
	printk("SENSOR_DRVNAME_IMX111_MIPI_RAW  power on  \r\n");			

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }

        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(50);
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
	 mdelay(50);

            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))//ov8825 8830
            {
                 PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                 //return -EIO;
                 goto _kdCISModulePowerOn_exit_;
            }
        
        // wait power to be stable 
        mdelay(5); 
	      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}	
            			
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("SENSOR_DRVNAME_IMX111_MIPI_RAW power off \r\n");	

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//
}
#if 1
//ov5648 works as sub cam
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV5648_MIPI_RAW,currSensorName))) // ||(0 == strcmp(SENSOR_DRVNAME_OV5650MIPI_RAW,currSensorName))))  //zhaoshaopeng for ov5647 20120524
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add /SENSOR_DRVNAME_OV5648_MIPI_RAW  power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

        #if 0  
               mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
               mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
               mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ONE);   

               mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
               mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
               mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);    
         #endif  
/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_subf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_subf[1-pinSetIdx][IDX_PS_CMRST],pinSet_subf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_subf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_subf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_subf[1-pinSetIdx][IDX_PS_CMRST],pinSet_subf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_subf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_subf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_subf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_subf[pinSetIdx][IDX_PS_CMRST],pinSet_subf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_subf[pinSetIdx][IDX_PS_CMRST],pinSet_subf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],pinSet_subf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],pinSet_subf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 udelay(10); 
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        udelay(50);
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
	 udelay(50);
	 //zhaoshaopeng vcore
	 {
            if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))
            {
                 PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
                 //return -EIO;
                 goto _kdCISModulePowerOn_exit_;
            }
	 }
        
        // wait power to be stable 
        mdelay(5); 
	      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_subf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],pinSet_subf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],pinSet_subf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_subf[pinSetIdx][IDX_PS_CMRST],pinSet_subf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_subf[pinSetIdx][IDX_PS_CMRST],pinSet_subf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}	
            			
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_OV5648_MIPI_RAW   8820 8830 power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_subf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_subf[pinSetIdx][IDX_PS_CMRST],pinSet_subf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],pinSet_subf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],pinSet_subf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_subf[pinSetIdx][IDX_PS_CMRST],pinSet_subf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_subf[pinSetIdx][IDX_PS_CMPDN],pinSet_subf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
#endif
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV5647MIPI_RAW,currSensorName))) // ||(0 == strcmp(SENSOR_DRVNAME_OV5650MIPI_RAW,currSensorName))))  //zhaoshaopeng for ov5647 20120524

{
    //power ON
    if (On) {

        #if 0
    
               mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
               mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
               mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ONE);   

               mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
               mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
               mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);    
        #endif
/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
        
      
    	 mdelay(50);     
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(5); 	      
	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 	      
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))//zhaoshaopeng ov5647 1.5
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(50);

        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 
  
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_OV5647MIPI_RAW power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
//else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV5647AMIPI_RAW,currSensorName)))  //zhaoshaopeng for ov5647a 20120831
//{
//    //power ON
//    if (On) {
//        //in case
//	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_OV5647AMIPI_RAW power on  \r\n");			
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
//#endif

///*
//	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
//	Power Configuration:
//		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
//		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
//*/
//  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
//		//1.

//           //disable inactive sensor
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//        }
//		
//       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//   	     //reset pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//            mdelay(1);

//            //PDN pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//        }
//	//zhaoshaopeng vdd_io	
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//	 mdelay(5); 	      
//	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//	 mdelay(10); 	      
//        //zhaoshaopeng vcore
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))//zhaoshaopeng ov5647 1.5
//        {
//             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//             //return -EIO;
//             goto _kdCISModulePowerOn_exit_;
//        }    		
//        mdelay(50);
//  
//        //zhaoshaopeng af
//        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        } 

//        // wait power to be stable 
//        mdelay(5); 
//  
//        //enable active sensor
//        //RST pin
//        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
//            //PDN pin
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
//	     mdelay(5); 		
//            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
//	     mdelay(10); 
//            //if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
//        }
//	 mdelay(20); 
//    }
//    else {//power OFF
//        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_OV5647MIPI_RAW 5650 power off \r\n");	
//#if 0 //TODO: depends on HW layout. Should be notified by SA.
//        printk("Set GPIO 94 for power OFF\n"); 
//        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
//        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
//#endif

//        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
//        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
//            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
//            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
//    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
//            mdelay(1);
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
//            mdelay(1);     
//            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
//        }
//       mdelay(5); 
//    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }     	
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
//            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }
//        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
//        {
//            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
//            //return -EIO;
//            goto _kdCISModulePowerOn_exit_;
//        }                    
//    }//

//}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_HI253_YUV,currSensorName)))  //zhaoshaopeng for ov5647a 20120831
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_HI253_YUV power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(5); 	      
	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 	      
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng ov5647 1.5
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(50);
  
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 
  
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_HI253_YUV  power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K5CAGX_YUV,currSensorName)))  //zhaoshaopeng for ov5647a 20120831
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K5CAGX_YUV power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(5); 	      
	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 	      
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng ov5647 1.5
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(50);
  
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 
  
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K5CAGX_YUV power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }        
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_HI704_YUV,currSensorName)))  //zhaoshaopeng for ov5647a 20120831
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_HI704_YUV power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(5); 	      
	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 	      
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng ov5647 1.5
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(50);
  
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 
  
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_HI704_YUV  power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }    
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
else if((currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC2035_YUV,currSensorName)))&& (pinSetIdx ==1))
//else if((currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC2035_YUV,currSensorName))) )  //zhaoshaopeng for ov5647a 20120831
{
//	if(pinSetIdx !=1)
//		goto _kdCISModulePowerOn_exit_;
	
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_GC2035_YUV power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(5); 	      
	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 	      
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng ov5647 1.5
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(50);
  
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 
  
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_GC2035_YUV  power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC2035MIPI_YUV,currSensorName))) 
{
//	if(pinSetIdx !=0)
//		goto _kdCISModulePowerOn_exit_;
	
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_GC2035MIPI_YUV power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
  	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(5); 
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1500,mode_name))//zhaoshaopeng from fae
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(5);
       
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(50);
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 

		      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_GC2035MIPI_YUV power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }       
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  
    }//

}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_S5K3H2YX_MIPI_RAW,currSensorName)))  //zhaoshaopeng for ov5647a 20120831
{
	//power ON
	if (On) {
		//in case
	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K3H2YX_MIPI_RAW power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
		printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
		if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
		if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
		if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
		if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //	  if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

		   //disable inactive sensor
		if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
			if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
			if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
		}
		
	   if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
		 //reset pin
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
			mdelay(1);

			//PDN pin
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		}
	//zhaoshaopeng vdd_io	
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
		{
			PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
			//return -EIO;
			goto _kdCISModulePowerOn_exit_;
		}
	 mdelay(5); 		  
	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
		{
			PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
			//return -EIO;
			goto _kdCISModulePowerOn_exit_;
		}
	 mdelay(10);		  
		//zhaoshaopeng vcore
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng ov5647 1.5
		{
			 PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
			 //return -EIO;
			 goto _kdCISModulePowerOn_exit_;
		}			
		mdelay(50);

		//zhaoshaopeng af
		if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
		{
			PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
			//return -EIO;
			goto _kdCISModulePowerOn_exit_;
		} 

		// wait power to be stable 
		mdelay(5); 
  
		//enable active sensor
		//RST pin
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
			//PDN pin
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		 	mdelay(5); 		
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
		 	mdelay(10); 
			//if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} 	 
		}
	 mdelay(20); 
	}
	else {//power OFF
		printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_S5K3H2YX_MIPI_RAW power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
		printk("Set GPIO 94 for power OFF\n"); 
		if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
		if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
		if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
		if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}			
#endif

		//PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			 if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
			mdelay(1);
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
			mdelay(1);	   
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module 		
		}
	   mdelay(5); 
		if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
			PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
			//return -EIO;
			goto _kdCISModulePowerOn_exit_;
		}
		if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
		{
			PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
			//return -EIO;
			goto _kdCISModulePowerOn_exit_;
		}		
		if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
			PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
			//return -EIO;
			goto _kdCISModulePowerOn_exit_;
		}
		if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
		{
			PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
			//return -EIO;
			goto _kdCISModulePowerOn_exit_;
		}	
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
	}//

}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_GC0329_YUV,currSensorName)))  //zhaoshaopeng for ov5647a 20120831
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_GC0329_YUV power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(5); 	      
	 if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(10); 	      
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))//zhaoshaopeng ov5647 1.5
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(50);
  
        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 
  
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add SENSOR_DRVNAME_GC0329_YUV  power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(1);     
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }  
#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

}
else if(currSensorName && (0 == strcmp(SENSOR_DRVNAME_SOC5140_MIPI_YUV,currSensorName)))
	{
			//power ON
			if (On)
			{
				
           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMRST],pinSet[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet[1-pinSetIdx][IDX_PS_CMPDN],pinSet[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }
				mdelay(10);
				if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))
				{
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					//return -EIO;
					goto _kdCISModulePowerOn_exit_;
				}
				udelay(100);
				if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
				{
						PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
						//return -EIO;
						goto _kdCISModulePowerOn_exit_;
				}	

				udelay(100);		  
				if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
				{
					PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
					//return -EIO;
					goto _kdCISModulePowerOn_exit_;
				}
				udelay(100);		  
				if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
				{
					PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
					//return -EIO;
					goto _kdCISModulePowerOn_exit_;
				}

			//enable active sensor
		//RST pin
		if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
			//PDN pin
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
		 	mdelay(5); 		
			if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
			if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
			if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
		 	mdelay(10); 
			//if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} 	 
		}
	 mdelay(20); 
			}
			else 
			{
				printk("SENSOR_DRVNAME_SOC5140_YUV  Power Down\r\n");
				//PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
				if (GPIO_CAMERA_INVALID != pinSet[pinSetIdx][IDX_PS_CMRST]) {
					if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
					if(mt_set_gpio_mode(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
					if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
					if(mt_set_gpio_dir(pinSet[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
					 if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
					mdelay(1);
					if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMRST],pinSet[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
					mdelay(1);	   
					if(mt_set_gpio_out(pinSet[pinSetIdx][IDX_PS_CMPDN],pinSet[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module 		
				}

				if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
				{
					PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
					//return -EIO;
					goto _kdCISModulePowerOn_exit_;
				}	

				if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D,mode_name)) {
					PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
					//return -EIO;
					goto _kdCISModulePowerOn_exit_;
				}
				if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name))
				{
						PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
						//return -EIO;
						goto _kdCISModulePowerOn_exit_;
				}	

				if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
				{
					PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
					//return -EIO;
					goto _kdCISModulePowerOn_exit_;
				}  
	
				mdelay(20);
				mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
				mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
				mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);   
				
				mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
				mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
				mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);    

			}
		}

else//other sensor
{
    //power ON
    if (On) {
        //in case
	printk("\r\n zhaoshaopeng add others power on  \r\n");			
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set CAMERA_POWER_PULL_PIN for power \n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[[CAMERA SENSOR] Set CAMERA_POWER_PULL_PIN DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ONE)){PK_DBG("[[CAMERA SENSOR] set CAMERA_POWER_PULL_PIN failed!! \n");}
#endif

/*
	For Lenovo75 Project: OV56487_Raw(Main) & MT9V114_YUV(Sub)
	Power Configuration:
		OV5647: VCAM_D:1.5V,VCAM_D2:1.8V,VCAM_A1:2.8V, VCAM_A2:2.8V For AF use
		MT9V114:VCAM_D2:1.8V,VCAM_A2:2.8V
*/
  //      if (currSensorName && (0 == strcmp(SENSOR_DRVNAME_OV3640_YUV,currSensorName)))
		//1.

           //disable inactive sensor
        if (GPIO_CAMERA_INVALID != pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST],pinSet_mainf[1-pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            if(mt_set_gpio_out(pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[1-pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
        }
		
       if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
   	     //reset pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
            mdelay(1);

            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
        }

  	//zhaoshaopeng vdd_io	
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D2, VOL_1800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
	 mdelay(1);
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        mdelay(1);	
        //zhaoshaopeng vcore
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_D, VOL_1800,mode_name))
        {
             PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
             //return -EIO;
             goto _kdCISModulePowerOn_exit_;
        }    		
        mdelay(5);
       

        //zhaoshaopeng af
        if(TRUE != hwPowerOn(CAMERA_POWER_VCAM_A2, VOL_2800,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        } 

        // wait power to be stable 
        mdelay(5); 

		      
        //enable active sensor
        //RST pin
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            //PDN pin
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}
	     mdelay(5); 		
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_ON])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}
	     mdelay(10); 
            //if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_ON])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");}		 
        }
	 mdelay(20); 
    }
    else {//power OFF
        printk("\r\n zhaoshaopeng add others power off \r\n");	
#if 0 //TODO: depends on HW layout. Should be notified by SA.
        printk("Set GPIO 94 for power OFF\n"); 
        if (mt_set_gpio_pull_enable(GPIO_CAMERA_LDO_EN_PIN, GPIO_PULL_DISABLE)) {PK_DBG("[CAMERA SENSOR] Set GPIO94 PULL DISABLE ! \n"); }
        if(mt_set_gpio_mode(GPIO_CAMERA_LDO_EN_PIN, GPIO_CAMERA_LDO_EN_PIN_M_GPIO)){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
        if(mt_set_gpio_dir(GPIO_CAMERA_LDO_EN_PIN,GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
        if(mt_set_gpio_out(GPIO_CAMERA_LDO_EN_PIN,GPIO_OUT_ZERO)){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");}    	    
#endif

        //PK_DBG("[OFF]sensorIdx:%d \n",SensorIdx);
        if (GPIO_CAMERA_INVALID != pinSet_mainf[pinSetIdx][IDX_PS_CMRST]) {
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_MODE])){PK_DBG("[CAMERA SENSOR] set gpio mode failed!! \n");}
            if(mt_set_gpio_mode(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_MODE])){PK_DBG("[CAMERA LENS] set gpio mode failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],GPIO_DIR_OUT)){PK_DBG("[CAMERA SENSOR] set gpio dir failed!! \n");}
            if(mt_set_gpio_dir(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],GPIO_DIR_OUT)){PK_DBG("[CAMERA LENS] set gpio dir failed!! \n");}
    	     if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module
            mdelay(1);
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMRST],pinSet_mainf[pinSetIdx][IDX_PS_CMRST+IDX_PS_OFF])){PK_DBG("[CAMERA SENSOR] set gpio failed!! \n");} //low == reset sensor
            mdelay(24);     
            if(mt_set_gpio_out(pinSet_mainf[pinSetIdx][IDX_PS_CMPDN],pinSet_mainf[pinSetIdx][IDX_PS_CMPDN+IDX_PS_OFF])){PK_DBG("[CAMERA LENS] set gpio failed!! \n");} //high == power down lens module			
        }
       mdelay(5); 
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D, mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }	   
    	if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A,mode_name)) {
            PK_DBG("[CAMERA SENSOR] Fail to OFF analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }
        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_A2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable analog power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }     	

        if(TRUE != hwPowerDown(CAMERA_POWER_VCAM_D2,mode_name))
        {
            PK_DBG("[CAMERA SENSOR] Fail to enable digital power\n");
            //return -EIO;
            goto _kdCISModulePowerOn_exit_;
        }  

#if 0  
	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMPDN], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMPDN],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMPDN],GPIO_OUT_ZERO);	 

	   mt_set_gpio_mode(pinSet[1][IDX_PS_CMRST], GPIO_MODE_00);  
	   mt_set_gpio_dir(pinSet[1][IDX_PS_CMRST],GPIO_DIR_OUT);
	   mt_set_gpio_out(pinSet[1][IDX_PS_CMRST],GPIO_OUT_ZERO);	  
 #endif  		
    }//

		}
	}
	else
	{
		CAMERADB("currSensorName == NULL \n");
		goto _kdCISModulePowerOn_exit_;
	}
	return 0;

_kdCISModulePowerOn_exit_:
    return -EIO;
}

EXPORT_SYMBOL(kdCISModulePowerOn);


//!--
//




