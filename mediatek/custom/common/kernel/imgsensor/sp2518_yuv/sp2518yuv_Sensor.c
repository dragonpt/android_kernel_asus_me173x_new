/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   sensor.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Source code of Sensor driver
 *
 *
 * Author:
 * -------
 *   PC Huang (MTK02204)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 * 10 13 2010 sean.cheng
 * [ALPS00021684] [Need Patch] [Volunteer Patch] CCT new feature
 * .
 *
 * 09 10 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .10y dual sensor
 *
 * 09 02 2010 jackie.su
 * [ALPS00002279] [Need Patch] [Volunteer Patch] ALPS.Wxx.xx Volunteer patch for
 * .roll back dual sensor
 *
 * 07 27 2010 sean.cheng
 * [ALPS00003112] [Need Patch] [Volunteer Patch] ISP/Sensor driver modification for Customer support
 * .1. add master clock switcher 
 *  2. add master enable/disable 
 *  3. add dummy line/pixel for sensor 
 *  4. add sensor driving current setting
 *
 * 07 01 2010 sean.cheng
 * [ALPS00121215][Camera] Change color when switch low and high 
 * .Add video delay frame.
 *
 * 07 01 2010 sean.cheng
 * [ALPS00002805][Need Patch] [Volunteer Patch]10X Patch for DS269 Video Frame Rate 
 * .Change the sensor clock to let frame rate to be 30fps in vidoe mode
 *
 * 06 13 2010 sean.cheng
 * [ALPS00002514][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for E1k Camera 
 * .
 * 1. Add set zoom factor and capdelay frame for YUV sensor 
 * 2. Modify e1k sensor setting
 *
 * 05 25 2010 sean.cheng
 * [ALPS00002250][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for YUV video frame rate 
 * .
 * Add 15fps option for video mode
 *
 * 05 03 2010 sean.cheng
 * [ALPS00001357][Meta]CameraTool 
 * .
 * Fix SP2518 YUV sensor frame rate to 30fps in vidoe mode
 *
 * Mar 4 2010 mtk70508
 * [DUMA00154792] Sensor driver
 * 
 *
 * Mar 4 2010 mtk70508
 * [DUMA00154792] Sensor driver
 * 
 *
 * Mar 1 2010 mtk01118
 * [DUMA00025869] [Camera][YUV I/F & Query feature] check in camera code
 * 
 *
 * Feb 24 2010 mtk01118
 * [DUMA00025869] [Camera][YUV I/F & Query feature] check in camera code
 * 
 *
 * Nov 24 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Oct 29 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Oct 27 2009 mtk02204
 * [DUMA00015869] [Camera Driver] Modifiy camera related drivers for dual/backup sensor/lens drivers.
 * 
 *
 * Aug 13 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Aug 5 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Jul 17 2009 mtk01051
 * [DUMA00009217] [Camera Driver] CCAP First Check In
 * 
 *
 * Jul 7 2009 mtk01051
 * [DUMA00008051] [Camera Driver] Add drivers for camera high ISO binning mode.
 * Add ISO query info for Sensor
 *
 * May 18 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * May 16 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * May 16 2009 mtk01051
 * [DUMA00005921] [Camera] LED Flashlight first check in
 * 
 *
 * Apr 7 2009 mtk02204
 * [DUMA00004012] [Camera] Restructure and rename camera related custom folders and folder name of came
 * 
 *
 * Mar 27 2009 mtk02204
 * [DUMA00002977] [CCT] First check in of MT6516 CCT drivers
 *
 *
 * Mar 25 2009 mtk02204
 * [DUMA00111570] [Camera] The system crash after some operations
 *
 *
 * Mar 20 2009 mtk02204
 * [DUMA00002977] [CCT] First check in of MT6516 CCT drivers
 *
 *
 * Mar 2 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Feb 24 2009 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Dec 27 2008 MTK01813
 * DUMA_MBJ CheckIn Files
 * created by clearfsimport
 *
 * Dec 10 2008 mtk02204
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 *
 *
 * Oct 27 2008 mtk01051
 * [DUMA00000851] Camera related drivers check in
 * Modify Copyright Header
 *
 * Oct 24 2008 mtk02204
 * [DUMA00000851] Camera related drivers check in
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <asm/io.h>
#include <asm/system.h>
	 
#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"
	 
#include "sp2518yuv_Sensor.h"
#include "sp2518yuv_Camera_Sensor_para.h"
#include "sp2518yuv_CameraCustomized.h" 


#define SP2518YUV_DEBUG
#ifdef SP2518YUV_DEBUG
#define SP2518SENSORDB printk
#else
#define SP2518SENSORDB(x,...)
#endif


//AE target

#define  SP2518_P0_0xf7  0x80//78

#define  SP2518_P0_0xf8  0x74//6e

#define  SP2518_P0_0xf9  0x80//74

#define  SP2518_P0_0xfa  0x74//6a

//HEQ

#define  SP2518_P0_0xdd  0x80

#define  SP2518_P0_0xde  0x95//90

//sharpen

#define  SP2518_P1_0xe9  0x20//0x30//14

#define  SP2518_P1_0xed  0x20//0x30//10

//sat

#define  SP2518_P0_0xd4  0x77

#define  SP2518_P0_0xd9  0x77

//auto lum

#define SP2518_NORMAL_Y0ffset  0x10

#define SP2518_LOWLIGHT_Y0ffset  0x20



#define CLK24M_48M

// #define CLK24M_72M


static DEFINE_SPINLOCK(sp2518_drv_lock);

static MSDK_SCENARIO_ID_ENUM CurrentScenarioId = MSDK_SCENARIO_ID_CAMERA_PREVIEW;

#if 0
extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId);
extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);

#define SP2518_write_cmos_sensor(addr, para) iWriteReg((u16) addr , (u32) para ,1,SP2518_WRITE_ID)
kal_uint16 SP2518_read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte=0;
    if(iReadReg((u16) addr ,(u8*)&get_byte,SP2518_WRITE_ID)<0)
    printk("sp2518 i2c read error");
    
    return get_byte;
}
#else
extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);
kal_uint16 SP2518_write_cmos_sensor(kal_uint8 addr, kal_uint8 para)
{
    char puSendCmd[2] = {(char)(addr & 0xFF) , (char)(para & 0xFF)};
	
	iWriteRegI2C(puSendCmd , 2, SP2518_WRITE_ID);

}
kal_uint16 SP2518_read_cmos_sensor(kal_uint8 addr)
{
	kal_uint16 get_byte=0;
    char puSendCmd = { (char)(addr & 0xFF) };
	iReadRegI2C(&puSendCmd , 1, (u8*)&get_byte, 1, SP2518_WRITE_ID);
	
    return get_byte;
}
#endif
#define mDELAY(ms)  mdelay(ms)

static struct
{
	kal_uint8   Banding;
	kal_bool	NightMode;
	kal_bool	VideoMode;
	kal_uint16  Fps;
	kal_uint16  ShutterStep;
	kal_uint8   IsPVmode;
	kal_uint32  PreviewDummyPixels;
	kal_uint32  PreviewDummyLines;
	kal_uint32  CaptureDummyPixels;
	kal_uint32  CaptureDummyLines;
	kal_uint32  PreviewPclk;
	kal_uint32  CapturePclk;
	kal_uint32  PreviewShutter;
	kal_uint32  PreviewExtraShutter;
	kal_uint32  SensorGain;
	SP2518_SENSOR_MODE SensorMode;
} SP2518Sensor;

/* Global Valuable */

static kal_uint32 zoom_factor = 0; 

static kal_int8 SP2518_DELAY_AFTER_PREVIEW = -1;

static kal_uint8 SP2518_Banding_setting = AE_FLICKER_MODE_50HZ;  //Wonder add

static kal_bool SP2518_AWB_ENABLE = KAL_TRUE; 
static kal_bool SP2518_AE_ENABLE = KAL_TRUE; 

//SENSOR_REG_STRUCT SP2518SensorCCT[FACTORY_END_ADDR]=CAMERA_SENSOR_CCT_DEFAULT_VALUE;
//SENSOR_REG_STRUCT SP2518SensorReg[ENGINEER_END]=CAMERA_SENSOR_REG_DEFAULT_VALUE;
//	camera_para.SENSOR.cct	SensorCCT	=> SensorCCT
//	camera_para.SENSOR.reg	SensorReg
MSDK_SENSOR_CONFIG_STRUCT SP2518SensorConfigData;


/*************************************************************************
* FUNCTION
*	SP2518_set_dummy
*
* DESCRIPTION
*	This function set the dummy pixels(Horizontal Blanking) & dummy lines(Vertical Blanking), it can be
*	used to adjust the frame rate or gain more time for back-end process.
*	
*	IMPORTANT NOTICE: the base shutter need re-calculate for some sensor, or else flicker may occur.
*
* PARAMETERS
*	1. kal_uint32 : Dummy Pixels (Horizontal Blanking)
*	2. kal_uint32 : Dummy Lines (Vertical Blanking)
*
* RETURNS
*	None
*
*************************************************************************/
static void SP2518SetDummy(kal_uint32 dummy_pixels, kal_uint32 dummy_lines)
{
#if 0
        if (SP2518Sensor.IsPVmode)  
        {
            dummy_pixels = dummy_pixels+0x0514; 
            SP2518_write_cmos_sensor(0x380D,( dummy_pixels&0xFF));         
            SP2518_write_cmos_sensor(0x380C,(( dummy_pixels&0xFF00)>>8)); 
      
            dummy_lines= dummy_lines+0x0268; 
            SP2518_write_cmos_sensor(0x380F,(dummy_lines&0xFF));       
            SP2518_write_cmos_sensor(0x380E,((dummy_lines&0xFF00)>>8));  
        } 
        else
        {
            dummy_pixels = dummy_pixels+0x079f; 
            SP2518_write_cmos_sensor(0x380D,( dummy_pixels&0xFF));         
            SP2518_write_cmos_sensor(0x380C,(( dummy_pixels&0xFF00)>>8)); 
      
            dummy_lines= dummy_lines*2+0x04d0; 
            SP2518_write_cmos_sensor(0x380F,(dummy_lines&0xFF));       
            SP2518_write_cmos_sensor(0x380E,((dummy_lines&0xFF00)>>8));  
        } 

	#endif
}    /* SP2518_set_dummy */

/*************************************************************************
* FUNCTION
*	SP2518WriteShutter
*
* DESCRIPTION
*	This function used to write the shutter.
*
* PARAMETERS
*	1. kal_uint32 : The shutter want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/
static void SP2518WriteShutter(kal_uint32 shutter)
{
	kal_uint32 extra_exposure_lines = 0;
	#if 0
	if (shutter < 1)
	{
		shutter = 1;
	}
	
	if (SP2518Sensor.IsPVmode) 
	{
		if (shutter <= SP2518_PV_EXPOSURE_LIMITATION) 
		{
			extra_exposure_lines = 0;
		}
		else 
		{
			extra_exposure_lines=shutter - SP2518_PV_EXPOSURE_LIMITATION;
		}
		
	}
	else 
	{
		if (shutter <= SP2518_FULL_EXPOSURE_LIMITATION) 
		{
			extra_exposure_lines = 0;
		}
		else 
		{
			extra_exposure_lines = shutter - SP2518_FULL_EXPOSURE_LIMITATION;
		}
		
	}
	
	//AEC PK EXPOSURE
	shutter*=16;
	SP2518_write_cmos_sensor(0x3502, (shutter & 0x00FF));           //AEC[7:0]
	SP2518_write_cmos_sensor(0x3501, ((shutter & 0x0FF00) >>8));  //AEC[15:8]
	SP2518_write_cmos_sensor(0x3500, ((shutter & 0xFF0000) >> 16));	
	
	if(extra_exposure_lines>0)
	{
		// set extra exposure line [aec add vts]
		SP2518_write_cmos_sensor(0x3507, extra_exposure_lines & 0xFF);          // EXVTS[b7~b0]
		SP2518_write_cmos_sensor(0x3506, (extra_exposure_lines & 0xFF00) >> 8); // EXVTS[b15~b8]
	}
	else
	{
		// set extra exposure line [aec add vts]
		SP2518_write_cmos_sensor(0x3507, 0x00);          // EXVTS[b7~b0]
		SP2518_write_cmos_sensor(0x3506, 0x00); // EXVTS[b15~b8]
	}
	#endif
	
}    /* SP2518_write_shutter */

/*************************************************************************
* FUNCTION
*	SP2518WriteWensorGain
*
* DESCRIPTION
*	This function used to write the sensor gain.
*
* PARAMETERS
*	1. kal_uint32 : The sensor gain want to apply to sensor.
*
* RETURNS
*	None
*
*************************************************************************/
static void SP2518WriteSensorGain(kal_uint32 gain)
{
	kal_uint16 temp_reg = 0;
	#if 0
		
	if(gain > 1024)  ASSERT(0);
	temp_reg = 0;
	
	temp_reg=gain&0x0FF;	
	SP2518_write_cmos_sensor(0x350B,temp_reg);
	#endif
}  /* SP2518_write_sensor_gain */

/*************************************************************************
* FUNCTION
*	SP2518ReadShutter
*
* DESCRIPTION
*	This function read current shutter for calculate the exposure.
*
* PARAMETERS
*	None
*
* RETURNS
*	kal_uint16 : The current shutter value.
*
*************************************************************************/
static kal_uint32 SP2518ReadShutter(void)
{
	kal_uint16 temp_reg1, temp_reg2 ,temp_reg3;
	#if 0
	temp_reg1 = SP2518_read_cmos_sensor(0x3500);    // AEC[b19~b16]
	temp_reg2 = SP2518_read_cmos_sensor(0x3501);    // AEC[b15~b8]
	temp_reg3 = SP2518_read_cmos_sensor(0x3502);    // AEC[b7~b0]
	//read out register value and divide 16;
	spin_lock(&sp2518_drv_lock);
	SP2518Sensor.PreviewShutter  = (temp_reg1 <<12)| (temp_reg2<<4)|(temp_reg3>>4);
	spin_unlock(&sp2518_drv_lock);
	
	temp_reg1 = SP2518_read_cmos_sensor(0x3506);    // EXVTS[b15~b8]
	temp_reg2 = SP2518_read_cmos_sensor(0x3507);    // EXVTS[b7~b0]
	/* Backup the preview mode last shutter & sensor gain. */
	spin_lock(&sp2518_drv_lock);
	SP2518Sensor.PreviewExtraShutter = (temp_reg2 & 0xFF) | (temp_reg1 << 8);
	spin_unlock(&sp2518_drv_lock);
	#endif
	return SP2518Sensor.PreviewShutter;
}    /* SP2518_read_shutter */

/*************************************************************************
* FUNCTION
*	SP2518ReadSensorGain
*
* DESCRIPTION
*	This function read current sensor gain for calculate the exposure.
*
* PARAMETERS
*	None
*
* RETURNS
*	kal_uint16 : The current sensor gain value.
*
*************************************************************************/
static kal_uint32 SP2518ReadSensorGain(void)
{
	kal_uint16 temp_reg = 0;
	kal_uint32 sensor_gain = 0;
	//sensor_gain=(SP2518_read_cmos_sensor(0x350B)&0xFF);//+((SP2518_read_cmos_sensor(0x350A)&0xFF<<8)&0xFF00);  
		
	return sensor_gain;

}  /* SP2518ReadSensorGain */


static void SP2518_set_AE_mode(kal_bool AE_enable)
{
    kal_uint8 AeTemp;
	#if 0
	AeTemp = SP2518_read_cmos_sensor(0x3503);

    if (AE_enable == KAL_TRUE)
    {
        // turn on AEC/AGC
        SP2518_write_cmos_sensor(0x3503, (AeTemp&(~0x07)));
    }
    else
    {
        // turn off AEC/AGC
      SP2518_write_cmos_sensor(0x3503, (AeTemp| 0x07));
    }
	#endif
}


static void SP2518_set_AWB_mode(kal_bool AWB_enable)
{
    kal_uint8 AwbTemp;
	#if 0
	AwbTemp = SP2518_read_cmos_sensor(0x3406);

    //return ;

    if (AWB_enable == KAL_TRUE)
    {
        //enable Auto WB
      
		SP2518_write_cmos_sensor(0x3406 , AwbTemp&0xFE ); 
		
    }
    else
    {
        //turn off AWB
     
		SP2518_write_cmos_sensor(0x3406 , AwbTemp|0x01 ); 
    }
	#endif
}


/*************************************************************************
* FUNCTION
*	SP2518_night_mode
*
* DESCRIPTION
*	This function night mode of SP2518.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void SP2518_night_mode(kal_bool enable)
{
	//kal_uint16 night = SP2518_read_cmos_sensor(0x3A00); 
	if (enable)
	{ 
		SP2518Sensor.NightMode = KAL_TRUE;  
		SP2518_write_cmos_sensor(0xfd,0x00);
		SP2518_write_cmos_sensor(0xb2,SP2518_LOWLIGHT_Y0ffset);
		if(SP2518_Banding_setting == AE_FLICKER_MODE_50HZ)
		{
			#ifdef CLK24M_48M

			//capture preview night 48M 50hz fix 6FPS maxgain 

			SP2518_write_cmos_sensor(0xfd,0x00);

			SP2518_write_cmos_sensor(0x03,0x01);

			SP2518_write_cmos_sensor(0x04,0xd4);

			SP2518_write_cmos_sensor(0x09,0x02);

			SP2518_write_cmos_sensor(0x0a,0x64); 

			SP2518_write_cmos_sensor(0xf0,0x4e);

			SP2518_write_cmos_sensor(0xf1,0x00);

			SP2518_write_cmos_sensor(0xfd,0x01);	

			SP2518_write_cmos_sensor(0x90,0x10);	

			SP2518_write_cmos_sensor(0x92,0x01);

			SP2518_write_cmos_sensor(0x98,0x4e);

			SP2518_write_cmos_sensor(0x99,0x00);

			SP2518_write_cmos_sensor(0x9a,0x01);

			SP2518_write_cmos_sensor(0x9b,0x00);

			                  

			// status               

			SP2518_write_cmos_sensor(0xfd,0x01);

			SP2518_write_cmos_sensor(0xce,0xe0);

			SP2518_write_cmos_sensor(0xcf,0x04);

			SP2518_write_cmos_sensor(0xd0,0xe0);

			SP2518_write_cmos_sensor(0xd1,0x04);

			SP2518_write_cmos_sensor(0xd7,0x50);//exp_nr_outd_8lsb

			SP2518_write_cmos_sensor(0xd8,0x00);

			SP2518_write_cmos_sensor(0xd9,0x54);//exp_outd_nr_8lsb

			SP2518_write_cmos_sensor(0xda,0x00);

			SP2518_write_cmos_sensor(0xfd,0x00);	

			#elif  defined(CLK24M_72M)

			//capture preview night 72M 50hz fix 6FPS maxgain                                      

			SP2518_write_cmos_sensor(0xfd , 0x00);

			SP2518_write_cmos_sensor(0x03 , 0x01);

			SP2518_write_cmos_sensor(0x04 , 0xd4);

			SP2518_write_cmos_sensor(0x05 , 0x00);

			SP2518_write_cmos_sensor(0x06 , 0x00);

			SP2518_write_cmos_sensor(0x07 , 0x00);

			SP2518_write_cmos_sensor(0x08 , 0x00);

			SP2518_write_cmos_sensor(0x09 , 0x05);

			SP2518_write_cmos_sensor(0x0a , 0x66);

			SP2518_write_cmos_sensor(0xf0 , 0x4e);

			SP2518_write_cmos_sensor(0xf1 , 0x00);

			SP2518_write_cmos_sensor(0xfd , 0x01);

			SP2518_write_cmos_sensor(0x90 , 0x10);

			SP2518_write_cmos_sensor(0x92 , 0x01);

			SP2518_write_cmos_sensor(0x98 , 0x4e);

			SP2518_write_cmos_sensor(0x99 , 0x00);

			SP2518_write_cmos_sensor(0x9a , 0x01);

			SP2518_write_cmos_sensor(0x9b , 0x00);

			//Status                              

			SP2518_write_cmos_sensor(0xfd , 0x01);

			SP2518_write_cmos_sensor(0xce , 0xe0);

			SP2518_write_cmos_sensor(0xcf , 0x04);

			SP2518_write_cmos_sensor(0xd0 , 0xe0);

			SP2518_write_cmos_sensor(0xd1 , 0x04);

			SP2518_write_cmos_sensor(0xd7 , 0x50);

			SP2518_write_cmos_sensor(0xd8 , 0x00);

			SP2518_write_cmos_sensor(0xd9 , 0x54);

			SP2518_write_cmos_sensor(0xda , 0x00);

			SP2518_write_cmos_sensor(0xfd , 0x00);

			#endif
		}else
		{
			#ifdef CLK24M_48M

			//capture preview night 48M 60hz fix 6FPS maxgain

			SP2518_write_cmos_sensor(0xfd,0x00);

			SP2518_write_cmos_sensor(0x03,0x01);

			SP2518_write_cmos_sensor(0x04,0x86);

			SP2518_write_cmos_sensor(0x09,0x02);

			SP2518_write_cmos_sensor(0x0a,0x64); 

			SP2518_write_cmos_sensor(0xf0,0x41);

			SP2518_write_cmos_sensor(0xf1,0x00);

			SP2518_write_cmos_sensor(0xfd,0x01);	

			SP2518_write_cmos_sensor(0x90,0x14);	

			SP2518_write_cmos_sensor(0x92,0x01);

			SP2518_write_cmos_sensor(0x98,0x41);

			SP2518_write_cmos_sensor(0x99,0x00);

			SP2518_write_cmos_sensor(0x9a,0x01);

			SP2518_write_cmos_sensor(0x9b,0x00);

			                  

			// status               

			SP2518_write_cmos_sensor(0xfd,0x01);

			SP2518_write_cmos_sensor(0xce,0x14);

			SP2518_write_cmos_sensor(0xcf,0x05);

			SP2518_write_cmos_sensor(0xd0,0x14);

			SP2518_write_cmos_sensor(0xd1,0x05);

			SP2518_write_cmos_sensor(0xd7,0x43);//exp_nr_outd_8lsb

			SP2518_write_cmos_sensor(0xd8,0x00);

			SP2518_write_cmos_sensor(0xd9,0x47);//exp_outd_nr_8lsb

			SP2518_write_cmos_sensor(0xda,0x00);

			SP2518_write_cmos_sensor(0xfd,0x00);

			#elif  defined(CLK24M_72M)

			//capture preview night 72M 60hz fix 6FPS maxgain                                                 

			SP2518_write_cmos_sensor(0xfd , 0x00);

			SP2518_write_cmos_sensor(0x03 , 0x01);

			SP2518_write_cmos_sensor(0x04 , 0x86);

			SP2518_write_cmos_sensor(0x05 , 0x00);

			SP2518_write_cmos_sensor(0x06 , 0x00);

			SP2518_write_cmos_sensor(0x07 , 0x00);

			SP2518_write_cmos_sensor(0x08 , 0x00);

			SP2518_write_cmos_sensor(0x09 , 0x05);

			SP2518_write_cmos_sensor(0x0a , 0x66);

			SP2518_write_cmos_sensor(0xf0 , 0x41);

			SP2518_write_cmos_sensor(0xf1 , 0x00);

			SP2518_write_cmos_sensor(0xfd , 0x01);

			SP2518_write_cmos_sensor(0x90 , 0x14);

			SP2518_write_cmos_sensor(0x92 , 0x01);

			SP2518_write_cmos_sensor(0x98 , 0x41);

			SP2518_write_cmos_sensor(0x99 , 0x00);

			SP2518_write_cmos_sensor(0x9a , 0x01);

			SP2518_write_cmos_sensor(0x9b , 0x00);

			//Status                              

			SP2518_write_cmos_sensor(0xfd , 0x01);

			SP2518_write_cmos_sensor(0xce , 0x14);

			SP2518_write_cmos_sensor(0xcf , 0x05);

			SP2518_write_cmos_sensor(0xd0 , 0x14);

			SP2518_write_cmos_sensor(0xd1 , 0x05);

			SP2518_write_cmos_sensor(0xd7 , 0x43);

			SP2518_write_cmos_sensor(0xd8 , 0x00);

			SP2518_write_cmos_sensor(0xd9 , 0x47);

			SP2518_write_cmos_sensor(0xda , 0x00);

			SP2518_write_cmos_sensor(0xfd , 0x00);

			#endif	
		}
	}
	else
	{    
		SP2518Sensor.NightMode = KAL_FALSE;
		SP2518_write_cmos_sensor(0xfd,0x00);
		SP2518_write_cmos_sensor(0xb2,SP2518_NORMAL_Y0ffset);
		if(SP2518_Banding_setting == AE_FLICKER_MODE_50HZ)
		{
			#ifdef CLK24M_48M

		  //capture preview daylight 48M 50hz fix 9FPS maxgain   

				SP2518_write_cmos_sensor(0xfd,0x00);

				SP2518_write_cmos_sensor(0x03,0x02);

				SP2518_write_cmos_sensor(0x04,0xbe);

				SP2518_write_cmos_sensor(0x09,0x00);

				SP2518_write_cmos_sensor(0x0a,0x64); 

				SP2518_write_cmos_sensor(0xf0,0x75);

				SP2518_write_cmos_sensor(0xf1,0x00);

				SP2518_write_cmos_sensor(0xfd,0x01);	

				SP2518_write_cmos_sensor(0x90,0x0b);	

				SP2518_write_cmos_sensor(0x92,0x01);

				SP2518_write_cmos_sensor(0x98,0x75);

				SP2518_write_cmos_sensor(0x99,0x00);

				SP2518_write_cmos_sensor(0x9a,0x01);

				SP2518_write_cmos_sensor(0x9b,0x00);

				                  

				// status               

				SP2518_write_cmos_sensor(0xfd,0x01);

				SP2518_write_cmos_sensor(0xce,0x07);

				SP2518_write_cmos_sensor(0xcf,0x05);

				SP2518_write_cmos_sensor(0xd0,0x07);

				SP2518_write_cmos_sensor(0xd1,0x05);

				SP2518_write_cmos_sensor(0xd7,0x77);//exp_nr_outd_8lsb

				SP2518_write_cmos_sensor(0xd8,0x00);

				SP2518_write_cmos_sensor(0xd9,0x7b);//exp_outd_nr_8lsb

				SP2518_write_cmos_sensor(0xda,0x00);

				SP2518_write_cmos_sensor(0xfd,0x00);	

               #elif  defined(CLK24M_72M)	

                   //capture preview daylight 72M 50hz 8-13FPS maxgain

				SP2518_write_cmos_sensor(0xfd , 0x00);

				SP2518_write_cmos_sensor(0x03 , 0xf6);

				SP2518_write_cmos_sensor(0x04 , 0xbe); 

				SP2518_write_cmos_sensor(0x09 , 0x00);

				SP2518_write_cmos_sensor(0x0a , 0x8b);  

				SP2518_write_cmos_sensor(0xf0 , 0xa9);

				SP2518_write_cmos_sensor(0xf1 , 0x00);

				SP2518_write_cmos_sensor(0xfd , 0x01);	

				SP2518_write_cmos_sensor(0x90 , 0x0c);	

				SP2518_write_cmos_sensor(0x92 , 0x01);

				SP2518_write_cmos_sensor(0x98 , 0xa9);

				SP2518_write_cmos_sensor(0x99 , 0x00);

				SP2518_write_cmos_sensor(0x9a , 0x01);

				SP2518_write_cmos_sensor(0x9b , 0x00);



				// status 

				SP2518_write_cmos_sensor(0xfd , 0x01);

				SP2518_write_cmos_sensor(0xce , 0xec);

				SP2518_write_cmos_sensor(0xcf , 0x07);

				SP2518_write_cmos_sensor(0xd0 , 0xec);

				SP2518_write_cmos_sensor(0xd1 , 0x07);

				SP2518_write_cmos_sensor(0xd7 , 0xab);//exp_nr_outd_8lsb

				SP2518_write_cmos_sensor(0xd8 , 0x00);

				SP2518_write_cmos_sensor(0xd9 , 0xaf);//exp_outd_nr_8lsb

				SP2518_write_cmos_sensor(0xda , 0x00);

				SP2518_write_cmos_sensor(0xfd , 0x00);

		#endif		
		}
		else
		{
			#ifdef CLK24M_48M

	  		 //capture preview daylight 48M 60Hz fix 9FPS maxgain   

				SP2518_write_cmos_sensor(0xfd,0x00);

			       SP2518_write_cmos_sensor(0x03,0x02);

				SP2518_write_cmos_sensor(0x04,0x4c); 

				SP2518_write_cmos_sensor(0x09,0x00);

				SP2518_write_cmos_sensor(0x0a,0x5e); 

				SP2518_write_cmos_sensor(0xf0,0x62);

				SP2518_write_cmos_sensor(0xf1,0x00);

				SP2518_write_cmos_sensor(0xfd,0x01);	

				SP2518_write_cmos_sensor(0x90,0x0d);	

				SP2518_write_cmos_sensor(0x92,0x01);

				SP2518_write_cmos_sensor(0x98,0x62);

				SP2518_write_cmos_sensor(0x99,0x00);

				SP2518_write_cmos_sensor(0x9a,0x01);

				SP2518_write_cmos_sensor(0x9b,0x00);          

				// status               

				SP2518_write_cmos_sensor(0xfd,0x01);

				SP2518_write_cmos_sensor(0xce,0xfa);

				SP2518_write_cmos_sensor(0xcf,0x04);

				SP2518_write_cmos_sensor(0xd0,0xfa);

				SP2518_write_cmos_sensor(0xd1,0x04);

				SP2518_write_cmos_sensor(0xd7,0x64);//exp_nr_outd_8lsb

				SP2518_write_cmos_sensor(0xd8,0x00);

				SP2518_write_cmos_sensor(0xd9,0x68);//exp_outd_nr_8lsb

				SP2518_write_cmos_sensor(0xda,0x00);

				SP2518_write_cmos_sensor(0xfd,0x00);

			#elif  defined(CLK24M_72M)  

	  //capture preview daylight 72M 60Hz 8-13FPS maxgain

				SP2518_write_cmos_sensor(0xfd , 0x00);

				SP2518_write_cmos_sensor(0x03 , 0x03);

				SP2518_write_cmos_sensor(0x04 , 0x4e);

				SP2518_write_cmos_sensor(0x05 , 0x00);

				SP2518_write_cmos_sensor(0x06 , 0x00);

				SP2518_write_cmos_sensor(0x07 , 0x00);

				SP2518_write_cmos_sensor(0x08 , 0x00);

				SP2518_write_cmos_sensor(0x09 , 0x00);

				SP2518_write_cmos_sensor(0x0a , 0x8a);

				SP2518_write_cmos_sensor(0xf0 , 0x8d);

				SP2518_write_cmos_sensor(0xf1 , 0x00);

				SP2518_write_cmos_sensor(0xfd , 0x01);

				SP2518_write_cmos_sensor(0x90 , 0x0f);

				SP2518_write_cmos_sensor(0x92 , 0x01);

				SP2518_write_cmos_sensor(0x98 , 0x8d);

				SP2518_write_cmos_sensor(0x99 , 0x00);

				SP2518_write_cmos_sensor(0x9a , 0x01);

				SP2518_write_cmos_sensor(0x9b , 0x00);

				//Status                     

				SP2518_write_cmos_sensor(0xfd , 0x01);

				SP2518_write_cmos_sensor(0xce , 0x43);

				SP2518_write_cmos_sensor(0xcf , 0x08);

				SP2518_write_cmos_sensor(0xd0 , 0x43);

				SP2518_write_cmos_sensor(0xd1 , 0x08);

				SP2518_write_cmos_sensor(0xd7 , 0x8f);

				SP2518_write_cmos_sensor(0xd8 , 0x00);

				SP2518_write_cmos_sensor(0xd9 , 0x90);

				SP2518_write_cmos_sensor(0xda , 0x00);

				SP2518_write_cmos_sensor(0xfd , 0x00);

			 #endif			
		}
	}
}	/* SP2518_night_mode */



/*************************************************************************
* FUNCTION
*	SP2518_GetSensorID
*
* DESCRIPTION
*	This function get the sensor ID
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
//extern int kdCISModulePowerOn(CAMERA_DUAL_CAMERA_SENSOR_ENUM SensorIdx, char *currSensorName, BOOL On, char* mode_name);
static kal_uint32 SP2518_GetSensorID(kal_uint32 *sensorID)
{
	volatile signed char i;
		kal_uint32 sensor_id=0;
		kal_uint8 temp_sccb_addr = 0;
		//s_move to here from CISModulePowerOn()
		//kdCISModulePowerOn(1,1,"sp2518yuv","kd_camera_hw");
		SP2518_write_cmos_sensor(0xfd,0x00);// Reset sensor
	    mDELAY(10);
		//  Read sensor ID to adjust I2C is OK?
		for(i=0;i<3;i++)
		{
			*sensorID = SP2518_read_cmos_sensor(0x02);//
			SP2518SENSORDB("RONLUS SP2518 READ ID: %x",*sensorID);
			if(*sensorID != SP2518_SENSOR_ID)
			{	
				*sensorID =0xffffffff;
				return ERROR_SENSOR_CONNECT_FAIL;
			}
		}
    return ERROR_NONE;    
}   


/*************************************************************************
* FUNCTION
*    SP2518InitialSetting
*
* DESCRIPTION
*    This function initialize the registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void SP2518InitialSetting(void)
{
	
    //SI50_SP2518 V1.0
	
	//24M in, 48M out max fix 9fps
	
  SP2518_write_cmos_sensor(0xfd , 0x00);
	
  SP2518_write_cmos_sensor(0x1b , 0x02);
	
 SP2518_write_cmos_sensor(0x0e , 0x01);
	
 SP2518_write_cmos_sensor(0x0f , 0x2f);
	
 SP2518_write_cmos_sensor(0x10 , 0x2e);
	
 SP2518_write_cmos_sensor(0x11 , 0x00);
	
 SP2518_write_cmos_sensor(0x12 , 0x2f);//2//4f f
	
 SP2518_write_cmos_sensor(0x14 , 0x40);//20
	
 SP2518_write_cmos_sensor(0x16 , 0x02);
	
 SP2518_write_cmos_sensor(0x17 , 0x10);
	
 SP2518_write_cmos_sensor(0x1a , 0x1f);//1e
	
 SP2518_write_cmos_sensor(0x1e , 0x81);
	
 SP2518_write_cmos_sensor(0x21 , 0x00); //0x08 晃动发红问题
	
 SP2518_write_cmos_sensor(0x22 , 0x1b);//19  //防止照灯有红线
	
 SP2518_write_cmos_sensor(0x25 , 0x10);
	
 SP2518_write_cmos_sensor(0x26 , 0x25);
	
 SP2518_write_cmos_sensor(0x27 , 0x6d);//0x6d
	
 SP2518_write_cmos_sensor(0x2c , 0x23);// 31 45
	
 SP2518_write_cmos_sensor(0x2d , 0x75);
	
 SP2518_write_cmos_sensor(0x2e , 0x38);//18 800*600行噪
	
 //SP2518_write_cmos_sensor(0x31 , 0x10);  //mirror upside down
	
 SP2518_write_cmos_sensor(0x44 , 0x03);
	
 SP2518_write_cmos_sensor(0x6f , 0x00);
	
 SP2518_write_cmos_sensor(0xa0 , 0x04);
	
 SP2518_write_cmos_sensor(0x5f , 0x01);//Ronlus red blue neg 0x01);
	
 SP2518_write_cmos_sensor(0x32 , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x2c , 0x00);
	
 SP2518_write_cmos_sensor(0x2d , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xfb , 0x83);
	
 SP2518_write_cmos_sensor(0xf4 , 0x09);
	
  //Pregain 
	
  SP2518_write_cmos_sensor(0xfd , 0x01);
	
  SP2518_write_cmos_sensor(0xc6 , 0x90);
	
  SP2518_write_cmos_sensor(0xc7 , 0x90); 
	
  SP2518_write_cmos_sensor(0xc8 , 0x90);
	
  SP2518_write_cmos_sensor(0xc9 , 0x90);
	
 //blacklevel
	SP2518_write_cmos_sensor(0xfd , 0x00);
	SP2518_write_cmos_sensor(0x65 , 0x08);
	SP2518_write_cmos_sensor(0x66 , 0x08); 
	SP2518_write_cmos_sensor(0x67 , 0x08);
	SP2518_write_cmos_sensor(0x68 , 0x08);
	//bpc
	SP2518_write_cmos_sensor(0x46 , 0xff);
	//rpc     
	
  SP2518_write_cmos_sensor(0xfd , 0x00);
	
  SP2518_write_cmos_sensor(0xe0 , 0x6c); 
	
  SP2518_write_cmos_sensor(0xe1 , 0x54); 
	
  SP2518_write_cmos_sensor(0xe2 , 0x48); 
	
  SP2518_write_cmos_sensor(0xe3 , 0x40);
	
  SP2518_write_cmos_sensor(0xe4 , 0x40);
	
  SP2518_write_cmos_sensor(0xe5 , 0x3e);
	
  SP2518_write_cmos_sensor(0xe6 , 0x3e);
	
  SP2518_write_cmos_sensor(0xe8 , 0x3a);
	
  SP2518_write_cmos_sensor(0xe9 , 0x3a);
	
  SP2518_write_cmos_sensor(0xea , 0x3a);
	
  SP2518_write_cmos_sensor(0xeb , 0x38);
	
  SP2518_write_cmos_sensor(0xf5 , 0x38);
	
  SP2518_write_cmos_sensor(0xf6 , 0x38);
	
  SP2518_write_cmos_sensor(0xfd , 0x01);
	
  SP2518_write_cmos_sensor(0x94 , 0xc0);//f8
	
  SP2518_write_cmos_sensor(0x95 , 0x38);
	
  SP2518_write_cmos_sensor(0x9c , 0x6c);
	
  SP2518_write_cmos_sensor(0x9d , 0x38);     
	
#ifdef CLK24M_48M
	
//AE  pll 48M  fix 9fps   
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0x30 , 0x04);//PLL 2倍频
	
 SP2518_write_cmos_sensor(0x03 , 0x02);
	
 SP2518_write_cmos_sensor(0x04 , 0xbe); 
	
 SP2518_write_cmos_sensor(0x09 , 0x00);
	
 SP2518_write_cmos_sensor(0x0a , 0x64);  
	
 SP2518_write_cmos_sensor(0xf0 , 0x75);
	
 SP2518_write_cmos_sensor(0xf1 , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x01);	
	
 SP2518_write_cmos_sensor(0x90 , 0x0b);	
	
 SP2518_write_cmos_sensor(0x92 , 0x01);
	
 SP2518_write_cmos_sensor(0x98 , 0x75);
	
 SP2518_write_cmos_sensor(0x99 , 0x00);
	
 SP2518_write_cmos_sensor(0x9a , 0x01);
	
 SP2518_write_cmos_sensor(0x9b , 0x00);
	

	
 // status 
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0xce , 0x07);
	
 SP2518_write_cmos_sensor(0xcf , 0x05);
	
 SP2518_write_cmos_sensor(0xd0 , 0x07);
	
 SP2518_write_cmos_sensor(0xd1 , 0x05);
	
 SP2518_write_cmos_sensor(0xd7 , 0x77);//exp_nr_outd_8lsb
	
 SP2518_write_cmos_sensor(0xd8 , 0x00);
	
 SP2518_write_cmos_sensor(0xd9 , 0x7b);//exp_outd_nr_8lsb
	
 SP2518_write_cmos_sensor(0xda , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0xca , 0x30);//mean dummy2low
	
 SP2518_write_cmos_sensor(0xcb , 0x50);//mean low2dummy
	
 SP2518_write_cmos_sensor(0xcc , 0xc0);//rpc low
	
 SP2518_write_cmos_sensor(0xcd , 0xc0);//rpc dummy
	
 SP2518_write_cmos_sensor(0xd5 , 0x80);//mean normal2dummy
	
 SP2518_write_cmos_sensor(0xd6 , 0x90);//mean dummy2normal
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
#elif  defined(CLK24M_72M)
	
//AE  pll 72M  8-13fps   
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0x30 , 0x08);//PLL 3倍频
	
 SP2518_write_cmos_sensor(0x03 , 0xf6);
	
 SP2518_write_cmos_sensor(0x04 , 0xbe); 
	
 SP2518_write_cmos_sensor(0x09 , 0x00);
	
 SP2518_write_cmos_sensor(0x0a , 0x8b);  
	
 SP2518_write_cmos_sensor(0xf0 , 0xa9);
	
 SP2518_write_cmos_sensor(0xf1 , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x01);	
	
 SP2518_write_cmos_sensor(0x90 , 0x0c);	
	
 SP2518_write_cmos_sensor(0x92 , 0x01);
	
 SP2518_write_cmos_sensor(0x98 , 0xa9);
	
 SP2518_write_cmos_sensor(0x99 , 0x00);
	
 SP2518_write_cmos_sensor(0x9a , 0x01);
	
 SP2518_write_cmos_sensor(0x9b , 0x00);
	

	
 // status 
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0xce , 0xec);
	
 SP2518_write_cmos_sensor(0xcf , 0x07);
	
 SP2518_write_cmos_sensor(0xd0 , 0xec);
	
 SP2518_write_cmos_sensor(0xd1 , 0x07);
	
 SP2518_write_cmos_sensor(0xd7 , 0xab);//exp_nr_outd_8lsb
	
 SP2518_write_cmos_sensor(0xd8 , 0x00);
	
 SP2518_write_cmos_sensor(0xd9 , 0xaf);//exp_outd_nr_8lsb
	
 SP2518_write_cmos_sensor(0xda , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0xca , 0x30);//mean dummy2low
	
 SP2518_write_cmos_sensor(0xcb , 0x50);//mean low2dummy
	
 SP2518_write_cmos_sensor(0xcc , 0xf8);//rpc low
	
 SP2518_write_cmos_sensor(0xcd , 0xc0);//rpc dummy
	
 SP2518_write_cmos_sensor(0xd5 , 0x80);//mean normal2dummy
	
 SP2518_write_cmos_sensor(0xd6 , 0x90);//mean dummy2normal
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
#endif
	
    //lens shading for 舜泰979C-171A\181A
	
 SP2518_write_cmos_sensor(0xfd , 0x00); 
	
 SP2518_write_cmos_sensor(0xa1 , 0x20);
	
 SP2518_write_cmos_sensor(0xa2 , 0x20);
	
 SP2518_write_cmos_sensor(0xa3 , 0x20);
	
 SP2518_write_cmos_sensor(0xa4 , 0xff);
	
 SP2518_write_cmos_sensor(0xa5 , 0x80);
	
 SP2518_write_cmos_sensor(0xa6 , 0x80);
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x64 , 0x1e);//28
	
 SP2518_write_cmos_sensor(0x65 , 0x1c);//25
	
 SP2518_write_cmos_sensor(0x66 , 0x1c);//2a
	
 SP2518_write_cmos_sensor(0x67 , 0x16);//25
	
 SP2518_write_cmos_sensor(0x68 , 0x1c);//25
	
 SP2518_write_cmos_sensor(0x69 , 0x1c);//29
	
 SP2518_write_cmos_sensor(0x6a , 0x1a);//28
	
 SP2518_write_cmos_sensor(0x6b , 0x16);//20
	
 SP2518_write_cmos_sensor(0x6c , 0x1a);//22
	
 SP2518_write_cmos_sensor(0x6d , 0x1a);//22
	
 SP2518_write_cmos_sensor(0x6e , 0x1a);//22
	
 SP2518_write_cmos_sensor(0x6f , 0x16);//1c
	
 SP2518_write_cmos_sensor(0xb8 , 0x04);//0a
	
 SP2518_write_cmos_sensor(0xb9 , 0x13);//0a
	
 SP2518_write_cmos_sensor(0xba , 0x00);//23
	
 SP2518_write_cmos_sensor(0xbb , 0x03);//14
	
 SP2518_write_cmos_sensor(0xbc , 0x03);//08
	
 SP2518_write_cmos_sensor(0xbd , 0x11);//08
	
 SP2518_write_cmos_sensor(0xbe , 0x00);//12
	
 SP2518_write_cmos_sensor(0xbf , 0x02);//00
	
 SP2518_write_cmos_sensor(0xc0 , 0x04);//05
	
 SP2518_write_cmos_sensor(0xc1 , 0x0e);//05
	
 SP2518_write_cmos_sensor(0xc2 , 0x00);//18
	
 SP2518_write_cmos_sensor(0xc3 , 0x05);//08   
	
//raw filter
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0xde , 0x0f);
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	

	
 SP2518_write_cmos_sensor(0x57 , 0x08);//raw_dif_thr
	
 SP2518_write_cmos_sensor(0x58 , 0x08);//a
	
 SP2518_write_cmos_sensor(0x56 , 0x08);//10
	
 SP2518_write_cmos_sensor(0x59 , 0x10);
	
 //R\B通道间平滑
 SP2518_write_cmos_sensor(0x5a , 0xa0);//raw_rb_fac_outdoor
	
 SP2518_write_cmos_sensor(0xc4 , 0xa0);//60raw_rb_fac_indoor
	
 SP2518_write_cmos_sensor(0x43 , 0xa0);//40raw_rb_fac_dummy  
	
 SP2518_write_cmos_sensor(0xad , 0x40);//raw_rb_fac_low  
	

  //Gr、Gb 通道内部平滑
	
 SP2518_write_cmos_sensor(0x4f , 0xa0);//raw_gf_fac_outdoor
	
 SP2518_write_cmos_sensor(0xc3 , 0xa0);//60raw_gf_fac_indoor
	
 SP2518_write_cmos_sensor(0x3f , 0xa0);//40raw_gf_fac_dummy
	
 SP2518_write_cmos_sensor(0x42 , 0x40);//raw_gf_fac_low
	
 SP2518_write_cmos_sensor(0xc2 , 0x15);
	

	
//Gr、Gb通道间平滑
 SP2518_write_cmos_sensor(0xb6 , 0x80);//raw_gflt_fac_outdoor
	
 SP2518_write_cmos_sensor(0xb7 , 0x80);//60raw_gflt_fac_normal
	
 SP2518_write_cmos_sensor(0xb8 , 0x40);//40raw_gflt_fac_dummy
	
 SP2518_write_cmos_sensor(0xb9 , 0x20);//raw_gflt_fac_low
	
 //Gr、Gb通道阈值
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x50 , 0x0c);//raw_grgb_thr
	
 SP2518_write_cmos_sensor(0x51 , 0x0c);
	
 SP2518_write_cmos_sensor(0x52 , 0x10);
	
 SP2518_write_cmos_sensor(0x53 , 0x10);
	
 SP2518_write_cmos_sensor(0xfd , 0x00);	
	

	
 // awb1   
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x11 , 0x10);
	
 SP2518_write_cmos_sensor(0x12 , 0x1f);
	
 SP2518_write_cmos_sensor(0x16 , 0x1c);
	
 SP2518_write_cmos_sensor(0x18 , 0x00);
	
 SP2518_write_cmos_sensor(0x19 , 0x00);
	
 SP2518_write_cmos_sensor(0x1b , 0x96);
	
 SP2518_write_cmos_sensor(0x1a , 0x9a);//95
	
 SP2518_write_cmos_sensor(0x1e , 0x2f);
	
 SP2518_write_cmos_sensor(0x1f , 0x29);
	
 SP2518_write_cmos_sensor(0x20 , 0xff);
	
 SP2518_write_cmos_sensor(0x22 , 0xff);  
	
 SP2518_write_cmos_sensor(0x28 , 0xce);
	
 SP2518_write_cmos_sensor(0x29 , 0x8a);
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xe7 , 0x03);
	
 SP2518_write_cmos_sensor(0xe7 , 0x00);
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x2a , 0xf0);
	
 SP2518_write_cmos_sensor(0x2b , 0x10);
	
 SP2518_write_cmos_sensor(0x2e , 0x04);
	
 SP2518_write_cmos_sensor(0x2f , 0x18);
	
 SP2518_write_cmos_sensor(0x21 , 0x60);
	
 SP2518_write_cmos_sensor(0x23 , 0x60);
	
 SP2518_write_cmos_sensor(0x8b , 0xab);
	
 SP2518_write_cmos_sensor(0x8f , 0x12);
	

	
 //awb2   
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x1a , 0x80);
	
 SP2518_write_cmos_sensor(0x1b , 0x80); 
	
 SP2518_write_cmos_sensor(0x43 , 0x80);
	
 //outdoor
	SP2518_write_cmos_sensor(0x00,0xd4);
	SP2518_write_cmos_sensor(0x01,0xb0);
	SP2518_write_cmos_sensor(0x02,0x90);
	SP2518_write_cmos_sensor(0x03,0x78);
	//d65  
	
 SP2518_write_cmos_sensor(0x35 , 0xd6);//
	
 SP2518_write_cmos_sensor(0x36 , 0xf0);//
	
 SP2518_write_cmos_sensor(0x37 , 0x7a);//8a
	
 SP2518_write_cmos_sensor(0x38 , 0x9a);//dc
	
 //indoor
	
 SP2518_write_cmos_sensor(0x39 , 0xab);
	
 SP2518_write_cmos_sensor(0x3a , 0xca);
	
 SP2518_write_cmos_sensor(0x3b , 0xa3);
	
 SP2518_write_cmos_sensor(0x3c , 0xc1);
	
//f      
	
 SP2518_write_cmos_sensor(0x31 , 0x82);//7d
	
 SP2518_write_cmos_sensor(0x32 , 0xa5);//a0
	
 SP2518_write_cmos_sensor(0x33 , 0xd6);//d2
	
 SP2518_write_cmos_sensor(0x34 , 0xec);//e8
	
 //cwf     
	
 SP2518_write_cmos_sensor(0x3d , 0xa5);//a7
	
 SP2518_write_cmos_sensor(0x3e , 0xc2);//be
	
 SP2518_write_cmos_sensor(0x3f , 0xa7);//b3
	
 SP2518_write_cmos_sensor(0x40 , 0xc5);//c5
	

	
  //Color Correction
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x1c , 0xc0);//add
	SP2518_write_cmos_sensor(0x1d , 0x95);//add
#if 1
	SP2518_write_cmos_sensor(0xa0 , 0xa6);
	SP2518_write_cmos_sensor(0xa1 , 0xda);
	SP2518_write_cmos_sensor(0xa2 , 0x00);
	SP2518_write_cmos_sensor(0xa3 , 0x06);
	SP2518_write_cmos_sensor(0xa4 , 0xb2);
	SP2518_write_cmos_sensor(0xa5 , 0xc7);
	SP2518_write_cmos_sensor(0xa6 , 0x00);
	SP2518_write_cmos_sensor(0xa7 , 0xce);
	SP2518_write_cmos_sensor(0xa8 , 0xb2);
	SP2518_write_cmos_sensor(0xa9 , 0x0c);
	SP2518_write_cmos_sensor(0xaa , 0x30);
	SP2518_write_cmos_sensor(0xab , 0x0c);
#endif
#if 0
	SP2518_write_cmos_sensor(0xa0 , 0x80);//b8 
	
 SP2518_write_cmos_sensor(0xa1 , 0x00);//d5
	
 SP2518_write_cmos_sensor(0xa2 , 0x00);//f2
	
 SP2518_write_cmos_sensor(0xa3 , 0xf6);//e8
	
 SP2518_write_cmos_sensor(0xa4 , 0x99);//95
	
 SP2518_write_cmos_sensor(0xa5 , 0xf2);//03
	
 SP2518_write_cmos_sensor(0xa6 , 0x0d);//f2
	
 SP2518_write_cmos_sensor(0xa7 , 0xda);//c4
	
 SP2518_write_cmos_sensor(0xa8 , 0x98);//ca
	
 SP2518_write_cmos_sensor(0xa9 , 0x00);//3c
	
 SP2518_write_cmos_sensor(0xaa , 0x33);//03
	
 SP2518_write_cmos_sensor(0xab , 0x0c);//0f
#endif

#if 1
SP2518_write_cmos_sensor(0xac , 0xcc);//b8 
	
 SP2518_write_cmos_sensor(0xad , 0xda);//d5
	
 SP2518_write_cmos_sensor(0xae , 0xda);//f2
	
 SP2518_write_cmos_sensor(0xaf , 0xf2);//e8
	
 SP2518_write_cmos_sensor(0xb0 , 0xa6);//95
	
 SP2518_write_cmos_sensor(0xb1 , 0xe8);//03
	
 SP2518_write_cmos_sensor(0xb2 , 0x00);//f2
	
 SP2518_write_cmos_sensor(0xb3 , 0xe7);//c4
	
 SP2518_write_cmos_sensor(0xb4 , 0x99);//ca
	
 SP2518_write_cmos_sensor(0xb5 , 0x3c);//3c
	
 SP2518_write_cmos_sensor(0xb6 , 0x33);//03
	
 SP2518_write_cmos_sensor(0xb7 , 0x0c);//0f

  SP2518_write_cmos_sensor(0xfd , 0x01);//3c//additional
	
 SP2518_write_cmos_sensor(0x18, 0x01);//03
	
 SP2518_write_cmos_sensor(0x19 , 0x01);//0f
#endif

#if 0//ori para
 SP2518_write_cmos_sensor(0xac , 0xc0);//b8 
	
 SP2518_write_cmos_sensor(0xad , 0xc0);//d5
	
 SP2518_write_cmos_sensor(0xae , 0x00);//f2
	
 SP2518_write_cmos_sensor(0xaf , 0xf2);//e8
	
 SP2518_write_cmos_sensor(0xb0 , 0xa6);//95
	
 SP2518_write_cmos_sensor(0xb1 , 0xe8);//03
	
 SP2518_write_cmos_sensor(0xb2 , 0x00);//f2
	
 SP2518_write_cmos_sensor(0xb3 , 0xe7);//c4
	
 SP2518_write_cmos_sensor(0xb4 , 0x99);//ca
	
 SP2518_write_cmos_sensor(0xb5 , 0x0c);//3c
	
 SP2518_write_cmos_sensor(0xb6 , 0x33);//03
	
 SP2518_write_cmos_sensor(0xb7 , 0x0c);//0f
#endif	

	
  //Saturation
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xbf , 0x01);
	
 SP2518_write_cmos_sensor(0xbe , 0xbb);
	
 SP2518_write_cmos_sensor(0xc0 , 0xb0);
	
 SP2518_write_cmos_sensor(0xc1 , 0xf0);
	
 SP2518_write_cmos_sensor(0xd3 , 0x77);
	
 SP2518_write_cmos_sensor(0xd4 , SP2518_P0_0xd4);
	
 SP2518_write_cmos_sensor(0xd6 , 0x77);
	
 SP2518_write_cmos_sensor(0xd7 , 0x77);
	
 SP2518_write_cmos_sensor(0xd8 , 0x77);
	
 SP2518_write_cmos_sensor(0xd9 , SP2518_P0_0xd9);
	
 SP2518_write_cmos_sensor(0xda , 0x77);
	
 SP2518_write_cmos_sensor(0xdb , 0x77);
	
 //uv_dif  
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xf3 , 0x03);
	
 SP2518_write_cmos_sensor(0xb0 , 0x00);
	
 SP2518_write_cmos_sensor(0xb1 , 0x23); 
	

	
 //gamma1 outdoor
	
 SP2518_write_cmos_sensor(0xfd , 0x00);//0 
	
 SP2518_write_cmos_sensor(0x8b , 0x0 );//14  
	
 SP2518_write_cmos_sensor(0x8c , 0xa );//24
	
 SP2518_write_cmos_sensor(0x8d , 0x13);//3a
	
 SP2518_write_cmos_sensor(0x8e , 0x25);//59
	
 SP2518_write_cmos_sensor(0x8f , 0x43);//6f
	
 SP2518_write_cmos_sensor(0x90 , 0x5d);//84
	
 SP2518_write_cmos_sensor(0x91 , 0x74);//95
	
 SP2518_write_cmos_sensor(0x92 , 0x88);//a3
	
 SP2518_write_cmos_sensor(0x93 , 0x9a);//b1
	
 SP2518_write_cmos_sensor(0x94 , 0xa9);//be
	
 SP2518_write_cmos_sensor(0x95 , 0xb5);//c7
	
 SP2518_write_cmos_sensor(0x96 , 0xc0);//d1
	
 SP2518_write_cmos_sensor(0x97 , 0xca);//d9
	
 SP2518_write_cmos_sensor(0x98 , 0xd4);//e1
	
 SP2518_write_cmos_sensor(0x99 , 0xdd);//e9
	
 SP2518_write_cmos_sensor(0x9a , 0xe6);//f1
	
 SP2518_write_cmos_sensor(0x9b , 0xef);//01
	
 SP2518_write_cmos_sensor(0xfd , 0x01);//f9
	
 SP2518_write_cmos_sensor(0x8d , 0xf7);//ff
	
 SP2518_write_cmos_sensor(0x8e , 0xff);//
	
 //gamma2   
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0x78 , 0x0 ); //0  
	
 SP2518_write_cmos_sensor(0x79 , 0xa ); //14
	
 SP2518_write_cmos_sensor(0x7a , 0x13); //24
	
 SP2518_write_cmos_sensor(0x7b , 0x25); //3a
	
 SP2518_write_cmos_sensor(0x7c , 0x43); //59
	
 SP2518_write_cmos_sensor(0x7d , 0x5d); //6f
	
 SP2518_write_cmos_sensor(0x7e , 0x74); //84
	
 SP2518_write_cmos_sensor(0x7f , 0x88); //95
	
 SP2518_write_cmos_sensor(0x80 , 0x9a); //a3
	
 SP2518_write_cmos_sensor(0x81 , 0xa9); //b1
	
 SP2518_write_cmos_sensor(0x82 , 0xb5); //be
	
 SP2518_write_cmos_sensor(0x83 , 0xc0); //c7
	
 SP2518_write_cmos_sensor(0x84 , 0xca); //d1
	
 SP2518_write_cmos_sensor(0x85 , 0xd4); //d9
	
 SP2518_write_cmos_sensor(0x86 , 0xdd); //e1
	
 SP2518_write_cmos_sensor(0x87 , 0xe6); //e9
	
 SP2518_write_cmos_sensor(0x88 , 0xef); //f1
	
 SP2518_write_cmos_sensor(0x89 , 0xf7); //f9
	
 SP2518_write_cmos_sensor(0x8a , 0xff); //ff
	
 //gamma_ae 
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x96 , 0x46);
	
 SP2518_write_cmos_sensor(0x97 , 0x14);
	
 SP2518_write_cmos_sensor(0x9f , 0x06);
	

	
  /*//光斑过度好
	//gamma1
	SP2518_write_cmos_sensor(0xfd,0x00);
	SP2518_write_cmos_sensor(0x8b,0x00);
	SP2518_write_cmos_sensor(0x8c,0x14);
	SP2518_write_cmos_sensor(0x8d,0x24);
	SP2518_write_cmos_sensor(0x8e,0x3A);
	SP2518_write_cmos_sensor(0x8f,0x59);
	SP2518_write_cmos_sensor(0x90,0x70);
	SP2518_write_cmos_sensor(0x91,0x85);
	SP2518_write_cmos_sensor(0x92,0x96);
	SP2518_write_cmos_sensor(0x93,0xA6);
	SP2518_write_cmos_sensor(0x94,0xB3);
	SP2518_write_cmos_sensor(0x95,0xBE);
	SP2518_write_cmos_sensor(0x96,0xC9);
	SP2518_write_cmos_sensor(0x97,0xD2);
	SP2518_write_cmos_sensor(0x98,0xDB);
	SP2518_write_cmos_sensor(0x99,0xE3);
	SP2518_write_cmos_sensor(0x9a,0xEB);
	SP2518_write_cmos_sensor(0x9b,0xF2);
	SP2518_write_cmos_sensor(0xfd,0x01);
	SP2518_write_cmos_sensor(0x8d,0xF9);
	SP2518_write_cmos_sensor(0x8e,0xFF);
	//gamma2   
	SP2518_write_cmos_sensor(0xfd,0x00);
	SP2518_write_cmos_sensor(0x78,0x00);
	SP2518_write_cmos_sensor(0x79,0x14);
	SP2518_write_cmos_sensor(0x7a,0x24);
	SP2518_write_cmos_sensor(0x7b,0x3A);
	SP2518_write_cmos_sensor(0x7c,0x59);
	SP2518_write_cmos_sensor(0x7d,0x70);
	SP2518_write_cmos_sensor(0x7e,0x85);
	SP2518_write_cmos_sensor(0x7f,0x96);
	SP2518_write_cmos_sensor(0x80,0xA6);
	SP2518_write_cmos_sensor(0x81,0xB3);
	SP2518_write_cmos_sensor(0x82,0xBE);
	SP2518_write_cmos_sensor(0x83,0xC9);
	SP2518_write_cmos_sensor(0x84,0xD2);
	SP2518_write_cmos_sensor(0x85,0xDB);
	SP2518_write_cmos_sensor(0x86,0xE3);
	SP2518_write_cmos_sensor(0x87,0xEB);
	SP2518_write_cmos_sensor(0x88,0xF2);
	SP2518_write_cmos_sensor(0x89,0xF9);
	SP2518_write_cmos_sensor(0x8a,0xFF);    
	*/
	//HEQ    
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xdd , SP2518_P0_0xdd);
	
 SP2518_write_cmos_sensor(0xde , SP2518_P0_0xde);
	
 SP2518_write_cmos_sensor(0xdf , 0x80);
	

	
 //Ytarget  
	
 SP2518_write_cmos_sensor(0xfd , 0x00);  
	
 SP2518_write_cmos_sensor(0xed , SP2518_P0_0xf7+0x04);
	
 SP2518_write_cmos_sensor(0xf7 , SP2518_P0_0xf7);
	
 SP2518_write_cmos_sensor(0xf8 , SP2518_P0_0xf8);
	
 SP2518_write_cmos_sensor(0xec , SP2518_P0_0xf8-0x04);
	
 SP2518_write_cmos_sensor(0xef , SP2518_P0_0xf9+0x04);
	
 SP2518_write_cmos_sensor(0xf9 , SP2518_P0_0xf9);
	
 SP2518_write_cmos_sensor(0xfa , SP2518_P0_0xfa);
	
 SP2518_write_cmos_sensor(0xee , SP2518_P0_0xfa-0x04); 
	

	
//sharpen  
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0xdf , 0x0f);
	
 SP2518_write_cmos_sensor(0xe5 , 0x10);
	
 SP2518_write_cmos_sensor(0xe7 , 0x10);
	
 SP2518_write_cmos_sensor(0xe8 , 0x20);  //18
	
 SP2518_write_cmos_sensor(0xec , 0x20);  //18
	
 SP2518_write_cmos_sensor(0xe9 , SP2518_P1_0xe9);
	
 SP2518_write_cmos_sensor(0xed , SP2518_P1_0xed);  
	
 SP2518_write_cmos_sensor(0xea , 0x10);
	
 SP2518_write_cmos_sensor(0xef , 0x10);  
	
 SP2518_write_cmos_sensor(0xeb , 0x10);
	
 SP2518_write_cmos_sensor(0xf0 , 0x10);    
	

	
//gw    
	
 SP2518_write_cmos_sensor(0xfd , 0x01);
	
 SP2518_write_cmos_sensor(0x70 , 0x76);
	
 SP2518_write_cmos_sensor(0x7b , 0x40);
	
 SP2518_write_cmos_sensor(0x81 , 0x30);
	

	

	
 //Y_offset 
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0xb2 , SP2518_NORMAL_Y0ffset);
	
 SP2518_write_cmos_sensor(0xb3 , 0x1f);
	
 SP2518_write_cmos_sensor(0xb4 , 0x30);
	
 SP2518_write_cmos_sensor(0xb5 , 0x50);
	

	
//CNR    
	
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0x5b , 0x20);
	
 SP2518_write_cmos_sensor(0x61 , 0x80);//20
	
 SP2518_write_cmos_sensor(0x77 , 0x80);//20//a0
	
 SP2518_write_cmos_sensor(0xca , 0x80);//20
	
 //YNR
	  SP2518_write_cmos_sensor(0xab , 0x00);
	
 SP2518_write_cmos_sensor(0xac , 0x02);
	
 SP2518_write_cmos_sensor(0xae , 0x08);
 SP2518_write_cmos_sensor(0xaf , 0x20);
 SP2518_write_cmos_sensor(0xfd , 0x00);
	
 SP2518_write_cmos_sensor(0x31 , 0x10);

 SP2518_write_cmos_sensor(0x32 , 0x0d);
 SP2518_write_cmos_sensor(0x33 , 0xcf);//ef
 SP2518_write_cmos_sensor(0x34 , 0x7f);//3f
	
 SP2518_write_cmos_sensor(0x35 , 0x40);//0x41 capture ok.preview red blue neg
	
 SP2518_write_cmos_sensor(0x1b , 0x1a);//02
	#if 1//

    //sca l e  1600*1200					   

    SP2518_write_cmos_sensor(0xfd , 0x01);		   

    SP2518_write_cmos_sensor(0x0e , 0x00);	//disable scale 

    SP2518_write_cmos_sensor(0x0f , 0x00);	//disable scale 
    SP2518_write_cmos_sensor(0xfd , 0x00);
    SP2518_write_cmos_sensor(0x2f , 0x00); //PCLK 不分频
	#endif
	#if 0

    //window  1280*960					   

    SP2518_write_cmos_sensor(0xfd , 0x01);		   

    SP2518_write_cmos_sensor(0x0e , 0x00);	//disable scale 	   

    SP2518_write_cmos_sensor(0xfd , 0x00);

    SP2518_write_cmos_sensor(0x4b , 0x00);

    SP2518_write_cmos_sensor(0x4c , 0xa0);

    SP2518_write_cmos_sensor(0x47 , 0x00);

    SP2518_write_cmos_sensor(0x48 , 0x78);

    SP2518_write_cmos_sensor(0x4d , 0x05);

    SP2518_write_cmos_sensor(0x4e , 0x00);

    SP2518_write_cmos_sensor(0x49 , 0x03);

    SP2518_write_cmos_sensor(0x4a , 0xc0);

    SP2518_write_cmos_sensor(0xfd , 0x00);

    SP2518_write_cmos_sensor(0x2f , 0x00);  //PCLK 不分频

    SP2518_write_cmos_sensor(0xFF , 0xFF);
	#endif
	#if 0

//  sca l e  800*600   2分频

	SP2518_write_cmos_sensor(0xfd , 0x00);

	SP2518_write_cmos_sensor(0x4b , 0x00);

	SP2518_write_cmos_sensor(0x4c , 0x00);

	SP2518_write_cmos_sensor(0x47 , 0x00);

	SP2518_write_cmos_sensor(0x48 , 0x00);

	

	SP2518_write_cmos_sensor(0xfd , 0x01);

	SP2518_write_cmos_sensor(0x0e , 0x01);

	SP2518_write_cmos_sensor(0x0a , 0x02);

	SP2518_write_cmos_sensor(0x0b , 0x58);

	SP2518_write_cmos_sensor(0x0c , 0x03);

	SP2518_write_cmos_sensor(0x0d , 0x20);

	SP2518_write_cmos_sensor(0x08 , 0x00);

	SP2518_write_cmos_sensor(0x09 , 0x40);

	SP2518_write_cmos_sensor(0x06 , 0x00);

	SP2518_write_cmos_sensor(0x07 , 0x40);

#ifdef CLK24M_48M

  	 SP2518_write_cmos_sensor(0xfd , 0x01);

        SP2518_write_cmos_sensor(0x0f , 0x00);

        SP2518_write_cmos_sensor(0xfd , 0x00);

        SP2518_write_cmos_sensor(0x2f , 0x00);

 #elif  defined(CLK24M_72M)

 	 SP2518_write_cmos_sensor(0xfd , 0x01);

        SP2518_write_cmos_sensor(0x0f , 0x01);

        SP2518_write_cmos_sensor(0xfd , 0x00);

        SP2518_write_cmos_sensor(0x2f , 0x08);//PCLK 2分频

  #endif 

        SP2518_write_cmos_sensor(0xfd , 0x00);
	#endif
	

}                                  

/*************************************************************************
* FUNCTION
*    SP2518PreviewSetting
*
* DESCRIPTION
*    This function config Preview setting related registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void SP2518PreviewSetting(void)
{
#if 0
              //SP2518_write_cmos_sensor(0x0103, 0x01); 
              //mDELAY(10); 
              //SP2518InitialSetting(); 

              SP2518_write_cmos_sensor(0X0100, 0X00);	

              SP2518_write_cmos_sensor(0x3503,SP2518_read_cmos_sensor(0x3503)|0x07);

              SP2518_write_cmos_sensor(0x3500,((SP2518Sensor.PreviewShutter*16)>>16)&0xff); 
              SP2518_write_cmos_sensor(0x3501,((SP2518Sensor.PreviewShutter*16)>>8)&0xff); 
              SP2518_write_cmos_sensor(0x3502,(SP2518Sensor.PreviewShutter*16)&0xff); 
	            SP2518_write_cmos_sensor(0x350B, SP2518Sensor.SensorGain);	
	
              SP2518_write_cmos_sensor(0x3a00,SP2518_read_cmos_sensor(0x3a00)|0x04); 
              SP2518_write_cmos_sensor(0x3503,SP2518_read_cmos_sensor(0x3503)&0xf8); 
              //SP2518_write_cmos_sensor(0x3406,SP2518ReadReg(0x3406)&0xfe); 
              SP2518_write_cmos_sensor(0x5066,0x28); //26
              SP2518_write_cmos_sensor(0x5067,0x10); //20
              SP2518_write_cmos_sensor(0x506a,0x10); //18
              SP2518_write_cmos_sensor(0x506b,0x20); //28

              SP2518_write_cmos_sensor(0x3800,0x00); 
              SP2518_write_cmos_sensor(0x3801,0x00); 
              SP2518_write_cmos_sensor(0x3802,0x00); 
              SP2518_write_cmos_sensor(0x3803,0x00); 
              SP2518_write_cmos_sensor(0x3804,0x06); 
              SP2518_write_cmos_sensor(0x3805,0x5f); 
              SP2518_write_cmos_sensor(0x3806,0x04); 
              SP2518_write_cmos_sensor(0x3807,0xb7); 
              SP2518_write_cmos_sensor(0x3808,0x03); 
              SP2518_write_cmos_sensor(0x3809,0x20); 
              SP2518_write_cmos_sensor(0x380a,0x02); 
              SP2518_write_cmos_sensor(0x380b,0x58); 
              SP2518_write_cmos_sensor(0x3811,0x08); 
              SP2518_write_cmos_sensor(0x3813,0x02); 
              SP2518_write_cmos_sensor(0x3814,0x31); 
              SP2518_write_cmos_sensor(0x3815,0x31); 
              if(1) 
              { 
              SP2518_write_cmos_sensor(0x3820,0x81);           
              SP2518_write_cmos_sensor(0x3821,0x01);         
              } 
              else 
              {         
              SP2518_write_cmos_sensor(0x3820,0x87);           
              SP2518_write_cmos_sensor(0x3821,0x07); 
              }         
              SP2518_write_cmos_sensor(0x3623,0x00); 
              SP2518_write_cmos_sensor(0x3634,0x76); 
              SP2518_write_cmos_sensor(0x3701,0x44); 
              SP2518_write_cmos_sensor(0x3208,0xa1); 
              SP2518_write_cmos_sensor(0x3705,0x0c);                           
        
              SP2518_write_cmos_sensor(0x370a,0x52); 
              SP2518_write_cmos_sensor(0x4608,0x00); 
              SP2518_write_cmos_sensor(0x4609,0x80); 
              SP2518_write_cmos_sensor(0x5002,0x10);           

              //pclk setting 
              SP2518_write_cmos_sensor(0x3003,0x80);//30fps 26mclk 
              SP2518_write_cmos_sensor(0x3004,0x10); 
              SP2518_write_cmos_sensor(0x3005,0x16); 
              SP2518_write_cmos_sensor(0x3006,0x0d); 
              //dummy lines&dummy pixels 
              SP2518_write_cmos_sensor(0x380c,0x05); 
              SP2518_write_cmos_sensor(0x380d,0x14); 
              SP2518_write_cmos_sensor(0x380e,0x02); 
              SP2518_write_cmos_sensor(0x380f,0xe3); 
              //banding       
              SP2518_write_cmos_sensor(0x3a08,0x00); 
              SP2518_write_cmos_sensor(0x3a09,0xb9); 
              SP2518_write_cmos_sensor(0x3a0e,0x04); 
              SP2518_write_cmos_sensor(0x3a0a,0x00); 
              SP2518_write_cmos_sensor(0x3a0b,0x9a); 
              SP2518_write_cmos_sensor(0x3a0d,0x04);   
              
              SP2518_write_cmos_sensor(0x4300,0x31); //modify first pixels
              
              SP2518_write_cmos_sensor(0X0100,0X01);
              SP2518_write_cmos_sensor(0X301d,0X08);
	            mDELAY(10); 
              SP2518_write_cmos_sensor(0X301d,0X00);

	            spin_lock(&sp2518_drv_lock);
	            SP2518Sensor.IsPVmode = KAL_TRUE;
	            SP2518Sensor.PreviewPclk= 480;
	            SP2518Sensor.SensorMode= SENSOR_MODE_PREVIEW;
	            spin_unlock(&sp2518_drv_lock);
	#endif
}

/*************************************************************************
* FUNCTION
*     SP2518FullSizeCaptureSetting
*
* DESCRIPTION
*    This function config full size capture setting related registers of CMOS sensor.
*
* PARAMETERS
*    None
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void SP2518FullSizeCaptureSetting(void)
{
#if 0
		            SP2518_write_cmos_sensor(0X0100, 0X00);	
                SP2518_write_cmos_sensor(0x3a00,SP2518_read_cmos_sensor(0x3a00)&0xfb); 
                SP2518_write_cmos_sensor(0x3503,SP2518_read_cmos_sensor(0x3503)|0x07); 
                //SP2518_write_cmos_sensor(0x3406,SP2518_read_cmos_sensor(0x3406)|0x01);                
  
                SP2518_write_cmos_sensor(0x5066, 0x28);         
                SP2518_write_cmos_sensor(0x5067, 0x18); 
                SP2518_write_cmos_sensor(0x506a, 0x16);    
                SP2518_write_cmos_sensor(0x506b, 0x2e);    

                SP2518_write_cmos_sensor(0x3800, 0x00); 
                SP2518_write_cmos_sensor(0x3801, 0x00); 
                SP2518_write_cmos_sensor(0x3802, 0x00); 
                SP2518_write_cmos_sensor(0x3803, 0x00); 
                SP2518_write_cmos_sensor(0x3804, 0x06); 
                SP2518_write_cmos_sensor(0x3805, 0x5f); 
                SP2518_write_cmos_sensor(0x3806, 0x04); 
                SP2518_write_cmos_sensor(0x3807, 0xbb); 
                SP2518_write_cmos_sensor(0x3808, 0x06); 
                SP2518_write_cmos_sensor(0x3809, 0x40); 
                SP2518_write_cmos_sensor(0x380a, 0x04); 
                SP2518_write_cmos_sensor(0x380b, 0xb0); 
                SP2518_write_cmos_sensor(0x3811, 0x10); 
                SP2518_write_cmos_sensor(0x3813, 0x06); 
                SP2518_write_cmos_sensor(0x3814, 0x11); 
                SP2518_write_cmos_sensor(0x3815, 0x11); 
                
                SP2518_write_cmos_sensor(0x3623, 0x00); 
                SP2518_write_cmos_sensor(0x3634, 0x44); 
                SP2518_write_cmos_sensor(0x3701, 0x44); 
                SP2518_write_cmos_sensor(0x3208, 0xa2); 
                SP2518_write_cmos_sensor(0x3705, 0x18);      
                SP2518_write_cmos_sensor(0x3820, SP2518_read_cmos_sensor(0x3820)&0xfe); 
                SP2518_write_cmos_sensor(0x3821, SP2518_read_cmos_sensor(0x3821)&0xfe); 
                
                SP2518_write_cmos_sensor(0x370a, 0x12); 
                SP2518_write_cmos_sensor(0x4608, 0x00); 
                SP2518_write_cmos_sensor(0x4609, 0x80); 
                SP2518_write_cmos_sensor(0x5002, 0x00);
                //frame rate
                SP2518_write_cmos_sensor(0x3003, 0x80); 
                SP2518_write_cmos_sensor(0x3004, 0x10);        
                SP2518_write_cmos_sensor(0x3005, 0x21);//15fps;0x16=10fps
                SP2518_write_cmos_sensor(0x3006, 0x0d); 

                SP2518_write_cmos_sensor(0x380c, 0x07); 
                SP2518_write_cmos_sensor(0x380d, 0x9f); 
                SP2518_write_cmos_sensor(0x380e, 0x04); 
                SP2518_write_cmos_sensor(0x380f, 0xd0); 
                           
                SP2518_write_cmos_sensor(0x3a08, 0x00); 
                SP2518_write_cmos_sensor(0x3a09, 0xb9);
                SP2518_write_cmos_sensor(0x3a0e, 0x06); 
                
                SP2518_write_cmos_sensor(0x4300, 0x30); //modify first pixels
                
                SP2518_write_cmos_sensor(0x3a0a, 0x00); 
                SP2518_write_cmos_sensor(0x3a0b, 0x9a);                 
                SP2518_write_cmos_sensor(0x3a0d, 0x08);         
	              SP2518_write_cmos_sensor(0x4003, 0x88);
	              
		            SP2518_write_cmos_sensor(0X0100, 0X01);	

	              spin_lock(&sp2518_drv_lock);
	              SP2518Sensor.IsPVmode = KAL_FALSE;
	              SP2518Sensor.CapturePclk= 480;//585
	              SP2518Sensor.SensorMode= SENSOR_MODE_CAPTURE;
	              spin_unlock(&sp2518_drv_lock);
	#endif
}


/*************************************************************************
* FUNCTION
*    SP2518SetHVMirror
*
* DESCRIPTION
*    This function set sensor Mirror
*
* PARAMETERS
*    Mirror
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void SP2518SetHVMirror(kal_uint8 Mirror)
{
  kal_uint8 mirror= 0, flip=0;
  #if 0
#ifdef SP2518_DEBUG
  kal_wap_trace(MOD_ENG,TRACE_INFO,"Mirror:%d",Mirror);
#endif 
    
  	flip = SP2518_read_cmos_sensor(0x3820);
	mirror=SP2518_read_cmos_sensor(0x3821);

	switch (Mirror)
	{
	case IMAGE_NORMAL:
		SP2518_write_cmos_sensor(0x3820, flip&0xf9);     
		SP2518_write_cmos_sensor(0x3821, mirror&0xf9);
		break;
	case IMAGE_H_MIRROR:
		SP2518_write_cmos_sensor(0x3820, flip&0xf9);     
		SP2518_write_cmos_sensor(0x3821, mirror|0x06);
		break;
	case IMAGE_V_MIRROR: 
		SP2518_write_cmos_sensor(0x3820, flip|0x06);     
		SP2518_write_cmos_sensor(0x3821, mirror&0xf9);
		break;
		
	case IMAGE_HV_MIRROR:
		SP2518_write_cmos_sensor(0x3820, flip|0x06);     
		SP2518_write_cmos_sensor(0x3821, mirror|0x06);
		break;
		
	default:
		ASSERT(0);
	}
	#endif
}

/*************************************************************************
* FUNCTION
*	SP2518Open
*
* DESCRIPTION
*	This function initialize the registers of CMOS sensor
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 SP2518Open(void)
{
	volatile signed int i;
	kal_uint16 sensor_id = 0;
	//kdCISModulePowerOn(1,1,"sp2518yuv","kd_camera_hw");
	SP2518SENSORDB("[SP2518]SP2518Open enter :\n ");
	SP2518_write_cmos_sensor(0xfd,0x00);// Reset sensor
    mDELAY(10);
	//  Read sensor ID to adjust I2C is OK?
	for(i=0;i<3;i++)
	{
		sensor_id = SP2518_read_cmos_sensor(0x02);
		SP2518SENSORDB("RONLUS SP2518 READ ID :%x",sensor_id);
		if(sensor_id != SP2518_SENSOR_ID)
		{
			return ERROR_SENSOR_CONNECT_FAIL;
		}
	}
	spin_lock(&sp2518_drv_lock);
	SP2518Sensor.VideoMode = KAL_FALSE;
  	SP2518Sensor.NightMode = KAL_FALSE;
	SP2518Sensor.Fps = 100;
  	SP2518Sensor.ShutterStep= 0xb8;
	SP2518Sensor.CaptureDummyPixels = 0;
  	SP2518Sensor.CaptureDummyLines = 0;
	SP2518Sensor.PreviewDummyPixels = 0;
  	SP2518Sensor.PreviewDummyLines = 0;
	SP2518Sensor.SensorMode= SENSOR_MODE_INIT;
	spin_unlock(&sp2518_drv_lock);

	SP2518InitialSetting();

	SP2518SENSORDB("[SP2518]SP2518Open exit :\n ");

	return ERROR_NONE;
}	/* SP2518Open() */

/*************************************************************************
* FUNCTION
*	SP2518Close
*
* DESCRIPTION
*	This function is to turn off sensor module power.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 SP2518Close(void)
{
//	CISModulePowerOn(FALSE);

	return ERROR_NONE;
}	/* SP2518Close() */
/*************************************************************************
* FUNCTION
*	SP2518Preview
*
* DESCRIPTION
*	This function start the sensor preview.
*
* PARAMETERS
*	*image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 SP2518Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint8 iTemp, temp_AE_reg, temp_AWB_reg;
    kal_uint16 iDummyPixels = 0, iDummyLines = 0, iStartX = 1, iStartY = 1;
	
	SP2518SENSORDB("[SP2518]SP2518Preview enter :\n ");
	//SP2518_set_AE_mode(KAL_TRUE);
	//SP2518_set_AWB_mode(KAL_TRUE);
	#if 0//defined(MT6575)
	switch(CurrentScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			SP2518SENSORDB("[SP2518][SP2518Preview]MSDK_SCENARIO_ID_CAMERA_ZSD :\n ");
			//SP2518FullSizeCaptureSetting();			
			break;
		default:
			/SP2518PreviewSetting();
			break;
	}
	#else
		//SP2518PreviewSetting();
	#endif
#if 1//
        //sca l e  1600*1200					   
        SP2518_write_cmos_sensor(0xfd , 0x01);		   
        SP2518_write_cmos_sensor(0x0e , 0x00);	//disable scale 
        SP2518_write_cmos_sensor(0x0f , 0x00);	//disable scale 
        SP2518_write_cmos_sensor(0xfd , 0x00);
        SP2518_write_cmos_sensor(0x2f , 0x00); //PCLK 不分频
#endif
        SP2518_write_cmos_sensor(0xfd , 0x00);
        SP2518_write_cmos_sensor(0x35 , 0x40);//0x41 capture ok.preview red blue neg


	#if 0

//  sca l e  800*600   2分频

	SP2518_write_cmos_sensor(0xfd , 0x00);

	SP2518_write_cmos_sensor(0x4b , 0x00);

	SP2518_write_cmos_sensor(0x4c , 0x00);

	SP2518_write_cmos_sensor(0x47 , 0x00);

	SP2518_write_cmos_sensor(0x48 , 0x00);

	SP2518_write_cmos_sensor(0xfd , 0x01);

	SP2518_write_cmos_sensor(0x0e , 0x01);

	SP2518_write_cmos_sensor(0x0a , 0x02);

	SP2518_write_cmos_sensor(0x0b , 0x58);

	SP2518_write_cmos_sensor(0x0c , 0x03);

	SP2518_write_cmos_sensor(0x0d , 0x20);

	SP2518_write_cmos_sensor(0x08 , 0x00);

	SP2518_write_cmos_sensor(0x09 , 0x40);

	SP2518_write_cmos_sensor(0x06 , 0x00);

	SP2518_write_cmos_sensor(0x07 , 0x40);

#ifdef CLK24M_48M

  	 SP2518_write_cmos_sensor(0xfd , 0x01);

        SP2518_write_cmos_sensor(0x0f , 0x00);

        SP2518_write_cmos_sensor(0xfd , 0x00);

        SP2518_write_cmos_sensor(0x2f , 0x00);

 #elif  defined(CLK24M_72M)

 	 SP2518_write_cmos_sensor(0xfd , 0x01);

        SP2518_write_cmos_sensor(0x0f , 0x01);

        SP2518_write_cmos_sensor(0xfd , 0x00);

        SP2518_write_cmos_sensor(0x2f , 0x08);//PCLK 2分频

  #endif 

        SP2518_write_cmos_sensor(0xfd , 0x00);
	#endif

	//mDELAY(300);	

	//SP2518SetHVMirror(3);
	//SP2518_night_mode(1);
	
	SP2518SENSORDB("RONLUS [SP2518]SP2518Preview exit :\n ");
	
  	return TRUE ;//ERROR_NONE;
  	
}	/* SP2518Preview() */

UINT32 SP2518Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	SP2518SENSORDB("[SP2518]SP2518Capture enter :\n ");
	kal_uint32 shutter = 0, temp_reg = 0;
  	kal_uint32 prev_line_len = 0;
  	kal_uint32 cap_line_len = 0;
	kal_uint32 temp;
	#if 0
	if(SENSOR_MODE_PREVIEW == SP2518Sensor.SensorMode )
	{
	SP2518SENSORDB("[SP2518]Normal Capture\n ");
	shutter=SP2518ReadShutter();
	temp =SP2518ReadSensorGain();
	spin_lock(&sp2518_drv_lock);
  	SP2518Sensor.SensorGain= temp;
	spin_unlock(&sp2518_drv_lock);
	SP2518SetHVMirror(3);
	SP2518_set_AE_mode(KAL_FALSE);
	SP2518_set_AWB_mode(KAL_FALSE);
	mDELAY(30);
	SP2518FullSizeCaptureSetting();
	spin_lock(&sp2518_drv_lock);
	SP2518Sensor.CaptureDummyPixels = 0;
  	SP2518Sensor.CaptureDummyLines = 0;
	spin_unlock(&sp2518_drv_lock);
  //shutter=shutter*4/3;
  shutter=shutter*2;
  	SP2518SetDummy(SP2518Sensor.CaptureDummyPixels, SP2518Sensor.CaptureDummyLines);

	//prev_line_len = SP2518_PV_PERIOD_PIXEL_NUMS + SP2518Sensor.PreviewDummyPixels;
  //cap_line_len = SP2518_PV_PERIOD_PIXEL_NUMS + SP2518Sensor.CaptureDummyPixels;
  //shutter = (shutter * SP2518Sensor.CapturePclk) / SP2518Sensor.PreviewPclk;
  //shutter = (shutter * prev_line_len) / cap_line_len;
	//#ifdef BINNING_EN/* By sensor design */
 	//	 shutter*=2;
 	//#endif
 	SP2518WriteShutter(shutter);
  mDELAY(40);

  //SP2518WriteSensorGain(SP2518Sensor.SensorGain);	
	}
	
	SP2518SENSORDB("[SP2518]SP2518Capture exit :\n ");
	#endif
#if 0//
    //sca l e  1600*1200					   
    SP2518_write_cmos_sensor(0xfd , 0x01);		   
    SP2518_write_cmos_sensor(0x0e , 0x00);	//disable scale 
    SP2518_write_cmos_sensor(0x0f , 0x00);	//disable scale 
    SP2518_write_cmos_sensor(0xfd , 0x00);
    SP2518_write_cmos_sensor(0x2f , 0x00); //PCLK 不分频
#endif
SP2518_write_cmos_sensor(0xfd , 0x00);
SP2518_write_cmos_sensor(0x35 , 0x41);//0x41 capture ok.preview red blue neg

	return ERROR_NONE; 
}/* SP2518Capture() */

UINT32 SP2518GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	SP2518SENSORDB("[SP2518][SP2518GetResolution enter] :\n ");
	pSensorResolution->SensorPreviewWidth= SP2518_IMAGE_SENSOR_UVGA_WITDH - 4 * 10;
	pSensorResolution->SensorPreviewHeight= SP2518_IMAGE_SENSOR_UVGA_HEIGHT - 4 * 10;
	
	pSensorResolution->SensorFullWidth= SP2518_IMAGE_SENSOR_UVGA_WITDH- 4 * 10;  
	pSensorResolution->SensorFullHeight= SP2518_IMAGE_SENSOR_UVGA_HEIGHT - 4 * 10;
	
	pSensorResolution->SensorVideoWidth=SP2518_IMAGE_SENSOR_UVGA_WITDH - 4 * 10;
	pSensorResolution->SensorVideoHeight=SP2518_IMAGE_SENSOR_UVGA_HEIGHT - 4 * 10;
	SP2518SENSORDB("[SP2518][SP2518GetInfo exit] :\n ");
	return ERROR_NONE;
}	/* SP2518GetResolution() */

UINT32 SP2518GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	SP2518SENSORDB("[SP2518]SP2518GetInfo enter :\n ");
	#if defined(MT6575)
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			pSensorInfo->SensorPreviewResolutionX=SP2518_IMAGE_SENSOR_UVGA_WITDH - 4 * 10;
			pSensorInfo->SensorPreviewResolutionY=SP2518_IMAGE_SENSOR_UVGA_HEIGHT - 4 * 10;	
			break;
		default:
			pSensorInfo->SensorPreviewResolutionX=SP2518_IMAGE_SENSOR_UVGA_WITDH - 4 * 10;
			pSensorInfo->SensorPreviewResolutionY=SP2518_IMAGE_SENSOR_UVGA_HEIGHT - 4 * 10;	
			break;
	}
	#else
		pSensorInfo->SensorPreviewResolutionX= SP2518_IMAGE_SENSOR_UVGA_WITDH - 4 * 10;
		pSensorInfo->SensorPreviewResolutionY= SP2518_IMAGE_SENSOR_UVGA_HEIGHT - 4 * 20;
	#endif
	pSensorInfo->SensorFullResolutionX= SP2518_IMAGE_SENSOR_UVGA_WITDH - 4 * 10;
	pSensorInfo->SensorFullResolutionY= SP2518_IMAGE_SENSOR_UVGA_HEIGHT - 4 * 10;

	pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=10;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=1;
	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YVYU;//capture both svga uvga ok.preview uvga red blue neg
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	/*??? */
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_HIGH;//LOW;
	pSensorInfo->SensorInterruptDelayLines = 1;
	pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_PARALLEL;

	pSensorInfo->CaptureDelayFrame = 2;//1
	pSensorInfo->PreviewDelayFrame = 2; 
	pSensorInfo->VideoDelayFrame = 4; 		
	pSensorInfo->SensorMasterClockSwitch = 0; 
       pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;   		

	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
            pSensorInfo->SensorGrabStartX = 10; 
            pSensorInfo->SensorGrabStartY = 10;             
			
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:
		#if defined(MT6575)
			case MSDK_SCENARIO_ID_CAMERA_ZSD:
		#endif
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount=	3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
                    pSensorInfo->SensorGrabStartX = 20; 
                    pSensorInfo->SensorGrabStartY = 20;             
			
		break;
		default:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount=3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
            pSensorInfo->SensorGrabStartX = 10; 
            pSensorInfo->SensorGrabStartY = 10;             
			
		break;
	}

	memcpy(pSensorConfigData, &SP2518SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	
	SP2518SENSORDB("[SP2518]SP2518GetInfo exit :\n ");
	
	return ERROR_NONE;
}	/* SP2518GetInfo() */


UINT32 SP2518Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	SP2518SENSORDB("[SP2518]SP2518Control enter :\n ");
	CurrentScenarioId = ScenarioId;
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		//case MSDK_SCENARIO_ID_VIDEO_CAPTURE_MPEG4:
		#if defined(MT6575)
			case MSDK_SCENARIO_ID_CAMERA_ZSD:
				SP2518SENSORDB("[SP2518][SP2518Control]MSDK_SCENARIO_ID_CAMERA_ZSD :\n ");
		#endif
			SP2518Preview(pImageWindow, pSensorConfigData);
			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		//case MSDK_SCENARIO_ID_CAMERA_CAPTURE_MEM:				
			SP2518Capture(pImageWindow, pSensorConfigData);
			break;
		default:
		    break; 
	}
	SP2518SENSORDB("[SP2518]SP2518Control exit :\n ");
	return TRUE;
}	/* SP2518Control() */

/* [TC] YUV sensor */	

BOOL SP2518_set_param_wb(UINT16 para)
{
    switch (para)
    {
	   case AWB_MODE_OFF:
			 //SP2518_write_cmos_sensor(0xfd,0x00);
			// SP2518_write_cmos_sensor(0x32,0x05);
		 break; 					
	   case AWB_MODE_AUTO:
			 SP2518_write_cmos_sensor(0xfd,0x01);
			 SP2518_write_cmos_sensor(0x28,0xce);
			 SP2518_write_cmos_sensor(0x29,0x8a);	   
			 SP2518_write_cmos_sensor(0xfd,0x00);
			 SP2518_write_cmos_sensor(0x32,0x0d);
		 break;
	   case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy
			 SP2518_write_cmos_sensor(0xfd,0x00);
			 SP2518_write_cmos_sensor(0x32,0x05);
			 SP2518_write_cmos_sensor(0xfd,0x01);
			 SP2518_write_cmos_sensor(0x28,0xe2);
			 SP2518_write_cmos_sensor(0x29,0x82);
			 SP2518_write_cmos_sensor(0xfd,0x00);
		 break;
	   case AWB_MODE_DAYLIGHT: //sunny
			 SP2518_write_cmos_sensor(0xfd,0x00);
			 SP2518_write_cmos_sensor(0x32,0x05);
			 SP2518_write_cmos_sensor(0xfd,0x01);
			 SP2518_write_cmos_sensor(0x28,0xc1);
			 SP2518_write_cmos_sensor(0x29,0x88);
			 SP2518_write_cmos_sensor(0xfd,0x00);
		 break;
	   case AWB_MODE_INCANDESCENT: //office
			 SP2518_write_cmos_sensor(0xfd,0x00);
			 SP2518_write_cmos_sensor(0x32,0x05);
			 SP2518_write_cmos_sensor(0xfd,0x01);
			 SP2518_write_cmos_sensor(0x28,0x7b);
			 SP2518_write_cmos_sensor(0x29,0xd3);
			 SP2518_write_cmos_sensor(0xfd,0x00);
		 break;
	   case AWB_MODE_TUNGSTEN: //home
			 SP2518_write_cmos_sensor(0xfd,0x00);
			 SP2518_write_cmos_sensor(0x32,0x05);
			 SP2518_write_cmos_sensor(0xfd,0x01);
			 SP2518_write_cmos_sensor(0x28,0xae);
			 SP2518_write_cmos_sensor(0x29,0xcc);
			 SP2518_write_cmos_sensor(0xfd,0x00);
		 break;
	   case AWB_MODE_FLUORESCENT:
			 SP2518_write_cmos_sensor(0xfd,0x00);
			 SP2518_write_cmos_sensor(0x32,0x05);
			 SP2518_write_cmos_sensor(0xfd,0x01);
			 SP2518_write_cmos_sensor(0x28,0xb4);
			 SP2518_write_cmos_sensor(0x29,0xc4);
			 SP2518_write_cmos_sensor(0xfd,0x00);
	   default:
		 return FALSE;
	 }
    return TRUE;
} /* SP2518_set_param_wb */


BOOL SP2518_set_param_effect(UINT16 para)
{
    switch (para)
    {
	    case MEFFECT_OFF:
			SP2518_write_cmos_sensor(0xfd, 0x00);
			SP2518_write_cmos_sensor(0x62, 0x00);
			SP2518_write_cmos_sensor(0x63, 0x80);
			SP2518_write_cmos_sensor(0x64, 0x80);
	      break;
	    case MEFFECT_SEPIA:
			SP2518_write_cmos_sensor(0xfd, 0x00);
			SP2518_write_cmos_sensor(0x62, 0x10);
			SP2518_write_cmos_sensor(0x63, 0xb0);
			SP2518_write_cmos_sensor(0x64, 0x40);
	      break;
	    case MEFFECT_NEGATIVE://----datasheet
			SP2518_write_cmos_sensor(0xfd, 0x00);
			SP2518_write_cmos_sensor(0x62, 0x04);
			SP2518_write_cmos_sensor(0x63, 0x80);
			SP2518_write_cmos_sensor(0x64, 0x80);
	      break;
	    case MEFFECT_SEPIAGREEN://----datasheet aqua
			SP2518_write_cmos_sensor(0xfd, 0x00);
			SP2518_write_cmos_sensor(0x62, 0x10);
			SP2518_write_cmos_sensor(0x63, 0x50);
			SP2518_write_cmos_sensor(0x64, 0x50);
	      break;
	    case MEFFECT_SEPIABLUE:
			SP2518_write_cmos_sensor(0xfd, 0x00);
			SP2518_write_cmos_sensor(0x62, 0x10);
			SP2518_write_cmos_sensor(0x63, 0x80);
			SP2518_write_cmos_sensor(0x64, 0xb0);
	      break;
	    case MEFFECT_MONO: //----datasheet black & white
			SP2518_write_cmos_sensor(0xfd, 0x00);
			SP2518_write_cmos_sensor(0x62, 0x20);
			SP2518_write_cmos_sensor(0x63, 0x80);
			SP2518_write_cmos_sensor(0x64, 0x80);
	      break;
	    default:
	      return FALSE;
	  }
    return KAL_FALSE;
} /* SP2518_set_param_effect */

kal_bool   SP2518_CAM_BANDING_50HZ = KAL_FALSE;

BOOL SP2518_set_param_banding(UINT16 para)
{

    kal_uint8 banding;
	kal_uint16 temp_reg = 0;
  	kal_uint32 base_shutter = 0, max_shutter_step = 0, exposure_limitation = 0;
  	kal_uint32 line_length = 0, sensor_pixel_clock = 0;

  #if 0
	if (SP2518Sensor.IsPVmode == KAL_TRUE)
	{
		line_length = SP2518_PV_PERIOD_PIXEL_NUMS + SP2518Sensor.PreviewDummyPixels;
		exposure_limitation = SP2518_PV_PERIOD_LINE_NUMS + SP2518Sensor.PreviewDummyLines;
		sensor_pixel_clock = SP2518Sensor.PreviewPclk * 100 * 1000;
	}
	else
	{
		line_length = SP2518_FULL_PERIOD_PIXEL_NUMS + SP2518Sensor.CaptureDummyPixels;
		exposure_limitation = SP2518_FULL_PERIOD_LINE_NUMS + SP2518Sensor.CaptureDummyLines;
		sensor_pixel_clock = SP2518Sensor.CapturePclk * 100 * 1000;
	}

	line_length = line_length * 2;		/* Multiple 2 is because one YUV422 pixels need two clock. */
	
    banding = SP2518_read_cmos_sensor(0x3A05);
    switch (para)
    {
        case AE_FLICKER_MODE_50HZ:
			spin_lock(&sp2518_drv_lock);
			 SP2518_Banding_setting = AE_FLICKER_MODE_50HZ;
			 spin_unlock(&sp2518_drv_lock);
			 /* + (line_length/2) is used fot base_shutter + 0.5 */
			 //base_shutter=framerate*max exposure line/100=(plck/2/(line_len*frame_len))*max exposure line/100
			 //base_shutter = ((sensor_pixel_clock/100) + (line_length/2)) / line_length;
			 //max_shutter_step = (exposure_limitation / base_shutter);// - 1;
			 //SP2518_write_cmos_sensor(0x3a0e, max_shutter_step); 
			 //SP2518_write_cmos_sensor(0x3a09, base_shutter); 
			                            SP2518_write_cmos_sensor(0x3a05, banding&0x7f);
			 break;

        case AE_FLICKER_MODE_60HZ:			
			spin_lock(&sp2518_drv_lock);
             SP2518_Banding_setting = AE_FLICKER_MODE_60HZ;
			 spin_unlock(&sp2518_drv_lock);
			 /* + (line_length/2) is used fot base_shutter + 0.5 */
			 //base_shutter = ((sensor_pixel_clock/120) + (line_length/2)) / line_length;
		 	 //max_shutter_step = (exposure_limitation / base_shutter);// - 1;
		 	 //SP2518_write_cmos_sensor(0x3a0d, max_shutter_step); 
			 //SP2518_write_cmos_sensor(0x3a0b, base_shutter); 
			                            SP2518_write_cmos_sensor(0x3a05, banding|0x80);
			 break;

        default:
              return FALSE;
    }
#endif
	switch (para)//Ronlus
	{
		case AE_FLICKER_MODE_50HZ:
			SP2518_Banding_setting = AE_FLICKER_MODE_50HZ;
			break;
		case AE_FLICKER_MODE_60HZ:			
			SP2518_Banding_setting = AE_FLICKER_MODE_60HZ;
			break;
		default:
			return FALSE;
	}
	SP2518_night_mode(SP2518Sensor.NightMode);
    return TRUE;
} /* SP2518_set_param_banding */

BOOL SP2518_set_param_exposure(UINT16 para)
{
    kal_uint8 EvTemp0 = 0x00, EvTemp1 = 0x00, temp_reg= 0x00;
	  //temp_reg=SP2518_read_cmos_sensor(0x5083);//[3] sign
	  //SP2518_write_cmos_sensor(0x507b,SP2518_read_cmos_sensor(0x507b)|0x04);
    switch (para)
    {
	    case AE_EV_COMP_n13:              /* EV -2 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0xc0);
	      break;
	    case AE_EV_COMP_n10:              /* EV -1.5 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0xd0);
	      break;
	    case AE_EV_COMP_n07:              /* EV -1 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0xe0);
	      break;
	    case AE_EV_COMP_n03:              /* EV -0.5 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0xf0);
	      break;
	    case AE_EV_COMP_00:                /* EV 0 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0x00);
	      break;
	    case AE_EV_COMP_03:              /* EV +0.5 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0x10);
	      break;
	    case AE_EV_COMP_07:              /* EV +1 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0x20);
	      break;
	    case AE_EV_COMP_10:              /* EV +1.5 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0x30);
	      break;
	    case AE_EV_COMP_13:              /* EV +2 */
			SP2518_write_cmos_sensor(0xfd,0x00);
			SP2518_write_cmos_sensor(0xdc,0x40);
	      break;
	    default:
	      return FALSE;
	  }
    return TRUE;
} /* SP2518_set_param_exposure */



UINT32 SP2518YUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
	SP2518SENSORDB("SP2518YUVSensorSetting:iCmd=%d,iPara=%d, %d \n",iCmd, iPara);

	switch (iCmd) {
		case FID_SCENE_MODE:
			SP2518SENSORDB("Night Mode:%d\n", iPara); 
	    	if (iPara == SCENE_MODE_OFF)
	    	{
	       		SP2518_night_mode(KAL_FALSE); 
	    	}
	    	else if (iPara == SCENE_MODE_NIGHTSCENE)
	    	{
          		SP2518_night_mode(KAL_TRUE); 
	    	}	    
	    	break; 	    
		case FID_AWB_MODE:
			SP2518SENSORDB("Set AWB Mode:%d\n", iPara); 	    
        	SP2518_set_param_wb(iPara);
			break;
		case FID_COLOR_EFFECT:
	     	SP2518SENSORDB("Set Color Effect:%d\n", iPara); 	    	    
         	SP2518_set_param_effect(iPara);
		 	break;
		case FID_AE_EV:
         	SP2518SENSORDB("Set EV:%d\n", iPara); 	    	    
         	SP2518_set_param_exposure(iPara);
		 break;
		case FID_AE_FLICKER:
         	SP2518SENSORDB("Set Flicker:%d\n", iPara); 	    	    	    
         	SP2518_set_param_banding(iPara);
			SP2518_night_mode(SP2518Sensor.NightMode); //Ronlus
	        break;
		 	break;
    	case FID_AE_SCENE_MODE: 
			SP2518SENSORDB("Set AE Mode:%d\n", iPara);
         	if (iPara == AE_MODE_OFF) {
				spin_lock(&sp2518_drv_lock);
		 		SP2518_AE_ENABLE = KAL_FALSE; 
				spin_unlock(&sp2518_drv_lock);
         	}
         	else {
				spin_lock(&sp2518_drv_lock);
		 		SP2518_AE_ENABLE = KAL_TRUE; 
				spin_unlock(&sp2518_drv_lock);
	     	}
         	SP2518_set_AE_mode(SP2518_AE_ENABLE);
         	break; 
    case FID_ZOOM_FACTOR:
   		 	SP2518SENSORDB("FID_ZOOM_FACTOR:%d\n", iPara); 	    
			spin_lock(&sp2518_drv_lock);
	     	zoom_factor = iPara; 
			spin_unlock(&sp2518_drv_lock);
         	break; 
	default:
		 	break;
	}
	return TRUE;
}   /* SP2518YUVSensorSetting */

UINT32 SP2518YUVSetVideoMode(UINT16 u2FrameRate)
{
#if 0
   if (u2FrameRate == 30)
	  {
        SP2518_write_cmos_sensor(0x3003,0x80);//25fps 26mclk 
        SP2518_write_cmos_sensor(0x3004,0x10); 
        SP2518_write_cmos_sensor(0x3005,0x16); 
        SP2518_write_cmos_sensor(0x3006,0x0d); 
        //dummy lines&dummy pixels 
        SP2518_write_cmos_sensor(0x380c,0x05); 
        SP2518_write_cmos_sensor(0x380d,0x14); 
        SP2518_write_cmos_sensor(0x380e,0x02); 
        SP2518_write_cmos_sensor(0x380f,0xe3); 
        //banding                    
        SP2518_write_cmos_sensor(0x3a08,0x00); 
        SP2518_write_cmos_sensor(0x3a09,0xb9); 
        SP2518_write_cmos_sensor(0x3a0e,0x04); 
        SP2518_write_cmos_sensor(0x3a0a,0x00); 
        SP2518_write_cmos_sensor(0x3a0b,0x9a); 
        SP2518_write_cmos_sensor(0x3a0d,0x04);  
              
        SP2518_write_cmos_sensor(0x3a00,0x38); 
        SP2518_write_cmos_sensor(0x3a02,0x02); 
        SP2518_write_cmos_sensor(0x3a03,0xe3); 
        SP2518_write_cmos_sensor(0x3a14,0x02); 
        SP2518_write_cmos_sensor(0x3a15,0xe3);   
	  }
    else if (u2FrameRate == 15)   
    {
    	  SP2518_write_cmos_sensor(0x3003,0x80);//12.5fps 26mclk 
        SP2518_write_cmos_sensor(0x3004,0x20);
        SP2518_write_cmos_sensor(0x3005,0x16); 
        SP2518_write_cmos_sensor(0x3006,0x0d); 
        //dummy lines&dummy pixels 
        SP2518_write_cmos_sensor(0x380c,0x05); 
        SP2518_write_cmos_sensor(0x380d,0x14); 
        SP2518_write_cmos_sensor(0x380e,0x02); 
        SP2518_write_cmos_sensor(0x380f,0xe3); 
        //banding                   
        SP2518_write_cmos_sensor(0x3a08,0x00); 
        SP2518_write_cmos_sensor(0x3a09,0x5c); 
        SP2518_write_cmos_sensor(0x3a0e,0x08); 
        SP2518_write_cmos_sensor(0x3a0a,0x00); 
        SP2518_write_cmos_sensor(0x3a0b,0x4d); 
        SP2518_write_cmos_sensor(0x3a0d,0x09);          
                                  
        SP2518_write_cmos_sensor(0x3a00,0x38); 
        SP2518_write_cmos_sensor(0x3a02,0x02); 
        SP2518_write_cmos_sensor(0x3a03,0xe3); 
        SP2518_write_cmos_sensor(0x3a14,0x02); 
        SP2518_write_cmos_sensor(0x3a15,0xe3);   
    }   
    else 
    {
        printk("Wrong frame rate setting \n");
    } 
	#endif
    return TRUE;
}
#if defined(MT6575)
static void SP2518GetEvAwbRef(UINT32 pSensorAEAWBRefStruct)
{
	PSENSOR_AE_AWB_REF_STRUCT Ref = (PSENSOR_AE_AWB_REF_STRUCT)pSensorAEAWBRefStruct;
	#if 0
	Ref->SensorAERef.AeRefLV05Shutter=1503;
	Ref->SensorAERef.AeRefLV05Gain=496*2;
	Ref->SensorAERef.AeRefLV13Shutter=49;
	Ref->SensorAERef.AeRefLV13Gain=64*2;
	Ref->SensorAwbGainRef.AwbRefD65Rgain=188;
	Ref->SensorAwbGainRef.AwbRefD65Bgain=128;
	Ref->SensorAwbGainRef.AwbRefCWFRgain=160;
	Ref->SensorAwbGainRef.AwbRefCWFBgain=164;
	#endif
	
}

static void SP2518GetCurAeAwbInfo(UINT32 pSensorAEAWBCurStruct)
{
	PSENSOR_AE_AWB_CUR_STRUCT Info = (PSENSOR_AE_AWB_CUR_STRUCT)pSensorAEAWBCurStruct;
	#if 0
	Info->SensorAECur.AeCurShutter=SP2518ReadShutter();
	Info->SensorAECur.AeCurGain=SP2518ReadSensorGain() * 2;
	Info->SensorAwbGainCur.AwbCurRgain=SP2518_read_cmos_sensor(0x504c);
	Info->SensorAwbGainCur.AwbCurBgain=SP2518_read_cmos_sensor(0x504e);
	#endif
}
#endif

UINT32 SP2518SetMaxFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 frameRate) {
	kal_uint32 pclk;
	kal_int16 dummyLine;
	kal_uint16 lineLength,frameHeight;
		
	SP2518SENSORDB("SP2518SetMaxFramerateByScenario: scenarioId = %d, frame rate = %d\n",scenarioId,frameRate);
#if 0
	switch (scenarioId) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			pclk = 480/10;
			lineLength = SP2518_PV_PERIOD_PIXEL_NUMS;
			frameHeight = (10 * pclk)/frameRate/lineLength;
			dummyLine = frameHeight - SP2518_PV_PERIOD_LINE_NUMS;
			SP2518SENSORDB("SP2518SetMaxFramerateByScenario MSDK_SCENARIO_ID_CAMERA_PREVIEW: lineLength = %d, dummy=%d\n",lineLength,dummyLine);
			SP2518SetDummy(0, dummyLine);			
			break;			
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			pclk = 480/10;
			lineLength = SP2518_PV_PERIOD_PIXEL_NUMS;
			frameHeight = (10 * pclk)/frameRate/lineLength;
			dummyLine = frameHeight - SP2518_PV_PERIOD_LINE_NUMS;
			SP2518SENSORDB("SP2518SetMaxFramerateByScenario MSDK_SCENARIO_ID_VIDEO_PREVIEW: lineLength = %d, dummy=%d\n",lineLength,dummyLine);			
			SP2518SetDummy(0, dummyLine);			
			break;			
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:			
			pclk = 480/10;
			lineLength = SP2518_FULL_PERIOD_PIXEL_NUMS;
			frameHeight = (10 * pclk)/frameRate/lineLength;
			dummyLine = frameHeight - SP2518_FULL_PERIOD_LINE_NUMS;
			SP2518SENSORDB("SP2518SetMaxFramerateByScenario MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG: lineLength = %d, dummy=%d\n",lineLength,dummyLine);			
			SP2518SetDummy(0, dummyLine);			
			break;		
        case MSDK_SCENARIO_ID_CAMERA_3D_PREVIEW: //added
            break;
        case MSDK_SCENARIO_ID_CAMERA_3D_VIDEO:
			break;
        case MSDK_SCENARIO_ID_CAMERA_3D_CAPTURE: //added   
			break;		
		default:
			break;
	}
	#endif
	return ERROR_NONE;
}
UINT32 SP2518GetDefaultFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 *pframeRate) 
{
#if 0
	switch (scenarioId) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			 *pframeRate = 300;
			 break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 *pframeRate = 300;
			break;		
        case MSDK_SCENARIO_ID_CAMERA_3D_PREVIEW: //added
        case MSDK_SCENARIO_ID_CAMERA_3D_VIDEO:
        case MSDK_SCENARIO_ID_CAMERA_3D_CAPTURE: //added   
			 *pframeRate = 300;
			break;		
		default:
			break;
	}
#endif
	return ERROR_NONE;
}

UINT32 SP2518FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
							 UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;
	UINT32 Tony_Temp1 = 0;
	UINT32 Tony_Temp2 = 0;
	Tony_Temp1 = pFeaturePara[0];
	Tony_Temp2 = pFeaturePara[1];
	
	//SP2518SENSORDB("[SP2518]SP2518FeatureControl enter :\n ");
	SP2518SENSORDB("[SP2518][SP2518FeatureControl]feature id=%d \n",FeatureId);
	
	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_RESOLUTION \n");
			*pFeatureReturnPara16++=SP2518_IMAGE_SENSOR_UVGA_WITDH;
			*pFeatureReturnPara16=SP2518_IMAGE_SENSOR_UVGA_HEIGHT;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_GET_PERIOD:
			#if defined(MT6575)
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					SP2518SENSORDB("[SP2518][SENSOR_FEATURE_GET_PERIOD]MSDK_SCENARIO_ID_CAMERA_ZSD :\n ");
					//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_PERIOD \n");
					*pFeatureReturnPara16++=SP2518_FULL_PERIOD_PIXEL_NUMS + SP2518Sensor.CaptureDummyPixels;
					*pFeatureReturnPara16=SP2518_FULL_PERIOD_LINE_NUMS + SP2518Sensor.CaptureDummyLines;
					*pFeatureParaLen=4;
					break;
				default:
					//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_PERIOD \n");
					*pFeatureReturnPara16++=SP2518_PV_PERIOD_PIXEL_NUMS + SP2518Sensor.PreviewDummyPixels;
					*pFeatureReturnPara16=SP2518_PV_PERIOD_LINE_NUMS + SP2518Sensor.PreviewDummyLines;
					*pFeatureParaLen=4;
					break;
			}
			#else
				//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_PERIOD \n");
				*pFeatureReturnPara16++=SP2518_PV_PERIOD_PIXEL_NUMS + SP2518Sensor.PreviewDummyPixels;
				*pFeatureReturnPara16=SP2518_PV_PERIOD_LINE_NUMS + SP2518Sensor.PreviewDummyLines;
				*pFeatureParaLen=4;
			#endif
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			#if defined(MT6575)
			switch(CurrentScenarioId)
			{
				case MSDK_SCENARIO_ID_CAMERA_ZSD:
					SP2518SENSORDB("[SP2518][SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ]MSDK_SCENARIO_ID_CAMERA_ZSD :\n ");
					//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ \n");
					*pFeatureReturnPara32 = SP2518Sensor.CapturePclk * 1000 *100;	 //unit: Hz				
					*pFeatureParaLen=4;
					break;
				default:
					//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ \n");
					*pFeatureReturnPara32 = SP2518Sensor.PreviewPclk * 1000 *100;	 //unit: Hz
					*pFeatureParaLen=4;
					break;
				}
			#else
				//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ \n");
				*pFeatureReturnPara32 = SP2518Sensor.PreviewPclk * 1000 *100;	 //unit: Hz
				*pFeatureParaLen=4;
			#endif
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_ESHUTTER \n");
			break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_NIGHTMODE \n");
			SP2518_night_mode((BOOL) *pFeatureData16);
			break;
		case SENSOR_FEATURE_SET_GAIN:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_GAIN \n");
			break;
		case SENSOR_FEATURE_SET_FLASHLIGHT:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_FLASHLIGHT \n");
			break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ \n");
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_REGISTER \n");
			SP2518_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_REGISTER \n");
			pSensorRegData->RegData = SP2518_read_cmos_sensor(pSensorRegData->RegAddr);
			break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_CONFIG_PARA \n");
			memcpy(pSensorConfigData, &SP2518SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
			*pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
			break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
		case SENSOR_FEATURE_GET_CCT_REGISTER:
		case SENSOR_FEATURE_SET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_REGISTER_DEFAULT:

		case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
		case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
		case SENSOR_FEATURE_GET_GROUP_INFO:
		case SENSOR_FEATURE_GET_ITEM_INFO:
		case SENSOR_FEATURE_SET_ITEM_INFO:
		case SENSOR_FEATURE_GET_ENG_INFO:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET/get_CCT_xxxx ect \n");
			break;
		case SENSOR_FEATURE_GET_GROUP_COUNT:
            *pFeatureReturnPara32++=0;
            *pFeatureParaLen=4;	   
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_GROUP_COUNT \n");
		    break; 
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_GET_LENS_DRIVER_ID \n");
			// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			// if EEPROM does not exist in camera module.
			*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			*pFeatureParaLen=4;
			break;
		case SENSOR_FEATURE_CHECK_SENSOR_ID:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_CHECK_SENSOR_ID \n");
			SP2518_GetSensorID(pFeatureData32);
			break;
		case SENSOR_FEATURE_SET_YUV_CMD:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_YUV_CMD \n");
			SP2518YUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_SET_VIDEO_MODE:
			//SP2518SENSORDB("SP2518FeatureControl:SENSOR_FEATURE_SET_VIDEO_MODE \n");
		    SP2518YUVSetVideoMode(*pFeatureData16);
		    break; 
		#if defined(MT6575)
		case SENSOR_FEATURE_GET_EV_AWB_REF:
			SP2518GetEvAwbRef(*pFeatureData32);
			break;
		case SENSOR_FEATURE_GET_SHUTTER_GAIN_AWB_GAIN:
			SP2518GetCurAeAwbInfo(*pFeatureData32);			
			break;
		#endif
		case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
			SP2518SetMaxFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32, *(pFeatureData32+1));
			break;
		case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
			SP2518GetDefaultFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32, (MUINT32 *)(*(pFeatureData32+1)));
			break;
		default:
			SP2518SENSORDB("SP2518FeatureControl:default \n");
			break;			
	}
	//SP2518SENSORDB("[SP2518]SP2518FeatureControl exit :\n ");
	return ERROR_NONE;
}	/* SP2518FeatureControl() */


SENSOR_FUNCTION_STRUCT	SensorFuncSP2518=
{
	SP2518Open,
	SP2518GetInfo,
	SP2518GetResolution,
	SP2518FeatureControl,
	SP2518Control,
	SP2518Close
};

UINT32 SP2518_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncSP2518;

	return ERROR_NONE;
}	/* SensorInit() */


