/*******************************************************************************************/
   

/******************************************************************************************/

#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/xlog.h>
#include <asm/system.h>

#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"

#include "ar0833mipiraw_Sensor.h"
#include "ar0833mipiraw_Camera_Sensor_para.h"
#include "ar0833mipiraw_CameraCustomized.h"

static DEFINE_SPINLOCK(ar0833mipiraw_drv_lock);

#define AR0833_RUN_RAW10 

#define AR0833_DEBUG
//#define AR0833_DEBUG_SOFIA

#ifdef AR0833_DEBUG
	#define AR0833DB(fmt, arg...) xlog_printk(ANDROID_LOG_DEBUG, "[AR0833Raw] ",  fmt, ##arg)
#else
	#define AR0833DB(fmt, arg...)
#endif

#ifdef AR0833_DEBUG_SOFIA
	#define AR0833DBSOFIA(fmt, arg...) xlog_printk(ANDROID_LOG_DEBUG, "[AR0833Raw] ",  fmt, ##arg)
#else
	#define AR0833DBSOFIA(fmt, arg...)
#endif

#define mDELAY(ms)  mdelay(ms)

kal_uint32 AR0833_FeatureControl_PERIOD_PixelNum=AR0833_PV_PERIOD_PIXEL_NUMS;
kal_uint32 AR0833_FeatureControl_PERIOD_LineNum=AR0833_PV_PERIOD_LINE_NUMS;

UINT16 VIDEO_MODE_TARGET_FPS = 30;
static BOOL ReEnteyCamera = KAL_FALSE;


MSDK_SENSOR_CONFIG_STRUCT AR0833SensorConfigData;

kal_uint32 AR0833_FAC_SENSOR_REG;

MSDK_SCENARIO_ID_ENUM AR0833CurrentScenarioId = MSDK_SCENARIO_ID_CAMERA_PREVIEW;

/* FIXME: old factors and DIDNOT use now. s*/
SENSOR_REG_STRUCT AR0833SensorCCT[]=CAMERA_SENSOR_CCT_DEFAULT_VALUE;
SENSOR_REG_STRUCT AR0833SensorReg[ENGINEER_END]=CAMERA_SENSOR_REG_DEFAULT_VALUE;
/* FIXME: old factors and DIDNOT use now. e*/
struct AR0833_SENSOR_STRUCT AR0833_sensor=
{
    .i2c_write_id = 0x6c,
    .i2c_read_id  = 0x6d,
    //.pvPclk       = 1040,  
    //.capPclk      = 1040,
    //.videoPclk    = 1040,
};

static AR0833_PARA_STRUCT ar0833;

extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);


kal_uint16 AR0833_read_cmos_sensor(kal_uint32 addr)
{
    kal_uint16 get_byte=0;

    char puSendCmd[2] = {(char)(addr >> 8) , (char)(addr & 0xFF) };

    iReadRegI2C(puSendCmd , 2, (u8*)&get_byte,2,AR0833_sensor.i2c_write_id);

    return ((get_byte<<8)&0xff00)|((get_byte>>8)&0x00ff);
}



void AR0833_write_cmos_sensor(kal_uint32 addr, kal_uint32 para)
{

    char puSendCmd[4] = {(char)(addr >> 8) , (char)(addr & 0xFF) ,(char)(para >> 8),(char)(para & 0xFF)};

    iWriteRegI2C(puSendCmd , 4, AR0833_sensor.i2c_write_id);
}


kal_uint16 AR0833_read_cmos_sensor_8(kal_uint32 addr)
{
    kal_uint16 get_byte=0;
    char puSendCmd[2] = {(char)(addr >> 8) , (char)(addr & 0xFF) };
    iReadRegI2C(puSendCmd , 2, (u8*)&get_byte,1,AR0833_sensor.i2c_write_id);
    return get_byte;
}

void AR0833_write_cmos_sensor_8(kal_uint32 addr, kal_uint32 para)
{

    char puSendCmd[4] = {(char)(addr >> 8) , (char)(addr & 0xFF) ,(char)(para & 0xFF)};

    iWriteRegI2C(puSendCmd , 3, AR0833_sensor.i2c_write_id);
}


#define Sleep(ms) mdelay(ms)


/*******************************************************************************
*
********************************************************************************/

static kal_uint16 AR0833MIPIReg2Gain(const kal_uint8 iReg)
{
	kal_uint16 iGain;
	AR0833DB("[AR0833MIPIReg2Gain] iReg is :%d \n", iReg);
	
    // Range: 1x to 4x
    iGain=iReg & 0x007F;
	AR0833DB("[AR0833MIPIReg2Gain] iGain is :%d \n", iGain);
    return iGain;
}

kal_uint16 reg_gain_temp=0;
	
void AR0833MIPIGain2Reg(kal_uint16 iGain)
{
    kal_uint16 reg_gain; //iGain4,iGain5,iGain6,iGain7;
	AR0833DB("[AR0833MIPIGain2Reg1] iGain is :%d \n", iGain);

	if(iGain>=512){
		reg_gain=((iGain*2/8)<<5)|0x0007;    
		/*
		iGain*2/8  means ((kal_uint16)(((iGain/64.0)/8.0)*128.0))
		*/
		AR0833DB("[AR0833MIPIGain2Reg3] Reg_gain is :%x \n", reg_gain);
		}
	else if(iGain>=384)
    	{
		reg_gain=((iGain*2/6)<<5)|0x0006;    
		AR0833DB("[AR0833MIPIGain2Reg4] 6 Reg_gain is :%x \n", reg_gain);
		AR0833DB("[AR0833MIPIGain2Reg4] 6 Reg_gain  twice is :%x \n", ((iGain*2/6)<<5)|0x0007);
    	}
	else if(iGain>=256){
		reg_gain=((iGain*2/4)<<5)|0x0003;    
		
		AR0833DB("[AR0833MIPIGain2Reg5] iGain is :%x \n", reg_gain);

	}
	else if(iGain>=192){
		reg_gain=((iGain*2/3)<<5)|0x0002;    
		
		AR0833DB("[AR0833MIPIGain2Reg6] iGain is :%x \n", reg_gain);
	}
	else if(iGain>=128){
		reg_gain=((iGain*2/2)<<5)|0x0001;    
		
		AR0833DB("[AR0833MIPIGain2Reg7] iGain is :%x \n", reg_gain);
		}
	else if(iGain>=64){
		reg_gain=((iGain*2/1)<<5)|0x0000;    
		
		AR0833DB("[AR0833MIPIGain2Reg8] iGain is :%x \n", reg_gain);
    	}
	AR0833DB("[AR0833MIPIGain2Reg9] Reg_gain is :%x \n", reg_gain);
	if(reg_gain_temp==reg_gain)
	   return;
	else
	AR0833_write_cmos_sensor(0x305E,reg_gain);

}

/*************************************************************************
* FUNCTION
*    AR0833MIPI_SetGain
*
* DESCRIPTION
*    This function is to set global gain to sensor.
*
* PARAMETERS
*    gain : sensor global gain(base: 0x40)
*
* RETURNS
*    the actually gain set to sensor.
*
* GLOBALS AFFECTED
*
*************************************************************************/


kal_uint16 read_AR0833MIPI_gain(void)
{   
    return (kal_uint16)AR0833_read_cmos_sensor(0x305E);

}  /* read_A8141MIPI_gain */

void write_AR0833MIPI_gain(kal_uint16 gain)
{
   AR0833MIPIGain2Reg(gain);
}

void AR0833MIPI_SetGain(UINT16 iGain)
{
	AR0833DB("[AR0833MIPI_SetGain] iGain is :%d \n ",iGain);
	
    write_AR0833MIPI_gain(iGain);

}   /*  AR0833MIPI_SetGain  */

static void AR0833_SetDummy( const kal_uint32 iPixels, const kal_uint32 iLines )
{
	kal_uint32 line_length = 0;
	kal_uint32 frame_length = 0;

	if ( SENSOR_MODE_PREVIEW == ar0833.sensorMode )	//SXGA size output
	{
		line_length = AR0833_PV_PERIOD_PIXEL_NUMS + iPixels;
		frame_length = AR0833_PV_PERIOD_LINE_NUMS + iLines;
	}
	else if( SENSOR_MODE_VIDEO== ar0833.sensorMode )
	{
		line_length = AR0833_VIDEO_PERIOD_PIXEL_NUMS + iPixels;
		frame_length = AR0833_VIDEO_PERIOD_LINE_NUMS + iLines;
	}
	else//QSXGA size output
	{
		line_length = AR0833_FULL_PERIOD_PIXEL_NUMS + iPixels;
		frame_length = AR0833_FULL_PERIOD_LINE_NUMS + iLines;
	}

	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.maxExposureLines = frame_length -4;
	AR0833_FeatureControl_PERIOD_PixelNum = line_length;
	AR0833_FeatureControl_PERIOD_LineNum = frame_length;
	spin_unlock(&ar0833mipiraw_drv_lock);

	//Set total line length
	AR0833_write_cmos_sensor(0x0340, frame_length);
	AR0833_write_cmos_sensor(0x0342, line_length);

}   /*  AR0833_SetDummy */

static void AR0833_Sensor_Init(void)
{
	  AR0833DB("AR0833_Sensor_Init 2lane_OB:\n ");			

	  AR0833_write_cmos_sensor(0x301A,  0x0019);	// RESET_REGISTER
	  mdelay(100);
	  AR0833_write_cmos_sensor(0x301A,  0x0218);	 // RESET_REGISTER
	
	  AR0833_write_cmos_sensor(0x3042,	0x0000);	
	  AR0833_write_cmos_sensor(0x30C0,	0x1810);	
	  AR0833_write_cmos_sensor(0x30C8,	0x0018);	
	  AR0833_write_cmos_sensor(0x30D2,	0x0000);	 
	  AR0833_write_cmos_sensor(0x30D4,	0x3030);	
	  AR0833_write_cmos_sensor(0x30D6,	0x2200);	
	  AR0833_write_cmos_sensor(0x30DA,	0x0080);	
	  AR0833_write_cmos_sensor(0x30DC,	0x0080);	
	  AR0833_write_cmos_sensor(0x30EE,	0x0340);	
	  AR0833_write_cmos_sensor(0x316A,	0x8800);	
	  AR0833_write_cmos_sensor(0x316C,	0x8200);	
	  AR0833_write_cmos_sensor(0x316E,	0x8200);	
	  AR0833_write_cmos_sensor(0x3172,	0x0286);	
	  AR0833_write_cmos_sensor(0x3174,	0x8000);	
	  AR0833_write_cmos_sensor(0x317C,	0xE103);	
	  AR0833_write_cmos_sensor(0x3180,	0xB080);	
	  AR0833_write_cmos_sensor(0x31E0,	0x0741);	
	  AR0833_write_cmos_sensor(0x31E6,	0x0000);	
	  AR0833_write_cmos_sensor(0x3ECC,	0x0056);	
	  AR0833_write_cmos_sensor(0x3ED0,	0xA666);	
	  AR0833_write_cmos_sensor(0x3ED2,	0x6664); 
	  AR0833_write_cmos_sensor(0x3ED4,	0x6ACC);	
	  AR0833_write_cmos_sensor(0x3ED8,	0x7488);	
	  AR0833_write_cmos_sensor(0x3EDA,	0x77CB);	
	  AR0833_write_cmos_sensor(0x3EDE,	0x6664);	
	  AR0833_write_cmos_sensor(0x3EE0,	0x26D5);	
	  AR0833_write_cmos_sensor(0x3EE4,	0x3548);	
	  AR0833_write_cmos_sensor(0x3EE6,	0xB10C);	
	  AR0833_write_cmos_sensor(0x3EE8,	0x6E79);	
	  AR0833_write_cmos_sensor(0x3EEA,	0xC8B9);	
	  AR0833_write_cmos_sensor(0x3EFA,	0xA656);	
	  AR0833_write_cmos_sensor(0x3EFE,	0x99CC);	
	  AR0833_write_cmos_sensor(0x3F00,	0x0028);	
	  AR0833_write_cmos_sensor(0x3F02,	0x0140);	
	  AR0833_write_cmos_sensor(0x3F04,	0x0002);	
	  AR0833_write_cmos_sensor(0x3F06,	0x0004);	
	  AR0833_write_cmos_sensor(0x3F08,	0x0008); 
	  AR0833_write_cmos_sensor(0x3F0A,	0x0B09);	
	  AR0833_write_cmos_sensor(0x3F0C,	0x0302);	
	  AR0833_write_cmos_sensor(0x3F10,	0x0505); 
	  AR0833_write_cmos_sensor(0x3F12,	0x0303); 
	  AR0833_write_cmos_sensor(0x3F14,	0x0101); 
	  AR0833_write_cmos_sensor(0x3F16,	0x2020); 
	  AR0833_write_cmos_sensor(0x3F18,	0x0404); 
	  AR0833_write_cmos_sensor(0x3F1A,	0x7070);	
	  AR0833_write_cmos_sensor(0x3F1C,	0x003A);	
	  AR0833_write_cmos_sensor(0x3F1E,	0x003C);	
	  AR0833_write_cmos_sensor(0x3F20,	0x0209); 
	  AR0833_write_cmos_sensor(0x3F2C,	0x2210); 
	  AR0833_write_cmos_sensor(0x3F38,	0x44A8); 
	  AR0833_write_cmos_sensor(0x3F40,	0x2020); 
	  AR0833_write_cmos_sensor(0x3F42,	0x0808); 
	  AR0833_write_cmos_sensor(0x3F44,	0x0101);				
							
	  //PLL Configuration	
#ifdef AR0833_RUN_RAW10	
	  AR0833_write_cmos_sensor( 0x0300, 0x0005);		   //VT_PIX_CLK_DIV=5		  
	  AR0833_write_cmos_sensor( 0x0302, 0x0001);		   //VT_SYS_CLK_DIV=1		  
	  AR0833_write_cmos_sensor( 0x0304, 0x0004);		   //PRE_PLL_CLK_DIV=4 //Note: 24MHz/4=6MHz 		  
	  AR0833_write_cmos_sensor( 0x0306, 0x007A);		   //PLL_MULTIPLIER=112 //Note: Running at 672MHz		  
	  AR0833_write_cmos_sensor( 0x0308, 0x000A);		   //OP_PIX_CLK_DIV=10			  
	  AR0833_write_cmos_sensor( 0x030A, 0x0001);		   //OP_SYS_CLK_DIV=1		  
	  AR0833_write_cmos_sensor( 0x0112, 0x0A0A );	   // CCP_DATA_FORMAT RAW10   
	  AR0833_write_cmos_sensor( 0x3064, 0x7800);	
#endif  
	  
#ifdef AR0833_RUN_RAW8    
	  AR0833_write_cmos_sensor(0x0300, 0x0004);   // VT_PIX_CLK_DIV  
	  AR0833_write_cmos_sensor(0x0302, 0x0001);  //VT_SYS_CLK_DIV=1		  
	  AR0833_write_cmos_sensor(0x0304, 0x0004);   // PRE_PLL_CLK_DIV   
	  AR0833_write_cmos_sensor(0x0306, 0x0062);   // PLL_MULTIPLIER    
	  AR0833_write_cmos_sensor(0x0308, 0x0008);   // OP_PIX_CLK_DIV    
	  AR0833_write_cmos_sensor(0x0342, 0x0FCC);   // LINE_LENGTH_PCK   
	  AR0833_write_cmos_sensor(0x0112, 0x0808);   // CCP_DATA_FORMAT RAW8	
#endif

	  mdelay(1);								 
	  //Initialize 2-lane MIPI and Sequencer					  
	  //2-lane MIPI timings 							 
	  AR0833_write_cmos_sensor( 0x31B0, 0x0049);					  
	  AR0833_write_cmos_sensor( 0x31B2, 0x0028);					  
	  AR0833_write_cmos_sensor( 0x31B4, 0x4535);					  
	  AR0833_write_cmos_sensor( 0x31B6, 0x31D4);					  
	  AR0833_write_cmos_sensor( 0x31B8, 0x3089);					  
	  AR0833_write_cmos_sensor( 0x31BA, 0x0208);					  
	  AR0833_write_cmos_sensor( 0x31BC, 0x8007);   //Continue Clock mode for mipi, 0007 discontinue mode
	  AR0833_write_cmos_sensor( 0x31AE, 0x0202);	  //2-lane MIPI


	  //Sequencer_Adjustment_v13p13   
	  mdelay(100);
	  // @00 Jump Table
	  AR0833_write_cmos_sensor_8(0x3D00, 0x04);
	  AR0833_write_cmos_sensor_8(0x3D01, 0x71);
	  AR0833_write_cmos_sensor_8(0x3D02, 0xC9);
	  AR0833_write_cmos_sensor_8(0x3D03, 0xFF);
	  AR0833_write_cmos_sensor_8(0x3D04, 0xFF);
	  AR0833_write_cmos_sensor_8(0x3D05, 0xFF);
	  AR0833_write_cmos_sensor_8(0x3D06, 0xFF);
	  AR0833_write_cmos_sensor_8(0x3D07, 0xFF);
	  AR0833_write_cmos_sensor_8(0x3D08, 0x6F);
	  AR0833_write_cmos_sensor_8(0x3D09, 0x40);
	  AR0833_write_cmos_sensor_8(0x3D0A, 0x14);
	  AR0833_write_cmos_sensor_8(0x3D0B, 0x0E);
	  AR0833_write_cmos_sensor_8(0x3D0C, 0x23);
	  AR0833_write_cmos_sensor_8(0x3D0D, 0xC2);
	  AR0833_write_cmos_sensor_8(0x3D0E, 0x41);
	  AR0833_write_cmos_sensor_8(0x3D0F, 0x20);
	  AR0833_write_cmos_sensor_8(0x3D10, 0x30);
	  AR0833_write_cmos_sensor_8(0x3D11, 0x54);
	  AR0833_write_cmos_sensor_8(0x3D12, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D13, 0x42);
	  AR0833_write_cmos_sensor_8(0x3D14, 0x00);
	  AR0833_write_cmos_sensor_8(0x3D15, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3D16, 0x83);
	  AR0833_write_cmos_sensor_8(0x3D17, 0x57);
	  AR0833_write_cmos_sensor_8(0x3D18, 0x84);
	  AR0833_write_cmos_sensor_8(0x3D19, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D1A, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D1B, 0x55);
	  AR0833_write_cmos_sensor_8(0x3D1C, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D1D, 0x23);
	  AR0833_write_cmos_sensor_8(0x3D1E, 0x00);
	  AR0833_write_cmos_sensor_8(0x3D1F, 0x65);
	  AR0833_write_cmos_sensor_8(0x3D20, 0x65);
	  AR0833_write_cmos_sensor_8(0x3D21, 0x82);
	  AR0833_write_cmos_sensor_8(0x3D22, 0x00);
	  AR0833_write_cmos_sensor_8(0x3D23, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3D24, 0x6E);
	  AR0833_write_cmos_sensor_8(0x3D25, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D26, 0x50);
	  AR0833_write_cmos_sensor_8(0x3D27, 0x51);
	  AR0833_write_cmos_sensor_8(0x3D28, 0x83);
	  AR0833_write_cmos_sensor_8(0x3D29, 0x42);
	  AR0833_write_cmos_sensor_8(0x3D2A, 0x83);
	  AR0833_write_cmos_sensor_8(0x3D2B, 0x58);
	  AR0833_write_cmos_sensor_8(0x3D2C, 0x6E);
	  AR0833_write_cmos_sensor_8(0x3D2D, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D2E, 0x5F);
	  AR0833_write_cmos_sensor_8(0x3D2F, 0x87);
	  AR0833_write_cmos_sensor_8(0x3D30, 0x63);
	  AR0833_write_cmos_sensor_8(0x3D31, 0x82);
	  AR0833_write_cmos_sensor_8(0x3D32, 0x5B);
	  AR0833_write_cmos_sensor_8(0x3D33, 0x82);
	  AR0833_write_cmos_sensor_8(0x3D34, 0x59);
	  AR0833_write_cmos_sensor_8(0x3D35, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D36, 0x5A);
	  AR0833_write_cmos_sensor_8(0x3D37, 0x5E);
	  AR0833_write_cmos_sensor_8(0x3D38, 0xBD);
	  AR0833_write_cmos_sensor_8(0x3D39, 0x59);
	  AR0833_write_cmos_sensor_8(0x3D3A, 0x59);
	  AR0833_write_cmos_sensor_8(0x3D3B, 0x9D);
	  AR0833_write_cmos_sensor_8(0x3D3C, 0x6C);
	  AR0833_write_cmos_sensor_8(0x3D3D, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D3E, 0x6D);
	  AR0833_write_cmos_sensor_8(0x3D3F, 0xA3);
	  AR0833_write_cmos_sensor_8(0x3D40, 0x50);
	  AR0833_write_cmos_sensor_8(0x3D41, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D42, 0x51);
	  AR0833_write_cmos_sensor_8(0x3D43, 0x82);
	  AR0833_write_cmos_sensor_8(0x3D44, 0x58);
	  AR0833_write_cmos_sensor_8(0x3D45, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D46, 0x66);
	  AR0833_write_cmos_sensor_8(0x3D47, 0x83);
	  AR0833_write_cmos_sensor_8(0x3D48, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D49, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D4A, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D4B, 0x30);
	  AR0833_write_cmos_sensor_8(0x3D4C, 0x50);
	  AR0833_write_cmos_sensor_8(0x3D4D, 0xDC);
	  AR0833_write_cmos_sensor_8(0x3D4E, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3D4F, 0x83);
	  AR0833_write_cmos_sensor_8(0x3D50, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3D51, 0xAA);
	  AR0833_write_cmos_sensor_8(0x3D52, 0x30);
	  AR0833_write_cmos_sensor_8(0x3D53, 0x94);
	  AR0833_write_cmos_sensor_8(0x3D54, 0x67);
	  AR0833_write_cmos_sensor_8(0x3D55, 0x84);
	  AR0833_write_cmos_sensor_8(0x3D56, 0x65);
	  AR0833_write_cmos_sensor_8(0x3D57, 0x65);
	  AR0833_write_cmos_sensor_8(0x3D58, 0x81);
	  AR0833_write_cmos_sensor_8(0x3D59, 0x4D);
	  AR0833_write_cmos_sensor_8(0x3D5A, 0x68);
	  AR0833_write_cmos_sensor_8(0x3D5B, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3D5C, 0xAC);
	  AR0833_write_cmos_sensor_8(0x3D5D, 0x06);
	  AR0833_write_cmos_sensor_8(0x3D5E, 0x08);
	  AR0833_write_cmos_sensor_8(0x3D5F, 0x8D);
	  AR0833_write_cmos_sensor_8(0x3D60, 0x45);
	  AR0833_write_cmos_sensor_8(0x3D61, 0x96);
	  AR0833_write_cmos_sensor_8(0x3D62, 0x45);
	  AR0833_write_cmos_sensor_8(0x3D63, 0x85);
	  AR0833_write_cmos_sensor_8(0x3D64, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3D65, 0x83);
	  AR0833_write_cmos_sensor_8(0x3D66, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3D67, 0x06);
	  AR0833_write_cmos_sensor_8(0x3D68, 0x08);
	  AR0833_write_cmos_sensor_8(0x3D69, 0xA9);
	  AR0833_write_cmos_sensor_8(0x3D6A, 0x30);
	  AR0833_write_cmos_sensor_8(0x3D6B, 0x90);
	  AR0833_write_cmos_sensor_8(0x3D6C, 0x67);
	  AR0833_write_cmos_sensor_8(0x3D6D, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D6E, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D6F, 0x89);
	  AR0833_write_cmos_sensor_8(0x3D70, 0x65);
	  AR0833_write_cmos_sensor_8(0x3D71, 0x65);
	  AR0833_write_cmos_sensor_8(0x3D72, 0x81);
	  AR0833_write_cmos_sensor_8(0x3D73, 0x58);
	  AR0833_write_cmos_sensor_8(0x3D74, 0x88);
	  AR0833_write_cmos_sensor_8(0x3D75, 0x10);
	  AR0833_write_cmos_sensor_8(0x3D76, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3D77, 0xB1);
	  AR0833_write_cmos_sensor_8(0x3D78, 0x5E);
	  AR0833_write_cmos_sensor_8(0x3D79, 0x96);
	  AR0833_write_cmos_sensor_8(0x3D7A, 0x53);
	  AR0833_write_cmos_sensor_8(0x3D7B, 0x82);
	  AR0833_write_cmos_sensor_8(0x3D7C, 0x5E);
	  AR0833_write_cmos_sensor_8(0x3D7D, 0x52);
	  AR0833_write_cmos_sensor_8(0x3D7E, 0x66);
	  AR0833_write_cmos_sensor_8(0x3D7F, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D80, 0x58);
	  AR0833_write_cmos_sensor_8(0x3D81, 0x83);
	  AR0833_write_cmos_sensor_8(0x3D82, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D83, 0x64);
	  AR0833_write_cmos_sensor_8(0x3D84, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D85, 0x5B);
	  AR0833_write_cmos_sensor_8(0x3D86, 0x81);
	  AR0833_write_cmos_sensor_8(0x3D87, 0x5A);
	  AR0833_write_cmos_sensor_8(0x3D88, 0x1D);
	  AR0833_write_cmos_sensor_8(0x3D89, 0x0C);
	  AR0833_write_cmos_sensor_8(0x3D8A, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D8B, 0x55);
	  AR0833_write_cmos_sensor_8(0x3D8C, 0x30);
	  AR0833_write_cmos_sensor_8(0x3D8D, 0x60);
	  AR0833_write_cmos_sensor_8(0x3D8E, 0x41);
	  AR0833_write_cmos_sensor_8(0x3D8F, 0x82);
	  AR0833_write_cmos_sensor_8(0x3D90, 0x42);
	  AR0833_write_cmos_sensor_8(0x3D91, 0xB2);
	  AR0833_write_cmos_sensor_8(0x3D92, 0x42);
	  AR0833_write_cmos_sensor_8(0x3D93, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D94, 0x40);
	  AR0833_write_cmos_sensor_8(0x3D95, 0x81);
	  AR0833_write_cmos_sensor_8(0x3D96, 0x40);
	  AR0833_write_cmos_sensor_8(0x3D97, 0x89);
	  AR0833_write_cmos_sensor_8(0x3D98, 0x06);
	  AR0833_write_cmos_sensor_8(0x3D99, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3D9A, 0x41);
	  AR0833_write_cmos_sensor_8(0x3D9B, 0x80);
	  AR0833_write_cmos_sensor_8(0x3D9C, 0x42);
	  AR0833_write_cmos_sensor_8(0x3D9D, 0x85);
	  AR0833_write_cmos_sensor_8(0x3D9E, 0x44);
	  AR0833_write_cmos_sensor_8(0x3D9F, 0x83);
	  AR0833_write_cmos_sensor_8(0x3DA0, 0x43);
	  AR0833_write_cmos_sensor_8(0x3DA1, 0x82);
	  AR0833_write_cmos_sensor_8(0x3DA2, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3DA3, 0x83);
	  AR0833_write_cmos_sensor_8(0x3DA4, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3DA5, 0x8D);
	  AR0833_write_cmos_sensor_8(0x3DA6, 0x43);
	  AR0833_write_cmos_sensor_8(0x3DA7, 0x83);
	  AR0833_write_cmos_sensor_8(0x3DA8, 0x44);
	  AR0833_write_cmos_sensor_8(0x3DA9, 0x81);
	  AR0833_write_cmos_sensor_8(0x3DAA, 0x41);
	  AR0833_write_cmos_sensor_8(0x3DAB, 0x85);
	  AR0833_write_cmos_sensor_8(0x3DAC, 0x06);
	  AR0833_write_cmos_sensor_8(0x3DAD, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3DAE, 0x8C);
	  AR0833_write_cmos_sensor_8(0x3DAF, 0x30);
	  AR0833_write_cmos_sensor_8(0x3DB0, 0xA4);
	  AR0833_write_cmos_sensor_8(0x3DB1, 0x67);
	  AR0833_write_cmos_sensor_8(0x3DB2, 0x81);
	  AR0833_write_cmos_sensor_8(0x3DB3, 0x42);
	  AR0833_write_cmos_sensor_8(0x3DB4, 0x82);
	  AR0833_write_cmos_sensor_8(0x3DB5, 0x65);
	  AR0833_write_cmos_sensor_8(0x3DB6, 0x65);
	  AR0833_write_cmos_sensor_8(0x3DB7, 0x81);
	  AR0833_write_cmos_sensor_8(0x3DB8, 0x69);
	  AR0833_write_cmos_sensor_8(0x3DB9, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3DBA, 0x96);
	  AR0833_write_cmos_sensor_8(0x3DBB, 0x40);
	  AR0833_write_cmos_sensor_8(0x3DBC, 0x82);
	  AR0833_write_cmos_sensor_8(0x3DBD, 0x40);
	  AR0833_write_cmos_sensor_8(0x3DBE, 0x89);
	  AR0833_write_cmos_sensor_8(0x3DBF, 0x06);
	  AR0833_write_cmos_sensor_8(0x3DC0, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3DC1, 0x41);
	  AR0833_write_cmos_sensor_8(0x3DC2, 0x80);
	  AR0833_write_cmos_sensor_8(0x3DC3, 0x42);
	  AR0833_write_cmos_sensor_8(0x3DC4, 0x85);
	  AR0833_write_cmos_sensor_8(0x3DC5, 0x44);
	  AR0833_write_cmos_sensor_8(0x3DC6, 0x83);
	  AR0833_write_cmos_sensor_8(0x3DC7, 0x43);
	  AR0833_write_cmos_sensor_8(0x3DC8, 0x92);
	  AR0833_write_cmos_sensor_8(0x3DC9, 0x43);
	  AR0833_write_cmos_sensor_8(0x3DCA, 0x83);
	  AR0833_write_cmos_sensor_8(0x3DCB, 0x44);
	  AR0833_write_cmos_sensor_8(0x3DCC, 0x85);
	  AR0833_write_cmos_sensor_8(0x3DCD, 0x41);
	  AR0833_write_cmos_sensor_8(0x3DCE, 0x81);
	  AR0833_write_cmos_sensor_8(0x3DCF, 0x06);
	  AR0833_write_cmos_sensor_8(0x3DD0, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3DD1, 0x81);
	  AR0833_write_cmos_sensor_8(0x3DD2, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3DD3, 0x83);
	  AR0833_write_cmos_sensor_8(0x3DD4, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3DD5, 0x82);
	  AR0833_write_cmos_sensor_8(0x3DD6, 0x42);
	  AR0833_write_cmos_sensor_8(0x3DD7, 0xA0);
	  AR0833_write_cmos_sensor_8(0x3DD8, 0x40);
	  AR0833_write_cmos_sensor_8(0x3DD9, 0x84);
	  AR0833_write_cmos_sensor_8(0x3DDA, 0x38);
	  AR0833_write_cmos_sensor_8(0x3DDB, 0xA8);
	  AR0833_write_cmos_sensor_8(0x3DDC, 0x33);
	  AR0833_write_cmos_sensor_8(0x3DDD, 0x00);
	  AR0833_write_cmos_sensor_8(0x3DDE, 0x28);
	  AR0833_write_cmos_sensor_8(0x3DDF, 0x30);
	  AR0833_write_cmos_sensor_8(0x3DE0, 0x70);
	  AR0833_write_cmos_sensor_8(0x3DE1, 0x00);
	  AR0833_write_cmos_sensor_8(0x3DE2, 0x6F);
	  AR0833_write_cmos_sensor_8(0x3DE3, 0x40);
	  AR0833_write_cmos_sensor_8(0x3DE4, 0x14);
	  AR0833_write_cmos_sensor_8(0x3DE5, 0x0E);
	  AR0833_write_cmos_sensor_8(0x3DE6, 0x23);
	  AR0833_write_cmos_sensor_8(0x3DE7, 0xC2);
	  AR0833_write_cmos_sensor_8(0x3DE8, 0x41);
	  AR0833_write_cmos_sensor_8(0x3DE9, 0x82);
	  AR0833_write_cmos_sensor_8(0x3DEA, 0x42);
	  AR0833_write_cmos_sensor_8(0x3DEB, 0x00);
	  AR0833_write_cmos_sensor_8(0x3DEC, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3DED, 0x5D);
	  AR0833_write_cmos_sensor_8(0x3DEE, 0x80);
	  AR0833_write_cmos_sensor_8(0x3DEF, 0x5A);
	  AR0833_write_cmos_sensor_8(0x3DF0, 0x80);
	  AR0833_write_cmos_sensor_8(0x3DF1, 0x57);
	  AR0833_write_cmos_sensor_8(0x3DF2, 0x84);
	  AR0833_write_cmos_sensor_8(0x3DF3, 0x64);
	  AR0833_write_cmos_sensor_8(0x3DF4, 0x80);
	  AR0833_write_cmos_sensor_8(0x3DF5, 0x55);
	  AR0833_write_cmos_sensor_8(0x3DF6, 0x86);
	  AR0833_write_cmos_sensor_8(0x3DF7, 0x64);
	  AR0833_write_cmos_sensor_8(0x3DF8, 0x80);
	  AR0833_write_cmos_sensor_8(0x3DF9, 0x65);
	  AR0833_write_cmos_sensor_8(0x3DFA, 0x88);
	  AR0833_write_cmos_sensor_8(0x3DFB, 0x65);
	  AR0833_write_cmos_sensor_8(0x3DFC, 0x82);
	  AR0833_write_cmos_sensor_8(0x3DFD, 0x54);
	  AR0833_write_cmos_sensor_8(0x3DFE, 0x80);
	  AR0833_write_cmos_sensor_8(0x3DFF, 0x58);
	  AR0833_write_cmos_sensor_8(0x3E00, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E01, 0x00);
	  AR0833_write_cmos_sensor_8(0x3E02, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3E03, 0x86);
	  AR0833_write_cmos_sensor_8(0x3E04, 0x42);
	  AR0833_write_cmos_sensor_8(0x3E05, 0x82);
	  AR0833_write_cmos_sensor_8(0x3E06, 0x10);
	  AR0833_write_cmos_sensor_8(0x3E07, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E08, 0x9C);
	  AR0833_write_cmos_sensor_8(0x3E09, 0x5C);
	  AR0833_write_cmos_sensor_8(0x3E0A, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E0B, 0x6E);
	  AR0833_write_cmos_sensor_8(0x3E0C, 0x86);
	  AR0833_write_cmos_sensor_8(0x3E0D, 0x5B);
	  AR0833_write_cmos_sensor_8(0x3E0E, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E0F, 0x63);
	  AR0833_write_cmos_sensor_8(0x3E10, 0x9E);
	  AR0833_write_cmos_sensor_8(0x3E11, 0x59);
	  AR0833_write_cmos_sensor_8(0x3E12, 0x8C);
	  AR0833_write_cmos_sensor_8(0x3E13, 0x5E);
	  AR0833_write_cmos_sensor_8(0x3E14, 0x8A);
	  AR0833_write_cmos_sensor_8(0x3E15, 0x6C);
	  AR0833_write_cmos_sensor_8(0x3E16, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E17, 0x6D);
	  AR0833_write_cmos_sensor_8(0x3E18, 0x81);
	  AR0833_write_cmos_sensor_8(0x3E19, 0x5F);
	  AR0833_write_cmos_sensor_8(0x3E1A, 0x60);
	  AR0833_write_cmos_sensor_8(0x3E1B, 0x61);
	  AR0833_write_cmos_sensor_8(0x3E1C, 0x88);
	  AR0833_write_cmos_sensor_8(0x3E1D, 0x10);
	  AR0833_write_cmos_sensor_8(0x3E1E, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E1F, 0x66);
	  AR0833_write_cmos_sensor_8(0x3E20, 0x83);
	  AR0833_write_cmos_sensor_8(0x3E21, 0x6E);
	  AR0833_write_cmos_sensor_8(0x3E22, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E23, 0x64);
	  AR0833_write_cmos_sensor_8(0x3E24, 0x87);
	  AR0833_write_cmos_sensor_8(0x3E25, 0x64);
	  AR0833_write_cmos_sensor_8(0x3E26, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E27, 0x50);
	  AR0833_write_cmos_sensor_8(0x3E28, 0xD3);
	  AR0833_write_cmos_sensor_8(0x3E29, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3E2A, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3E2B, 0xAD);
	  AR0833_write_cmos_sensor_8(0x3E2C, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E2D, 0x94);
	  AR0833_write_cmos_sensor_8(0x3E2E, 0x67);
	  AR0833_write_cmos_sensor_8(0x3E2F, 0x84);
	  AR0833_write_cmos_sensor_8(0x3E30, 0x65);
	  AR0833_write_cmos_sensor_8(0x3E31, 0x82);
	  AR0833_write_cmos_sensor_8(0x3E32, 0x4D);
	  AR0833_write_cmos_sensor_8(0x3E33, 0x83);
	  AR0833_write_cmos_sensor_8(0x3E34, 0x65);
	  AR0833_write_cmos_sensor_8(0x3E35, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E36, 0x50);
	  AR0833_write_cmos_sensor_8(0x3E37, 0xA7);
	  AR0833_write_cmos_sensor_8(0x3E38, 0x43);
	  AR0833_write_cmos_sensor_8(0x3E39, 0x06);
	  AR0833_write_cmos_sensor_8(0x3E3A, 0x00);
	  AR0833_write_cmos_sensor_8(0x3E3B, 0x8D);
	  AR0833_write_cmos_sensor_8(0x3E3C, 0x45);
	  AR0833_write_cmos_sensor_8(0x3E3D, 0x9A);
	  AR0833_write_cmos_sensor_8(0x3E3E, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3E3F, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3E40, 0x45);
	  AR0833_write_cmos_sensor_8(0x3E41, 0x85);
	  AR0833_write_cmos_sensor_8(0x3E42, 0x06);
	  AR0833_write_cmos_sensor_8(0x3E43, 0x00);
	  AR0833_write_cmos_sensor_8(0x3E44, 0x81);
	  AR0833_write_cmos_sensor_8(0x3E45, 0x43);
	  AR0833_write_cmos_sensor_8(0x3E46, 0x8A);
	  AR0833_write_cmos_sensor_8(0x3E47, 0x6F);
	  AR0833_write_cmos_sensor_8(0x3E48, 0x96);
	  AR0833_write_cmos_sensor_8(0x3E49, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E4A, 0x90);
	  AR0833_write_cmos_sensor_8(0x3E4B, 0x67);
	  AR0833_write_cmos_sensor_8(0x3E4C, 0x64);
	  AR0833_write_cmos_sensor_8(0x3E4D, 0x88);
	  AR0833_write_cmos_sensor_8(0x3E4E, 0x64);
	  AR0833_write_cmos_sensor_8(0x3E4F, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E50, 0x65);
	  AR0833_write_cmos_sensor_8(0x3E51, 0x82);
	  AR0833_write_cmos_sensor_8(0x3E52, 0x10);
	  AR0833_write_cmos_sensor_8(0x3E53, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3E54, 0x84);
	  AR0833_write_cmos_sensor_8(0x3E55, 0x65);
	  AR0833_write_cmos_sensor_8(0x3E56, 0xEF);
	  AR0833_write_cmos_sensor_8(0x3E57, 0x10);
	  AR0833_write_cmos_sensor_8(0x3E58, 0xC0);
	  AR0833_write_cmos_sensor_8(0x3E59, 0x66);
	  AR0833_write_cmos_sensor_8(0x3E5A, 0x85);
	  AR0833_write_cmos_sensor_8(0x3E5B, 0x64);
	  AR0833_write_cmos_sensor_8(0x3E5C, 0x81);
	  AR0833_write_cmos_sensor_8(0x3E5D, 0x17);
	  AR0833_write_cmos_sensor_8(0x3E5E, 0x00);
	  AR0833_write_cmos_sensor_8(0x3E5F, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E60, 0x20);
	  AR0833_write_cmos_sensor_8(0x3E61, 0x0D);
	  AR0833_write_cmos_sensor_8(0x3E62, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E63, 0x18);
	  AR0833_write_cmos_sensor_8(0x3E64, 0x0C);
	  AR0833_write_cmos_sensor_8(0x3E65, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E66, 0x64);
	  AR0833_write_cmos_sensor_8(0x3E67, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E68, 0x60);
	  AR0833_write_cmos_sensor_8(0x3E69, 0x41);
	  AR0833_write_cmos_sensor_8(0x3E6A, 0x82);
	  AR0833_write_cmos_sensor_8(0x3E6B, 0x42);
	  AR0833_write_cmos_sensor_8(0x3E6C, 0xB2);
	  AR0833_write_cmos_sensor_8(0x3E6D, 0x42);
	  AR0833_write_cmos_sensor_8(0x3E6E, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E6F, 0x40);
	  AR0833_write_cmos_sensor_8(0x3E70, 0x82);
	  AR0833_write_cmos_sensor_8(0x3E71, 0x40);
	  AR0833_write_cmos_sensor_8(0x3E72, 0x4C);
	  AR0833_write_cmos_sensor_8(0x3E73, 0x45);
	  AR0833_write_cmos_sensor_8(0x3E74, 0x92);
	  AR0833_write_cmos_sensor_8(0x3E75, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3E76, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3E77, 0x9B);
	  AR0833_write_cmos_sensor_8(0x3E78, 0x45);
	  AR0833_write_cmos_sensor_8(0x3E79, 0x81);
	  AR0833_write_cmos_sensor_8(0x3E7A, 0x4C);
	  AR0833_write_cmos_sensor_8(0x3E7B, 0x40);
	  AR0833_write_cmos_sensor_8(0x3E7C, 0x8C);
	  AR0833_write_cmos_sensor_8(0x3E7D, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E7E, 0xA4);
	  AR0833_write_cmos_sensor_8(0x3E7F, 0x67);
	  AR0833_write_cmos_sensor_8(0x3E80, 0x85);
	  AR0833_write_cmos_sensor_8(0x3E81, 0x65);
	  AR0833_write_cmos_sensor_8(0x3E82, 0x87);
	  AR0833_write_cmos_sensor_8(0x3E83, 0x65);
	  AR0833_write_cmos_sensor_8(0x3E84, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E85, 0x60);
	  AR0833_write_cmos_sensor_8(0x3E86, 0xD3);
	  AR0833_write_cmos_sensor_8(0x3E87, 0x6A);
	  AR0833_write_cmos_sensor_8(0x3E88, 0x6B);
	  AR0833_write_cmos_sensor_8(0x3E89, 0xAC);
	  AR0833_write_cmos_sensor_8(0x3E8A, 0x6C);
	  AR0833_write_cmos_sensor_8(0x3E8B, 0x32);
	  AR0833_write_cmos_sensor_8(0x3E8C, 0xA8);
	  AR0833_write_cmos_sensor_8(0x3E8D, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E8E, 0x28);
	  AR0833_write_cmos_sensor_8(0x3E8F, 0x30);
	  AR0833_write_cmos_sensor_8(0x3E90, 0x70);
	  AR0833_write_cmos_sensor_8(0x3E91, 0x00);
	  AR0833_write_cmos_sensor_8(0x3E92, 0x80);
	  AR0833_write_cmos_sensor_8(0x3E93, 0x40);
	  AR0833_write_cmos_sensor_8(0x3E94, 0x4C);
	  AR0833_write_cmos_sensor_8(0x3E95, 0xBD);
	  AR0833_write_cmos_sensor_8(0x3E96, 0x00);
	  AR0833_write_cmos_sensor_8(0x3E97, 0x0E);
	  AR0833_write_cmos_sensor_8(0x3E98, 0xBE);
	  AR0833_write_cmos_sensor_8(0x3E99, 0x44);
	  AR0833_write_cmos_sensor_8(0x3E9A, 0x88);
	  AR0833_write_cmos_sensor_8(0x3E9B, 0x44);
	  AR0833_write_cmos_sensor_8(0x3E9C, 0xBC);
	  AR0833_write_cmos_sensor_8(0x3E9D, 0x78);
	  AR0833_write_cmos_sensor_8(0x3E9E, 0x09);
	  AR0833_write_cmos_sensor_8(0x3E9F, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EA0, 0x89);
	  AR0833_write_cmos_sensor_8(0x3EA1, 0x04);
	  AR0833_write_cmos_sensor_8(0x3EA2, 0x80);
	  AR0833_write_cmos_sensor_8(0x3EA3, 0x80);
	  AR0833_write_cmos_sensor_8(0x3EA4, 0x02);
	  AR0833_write_cmos_sensor_8(0x3EA5, 0x40);
	  AR0833_write_cmos_sensor_8(0x3EA6, 0x86);
	  AR0833_write_cmos_sensor_8(0x3EA7, 0x09);
	  AR0833_write_cmos_sensor_8(0x3EA8, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EA9, 0x8E);
	  AR0833_write_cmos_sensor_8(0x3EAA, 0x09);
	  AR0833_write_cmos_sensor_8(0x3EAB, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EAC, 0x80);
	  AR0833_write_cmos_sensor_8(0x3EAD, 0x02);
	  AR0833_write_cmos_sensor_8(0x3EAE, 0x40);
	  AR0833_write_cmos_sensor_8(0x3EAF, 0x80);
	  AR0833_write_cmos_sensor_8(0x3EB0, 0x04);
	  AR0833_write_cmos_sensor_8(0x3EB1, 0x80);
	  AR0833_write_cmos_sensor_8(0x3EB2, 0x88);
	  AR0833_write_cmos_sensor_8(0x3EB3, 0x7D);
	  AR0833_write_cmos_sensor_8(0x3EB4, 0xA0);
	  AR0833_write_cmos_sensor_8(0x3EB5, 0x86);
	  AR0833_write_cmos_sensor_8(0x3EB6, 0x09);
	  AR0833_write_cmos_sensor_8(0x3EB7, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EB8, 0x87);
	  AR0833_write_cmos_sensor_8(0x3EB9, 0x7A);
	  AR0833_write_cmos_sensor_8(0x3EBA, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EBB, 0x0E);
	  AR0833_write_cmos_sensor_8(0x3EBC, 0xC3);
	  AR0833_write_cmos_sensor_8(0x3EBD, 0x79);
	  AR0833_write_cmos_sensor_8(0x3EBE, 0x4C);
	  AR0833_write_cmos_sensor_8(0x3EBF, 0x40);
	  AR0833_write_cmos_sensor_8(0x3EC0, 0xBF);
	  AR0833_write_cmos_sensor_8(0x3EC1, 0x70);
	  AR0833_write_cmos_sensor_8(0x3EC2, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EC3, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EC4, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EC5, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EC6, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EC7, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EC8, 0x00);
	  AR0833_write_cmos_sensor_8(0x3EC9, 0x00);
	  AR0833_write_cmos_sensor_8(0x3ECA, 0x00);
	  AR0833_write_cmos_sensor_8(0x3ECB, 0x00);
	
											 
	  AR0833_write_cmos_sensor(0x0342, 0xECE);			//LINE_LENGTH_PCK 6632			
	  AR0833_write_cmos_sensor(0x0340, 0x0A0F);  // FRAME_LENGTH_LINES
	  AR0833_write_cmos_sensor(0x0202, 0x0A0F);  // COARSE_INTEGRATION_TIME
	  AR0833_write_cmos_sensor(0x0344, 0x0008);  // X_ADDR_START
	  AR0833_write_cmos_sensor(0x0348, 0x0CC7);  // X_ADDR_END
	  AR0833_write_cmos_sensor(0x0346, 0x0008);  // Y_ADDR_START
	  AR0833_write_cmos_sensor(0x034A, 0x0997);  // Y_ADDR_END
	  AR0833_write_cmos_sensor(0x034C, 0x0650);  // X_OUTPUT_SIZE
	  AR0833_write_cmos_sensor(0x034E, 0x04BC);  // Y_OUTPUT_SIZE
	  AR0833_write_cmos_sensor(0x3040, 0x4041);  // READ_MODE
	  AR0833_write_cmos_sensor(0x0400, 0x0002);  // SCALING_MODE
	  AR0833_write_cmos_sensor(0x0402, 0x0000);  // SPATIAL_SAMPLING
	  AR0833_write_cmos_sensor(0x0404, 0x0020);  // SCALE_M
	  AR0833_write_cmos_sensor(0x0408, 0x0208);  // SECOND_RESIDUAL
	  AR0833_write_cmos_sensor(0x040A, 0x00C7);  // SECOND_CROP
	  AR0833_write_cmos_sensor(0x306E, 0x9090);  // DATA_PATH_SELECT
											
	  AR0833_write_cmos_sensor(0x301A, 0x001C);  // RESET_REGISTER
	  
	
	  AR0833_write_cmos_sensor(0x30F4, 0x0000);   // vcm_step_time (R/W)
      AR0833_write_cmos_sensor(0x30F0, 0x8000);   // vcm_control (R/W)

      AR0833DB("AR0833_Sensor_Init exit :\n ");
}   /*  AR0833_Sensor_Init  */

void AR0833PreviewSetting(void)
{
    AR0833DB("AR0833Preview setting:");
    AR0833DB("Func_RSE1616x1212_30FPS_MCLK24_2LANE_RAW10");

	AR0833_write_cmos_sensor(0x301A, 0x0218);   // RESET_REGISTER
                                        
    AR0833_write_cmos_sensor(0x0342, 0x0ECE);   // LINE_LENGTH_PCK
    AR0833_write_cmos_sensor(0x0340, 0x0A0F);   // FRAME_LENGTH_LINES
    AR0833_write_cmos_sensor(0x0202, 0x0A0F);   // COARSE_INTEGRATION_TIME
    AR0833_write_cmos_sensor(0x0344, 0x0008);   // X_ADDR_START
    AR0833_write_cmos_sensor(0x0348, 0x0CC7);   // X_ADDR_END
    AR0833_write_cmos_sensor(0x0346, 0x0008);   // Y_ADDR_START
    AR0833_write_cmos_sensor(0x034A, 0x0997);   // Y_ADDR_END
    AR0833_write_cmos_sensor(0x034C, 0x0650);   // X_OUTPUT_SIZE
    AR0833_write_cmos_sensor(0x034E, 0x04BC);   // Y_OUTPUT_SIZE
    AR0833_write_cmos_sensor(0x3040, 0x4041);   // READ_MODE
    AR0833_write_cmos_sensor(0x0400, 0x0002);   // SCALING_MODE
    AR0833_write_cmos_sensor(0x0402, 0x0000);   // SPATIAL_SAMPLING
    AR0833_write_cmos_sensor(0x0404, 0x0020);   // SCALE_M
    AR0833_write_cmos_sensor(0x0408, 0x0208);   // SECOND_RESIDUAL
    AR0833_write_cmos_sensor(0x040A, 0x00C7);   // SECOND_CROP
    AR0833_write_cmos_sensor(0x306E, 0x9090);   // DATA_PATH_SELECT
                                          
    AR0833_write_cmos_sensor(0x301A, 0x001C);   // RESET_REGISTER

}


void AR0833VideoSetting(void)
{
	AR0833DB("AR0833VideoSetting_16:9 exit :\n ");
      #if 0      
	  AR0833_write_cmos_sensor(0x301A, 0x0218);   // RESET_REGISTER
	  
	  AR0833_write_cmos_sensor( 0x0342, 0x21E8);  // LINE_LENGTH_PCK  8680
	  AR0833_write_cmos_sensor( 0x0340, 0x0408);  // FRAME_LENGTH_LINES
	  AR0833_write_cmos_sensor( 0x0202, 0x0300);  // COARSE_INTEGRATION_TIME
	  AR0833_write_cmos_sensor( 0x0344, 0x0008);   // X_ADDR_START
	  AR0833_write_cmos_sensor( 0x0348, 0x0CC5);  // X_ADDR_END
	  AR0833_write_cmos_sensor( 0x0346, 0x0130);   // Y_ADDR_START
	  AR0833_write_cmos_sensor( 0x034A, 0x085B);   // Y_ADDR_END
	  AR0833_write_cmos_sensor( 0x034C, 0x0500);  // X_OUTPUT_SIZE
	  AR0833_write_cmos_sensor( 0x034E, 0x02D0);  // Y_OUTPUT_SIZE
	  AR0833_write_cmos_sensor( 0x3040, 0x48C3);  // READ_MODE
	  AR0833_write_cmos_sensor( 0x0400, 0x0002);  // SCALING_MODE
	  AR0833_write_cmos_sensor( 0x0402, 0x0000);   // SPATIAL_SAMPLING
	  AR0833_write_cmos_sensor( 0x0404, 0x0014);  // SCALE_M
	  AR0833_write_cmos_sensor( 0x0408, 0x1414);  // SECOND_RESIDUAL
	  AR0833_write_cmos_sensor( 0x040A, 0x018C);  // SECOND_CROP
	  AR0833_write_cmos_sensor( 0x306E, 0x9080);   // DATA_PATH_SELECT
											  
	  AR0833_write_cmos_sensor(0x301A, 0x001C);   // RESET_REGISTER
	  
      #else
	  AR0833_write_cmos_sensor(0x301A, 0x0218);   // RESET_REGISTER
										  
	  AR0833_write_cmos_sensor(0x0342, 0x0ECE);   // LINE_LENGTH_PCK
	  AR0833_write_cmos_sensor(0x0340, 0x0A0F);   // FRAME_LENGTH_LINES
	  AR0833_write_cmos_sensor(0x0202, 0x0A0F);   // COARSE_INTEGRATION_TIME
	  AR0833_write_cmos_sensor(0x0344, 0x0008);   // X_ADDR_START
	  AR0833_write_cmos_sensor(0x0348, 0x0CC7);   // X_ADDR_END
	  AR0833_write_cmos_sensor(0x0346, 0x0008);   // Y_ADDR_START
	  AR0833_write_cmos_sensor(0x034A, 0x0997);   // Y_ADDR_END
	  AR0833_write_cmos_sensor(0x034C, 0x0650);   // X_OUTPUT_SIZE
	  AR0833_write_cmos_sensor(0x034E, 0x04BC);   // Y_OUTPUT_SIZE
	  AR0833_write_cmos_sensor(0x3040, 0x4041);   // READ_MODE
	  AR0833_write_cmos_sensor(0x0400, 0x0002);   // SCALING_MODE
	  AR0833_write_cmos_sensor(0x0402, 0x0000);   // SPATIAL_SAMPLING
	  AR0833_write_cmos_sensor(0x0404, 0x0020);   // SCALE_M
	  AR0833_write_cmos_sensor(0x0408, 0x0208);   // SECOND_RESIDUAL
	  AR0833_write_cmos_sensor(0x040A, 0x00C7);   // SECOND_CROP
	  AR0833_write_cmos_sensor(0x306E, 0x9090);   // DATA_PATH_SELECT
											
	  AR0833_write_cmos_sensor(0x301A, 0x001C);   // RESET_REGISTER
      #endif

}


void AR0833CaptureSetting(void)
{
      AR0833DB("AR0833capture setting:");
      AR0833DB("Func_RSE3264x2448_30FPS_MCLK24_2LANE_RAW10");

	  AR0833_write_cmos_sensor(0x301A, 0x0218);  // RESET_REGISTER
											  
      AR0833_write_cmos_sensor(0x0342, 0x1B2E);			//LINE_LENGTH_PCK 6958			
      AR0833_write_cmos_sensor(0x0340, 0x0A0F);			//FRAME_LENGTH_LINES 2575			
      AR0833_write_cmos_sensor(0x0202, 0x0A0F);			 //COARSE_INTEGRATION_TIME 2575			
	  AR0833_write_cmos_sensor(0x0344, 0x0008); 	// X_ADDR_START
	  AR0833_write_cmos_sensor(0x0348, 0x0CC7); 	// X_ADDR_END
	  AR0833_write_cmos_sensor(0x0346, 0x0008); 	// Y_ADDR_START
	  AR0833_write_cmos_sensor(0x034A, 0x0997); 	// Y_ADDR_END
	  AR0833_write_cmos_sensor(0x034C, 0x0CC0); 	// X_OUTPUT_SIZE
	  AR0833_write_cmos_sensor(0x034E, 0x0990); 	// Y_OUTPUT_SIZE
	  AR0833_write_cmos_sensor(0x3040, 0x4041); 	// READ_MODE
	  AR0833_write_cmos_sensor(0x0400, 0x0000); 	// SCALING_MODE
	  AR0833_write_cmos_sensor(0x0402, 0x0000); 	// SPATIAL_SAMPLING
	  AR0833_write_cmos_sensor(0x0404, 0x0010); 	// SCALE_M
	  AR0833_write_cmos_sensor(0x0408, 0x1010); 	// SECOND_RESIDUAL
	  AR0833_write_cmos_sensor(0x040A, 0x0210); 	// SECOND_CROP
	  AR0833_write_cmos_sensor(0x306E, 0x9080); 	// DATA_PATH_SELECT
	  AR0833_write_cmos_sensor(0x301A, 0x001C); 	// RESET_REGISTER


}



/*************************************************************************
* FUNCTION
*   AR0833Open
*
* DESCRIPTION
*   This function initialize the registers of CMOS sensor
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/

UINT32 AR0833Open(void)
{

	volatile signed int i;
	kal_uint16 sensor_id = 0;

	AR0833DB("AR0833Open enter :\n ");
	AR0833_write_cmos_sensor(0x301a,0x0019);// Reset sensor
    mDELAY(2);

	//  Read sensor ID to adjust I2C is OK?
	for(i=0;i<3;i++)
	{
		sensor_id = AR0833_read_cmos_sensor(0x0000);
		AR0833DB("OAR0833 READ ID :%x",sensor_id);
		if(sensor_id != AR0833_SENSOR_ID)
		{
			return ERROR_SENSOR_CONNECT_FAIL;
		}else
			break;
	}
	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.sensorMode = SENSOR_MODE_INIT;
	ar0833.AR0833AutoFlickerMode = KAL_FALSE;
	ar0833.AR0833VideoMode = KAL_FALSE;
	spin_unlock(&ar0833mipiraw_drv_lock);
	AR0833_Sensor_Init();

	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.DummyLines= 0;
	ar0833.DummyPixels= 0;
	ar0833.pvPclk =  (2928); 
	ar0833.videoPclk = (2928);
	ar0833.capPclk = (2928);

	ar0833.shutter = 0x4EA;
	ar0833.pvShutter = 0x4EA;
	ar0833.maxExposureLines =AR0833_PV_PERIOD_LINE_NUMS;

	ar0833.ispBaseGain = BASEGAIN;//0x40
	ar0833.sensorGlobalGain = 0x1000;//sensor gain 1x
	ar0833.pvGain = 0x1000;
	//ar0833.realGain = AR0833Reg2Gain(0x1f);//ispBaseGain as 1x
	spin_unlock(&ar0833mipiraw_drv_lock);
	//AR0833DB("AR0833Reg2Gain(0x1f)=%x :\n ",AR0833Reg2Gain(0x1f));

	AR0833DB("AR0833Open exit :\n ");

    return ERROR_NONE;
}

/*************************************************************************
* FUNCTION
*   AR0833GetSensorID
*
* DESCRIPTION
*   This function get the sensor ID
*
* PARAMETERS
*   *sensorID : return the sensor ID
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 AR0833GetSensorID(UINT32 *sensorID)
{
    int  retry = 1;

	AR0833DB("AR0833GetSensorID enter :\n ");
	AR0833_write_cmos_sensor(0x301a,0x0019);// Reset sensor
    mDELAY(10);

    // check if sensor ID correct
    do {
        *sensorID = AR0833_read_cmos_sensor(0x0000);
        if (*sensorID == AR0833_SENSOR_ID)
        	{
        		AR0833DB("Sensor ID = 0x%04x\n", *sensorID);
            	break;
        	}
        AR0833DB("Read Sensor ID Fail = 0x%04x\n", *sensorID);
        retry--;
    } while (retry > 0);

    if (*sensorID != AR0833_SENSOR_ID) {
        *sensorID = 0xFFFFFFFF;
        return ERROR_SENSOR_CONNECT_FAIL;
    }
    return ERROR_NONE;
}


/*************************************************************************
* FUNCTION
*   AR0833_SetShutter
*
* DESCRIPTION
*   This function set e-shutter of AR0833 to change exposure time.
*
* PARAMETERS
*   shutter : exposured lines
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void AR0833_write_shutter(kal_uint32 shutter)
{
	AR0833_write_cmos_sensor_8(0x0104, 0x01);     // GROUPED_PARAMETER_HOLD
    AR0833_write_cmos_sensor(0x0202, shutter);
    AR0833_write_cmos_sensor_8(0x0104, 0x00);     // GROUPED_PARAMETER_HOLD
}	/* write_AR0833_shutter */

void AR0833_SetShutter(kal_uint32 iShutter)
{

   AR0833DB("AR0833MIPI_SetShutter =%d \n ",iShutter);
   kal_uint16 frame_length=0;
   //iShutter=0x1E2D;
   if (iShutter < 1)
	   iShutter = 1; 
   
   if ( SENSOR_MODE_PREVIEW== ar0833.sensorMode ) 
   {
	   frame_length = AR0833_PV_PERIOD_LINE_NUMS+ar0833.DummyLines;
   }
   else if( SENSOR_MODE_VIDEO== ar0833.sensorMode )
   {
	   frame_length = AR0833_VIDEO_PERIOD_LINE_NUMS+ar0833.DummyLines;
   }
   else if( SENSOR_MODE_CAPTURE== ar0833.sensorMode)
   {
	   frame_length = AR0833_FULL_PERIOD_LINE_NUMS + ar0833.DummyLines;
   }
   
   if(iShutter>frame_length)
	   
	   AR0833_write_cmos_sensor(0x0340, iShutter+10);

   else
	   AR0833_write_cmos_sensor(0x0340, frame_length);


   //if(ar0833.shutter == iShutter)
   //return;
   spin_lock(&ar0833mipiraw_drv_lock);
   ar0833.shutter= iShutter;
   spin_unlock(&ar0833mipiraw_drv_lock);
   AR0833_write_shutter(iShutter);
   AR0833DB("[AR0833Preview]frame_len=%x, line_len=%x\n", AR0833_read_cmos_sensor(0x0340),AR0833_read_cmos_sensor(0x0342));
   return;
}   /*  AR0833_SetShutter   */



/*************************************************************************
* FUNCTION
*   AR0833_read_shutter
*
* DESCRIPTION
*   This function to  Get exposure time.
*
* PARAMETERS
*   None
*
* RETURNS
*   shutter : exposured lines
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 AR0833_read_shutter(void)
{

    kal_uint16 ishutter;

    ishutter = AR0833_read_cmos_sensor(0x0202); /* course_integration_time */

    AR0833DB("AR0833_read_shutter (0x%x)\n",ishutter);

    return ishutter;

}

/*************************************************************************
* FUNCTION
*   AR0833_night_mode
*
* DESCRIPTION
*   This function night mode of AR0833.
*
* PARAMETERS
*   none
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void AR0833_NightMode(kal_bool bEnable)
{
}/*	AR0833_NightMode */



/*************************************************************************
* FUNCTION
*   AR0833Close
*
* DESCRIPTION
*   This function is to turn off sensor module power.
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 AR0833Close(void)
{
    //  CISModulePowerOn(FALSE);
    //s_porting
    //  DRV_I2CClose(AR0833hDrvI2C);
    //e_porting
    ReEnteyCamera = KAL_FALSE;
    return ERROR_NONE;
}	/* AR0833Close() */

void AR0833SetFlipMirror(kal_int32 imgMirror)
{
	switch (imgMirror)
    {
        case IMAGE_NORMAL:         //Set first grab  B
            AR0833_write_cmos_sensor_8(0x0101,0x02);
        break;
        case IMAGE_H_MIRROR:      //Set first grab  Gb
            AR0833_write_cmos_sensor_8(0x0101,0x00);
        break;
        case IMAGE_V_MIRROR:     //Set first grab  Gr
            AR0833_write_cmos_sensor_8(0x0101,0x03);
        break;
        case IMAGE_HV_MIRROR:     //Set first grab  R
            AR0833_write_cmos_sensor_8(0x0101,0x01);
        break;
    }
}


/*************************************************************************
* FUNCTION
*   AR0833Preview
*
* DESCRIPTION
*   This function start the sensor preview.
*
* PARAMETERS
*   *image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 AR0833Preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
                                                MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{

	AR0833DB("AR0833Preview enter:");

	// preview size
	if(ar0833.sensorMode == SENSOR_MODE_PREVIEW)
	{
		// do nothing
		// FOR CCT PREVIEW
	}
	else
	{
		//AR0833DB("AR0833Preview setting!!\n");
		AR0833PreviewSetting();
	}
	
	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.sensorMode = SENSOR_MODE_PREVIEW; // Need set preview setting after capture mode
	ar0833.DummyPixels = 0;//define dummy pixels and lines
	ar0833.DummyLines = 0 ;
	AR0833_FeatureControl_PERIOD_PixelNum=AR0833_PV_PERIOD_PIXEL_NUMS+ ar0833.DummyPixels;
	AR0833_FeatureControl_PERIOD_LineNum=AR0833_PV_PERIOD_LINE_NUMS+ar0833.DummyLines;
	spin_unlock(&ar0833mipiraw_drv_lock);

	//AR0833_write_shutter(ar0833.shutter);
	//write_AR0833_gain(ar0833.pvGain);

	//set mirror & flip
	//AR0833DB("[AR0833Preview] mirror&flip: %d \n",sensor_config_data->SensorImageMirror);
	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.imgMirror = sensor_config_data->SensorImageMirror;
	spin_unlock(&ar0833mipiraw_drv_lock);
	AR0833SetFlipMirror(sensor_config_data->SensorImageMirror);

	AR0833DBSOFIA("[AR0833Preview]frame_len=%x\n", AR0833_read_cmos_sensor(0x0340));

    mDELAY(40);
	AR0833DB("AR0833Preview exit:\n");
    return ERROR_NONE;
}	/* AR0833Preview() */



UINT32 AR0833Video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
                                                MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{

	AR0833DB("AR0833Video enter:");

	if(ar0833.sensorMode == SENSOR_MODE_VIDEO)
	{
		// do nothing
	}
	else
	{
		AR0833VideoSetting();

	}
	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.sensorMode = SENSOR_MODE_VIDEO;
	ar0833.DummyPixels = 0;//define dummy pixels and lines
	ar0833.DummyLines = 0 ;
	AR0833_FeatureControl_PERIOD_PixelNum=AR0833_VIDEO_PERIOD_PIXEL_NUMS+ ar0833.DummyPixels;
	AR0833_FeatureControl_PERIOD_LineNum=AR0833_VIDEO_PERIOD_LINE_NUMS+ar0833.DummyLines;
	spin_unlock(&ar0833mipiraw_drv_lock);

	//AR0833_write_shutter(ar0833.shutter);
	//write_AR0833_gain(ar0833.pvGain);

	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.imgMirror = sensor_config_data->SensorImageMirror;
	spin_unlock(&ar0833mipiraw_drv_lock);
	AR0833SetFlipMirror(sensor_config_data->SensorImageMirror);

	AR0833DBSOFIA("[AR0833Video]frame_len=%x\n", AR0833_read_cmos_sensor(0x0340));

    mDELAY(40);
	AR0833DB("AR0833Video exit:\n");
    return ERROR_NONE;
}


UINT32 AR0833Capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
                                                MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{

 	kal_uint32 shutter = ar0833.shutter;
	kal_uint32 temp_data;
	//kal_uint32 pv_line_length , cap_line_length,

	if( SENSOR_MODE_CAPTURE== ar0833.sensorMode)
	{
		AR0833DB("AR0833Capture BusrtShot!!!\n");
	}else{
	AR0833DB("AR0833Capture enter:\n");

	//Record Preview shutter & gain
	shutter=AR0833_read_shutter();
	temp_data =  read_AR0833MIPI_gain();
	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.pvShutter =shutter;
	ar0833.sensorGlobalGain = temp_data;
	ar0833.pvGain =ar0833.sensorGlobalGain;
	spin_unlock(&ar0833mipiraw_drv_lock);

	AR0833DB("[AR0833Capture]ar0833.shutter=%d, read_pv_shutter=%d, read_pv_gain = 0x%x\n",ar0833.shutter, shutter,ar0833.sensorGlobalGain);

	// Full size setting
	AR0833CaptureSetting();
    //mDELAY(40);

	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.sensorMode = SENSOR_MODE_CAPTURE;
	ar0833.imgMirror = sensor_config_data->SensorImageMirror;
	ar0833.DummyPixels = 0;//define dummy pixels and lines
	ar0833.DummyLines = 0 ;
	AR0833_FeatureControl_PERIOD_PixelNum = AR0833_FULL_PERIOD_PIXEL_NUMS + ar0833.DummyPixels;
	AR0833_FeatureControl_PERIOD_LineNum = AR0833_FULL_PERIOD_LINE_NUMS + ar0833.DummyLines;
	spin_unlock(&ar0833mipiraw_drv_lock);

	//AR0833DB("[AR0833Capture] mirror&flip: %d\n",sensor_config_data->SensorImageMirror);
	AR0833SetFlipMirror(sensor_config_data->SensorImageMirror);

	//#if defined(MT6575)||defined(MT6577)
    if(AR0833CurrentScenarioId==MSDK_SCENARIO_ID_CAMERA_ZSD)
    {
		AR0833DB("AR0833Capture exit ZSD!!\n");
		return ERROR_NONE;
    }
	//#endif

	#if 0 //no need to calculate shutter from mt6589
	//calculate shutter
	pv_line_length = AR0833_PV_PERIOD_PIXEL_NUMS + ar0833.DummyPixels;
	cap_line_length = AR0833_FULL_PERIOD_PIXEL_NUMS + ar0833.DummyPixels;

	AR0833DB("[AR0833Capture]pv_line_length =%d,cap_line_length =%d\n",pv_line_length,cap_line_length);
	AR0833DB("[AR0833Capture]pv_shutter =%d\n",shutter );

	shutter =  shutter * pv_line_length / cap_line_length;
	shutter = shutter *ar0833.capPclk / ar0833.pvPclk;
	shutter *= 2; //preview bining///////////////////////////////////////

	if(shutter < 3)
	    shutter = 3;

	AR0833_write_shutter(shutter);

	//gain = read_AR0833_gain();

	AR0833DB("[AR0833Capture]cap_shutter =%d , cap_read gain = 0x%x\n",shutter,read_AR0833_gain());
	//write_AR0833_gain(ar0833.sensorGlobalGain);
   #endif

	AR0833DB("AR0833Capture exit:\n");
	}

    return ERROR_NONE;
}	/* AR0833Capture() */

UINT32 AR0833GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{

    AR0833DB("AR0833GetResolution!!\n");

	pSensorResolution->SensorPreviewWidth	= AR0833_IMAGE_SENSOR_PV_WIDTH;
    pSensorResolution->SensorPreviewHeight	= AR0833_IMAGE_SENSOR_PV_HEIGHT;
    pSensorResolution->SensorFullWidth		= AR0833_IMAGE_SENSOR_FULL_WIDTH;
    pSensorResolution->SensorFullHeight		= AR0833_IMAGE_SENSOR_FULL_HEIGHT;
    pSensorResolution->SensorVideoWidth		= AR0833_IMAGE_SENSOR_VIDEO_WIDTH;
    pSensorResolution->SensorVideoHeight    = AR0833_IMAGE_SENSOR_VIDEO_HEIGHT;
    return ERROR_NONE;
}   /* AR0833GetResolution() */

UINT32 AR0833GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
                                                MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
                                                MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{      switch(ScenarioId)
	   {
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			pSensorInfo->SensorPreviewResolutionX=AR0833_IMAGE_SENSOR_FULL_WIDTH;
			pSensorInfo->SensorPreviewResolutionY=AR0833_IMAGE_SENSOR_FULL_HEIGHT;
            pSensorInfo->SensorFullResolutionX    =  AR0833_IMAGE_SENSOR_FULL_WIDTH;
            pSensorInfo->SensorFullResolutionY    =  AR0833_IMAGE_SENSOR_FULL_HEIGHT; 			
			pSensorInfo->SensorCameraPreviewFrameRate=30;
		break;

        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
        	pSensorInfo->SensorPreviewResolutionX=AR0833_IMAGE_SENSOR_PV_WIDTH;
       		pSensorInfo->SensorPreviewResolutionY=AR0833_IMAGE_SENSOR_PV_HEIGHT;
            pSensorInfo->SensorFullResolutionX    =  AR0833_IMAGE_SENSOR_FULL_WIDTH;
            pSensorInfo->SensorFullResolutionY    =  AR0833_IMAGE_SENSOR_FULL_HEIGHT;       		
			pSensorInfo->SensorCameraPreviewFrameRate=30;            
            break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
        	pSensorInfo->SensorPreviewResolutionX=AR0833_IMAGE_SENSOR_VIDEO_WIDTH;
       		pSensorInfo->SensorPreviewResolutionY=AR0833_IMAGE_SENSOR_VIDEO_HEIGHT;
            pSensorInfo->SensorFullResolutionX    =  AR0833_IMAGE_SENSOR_FULL_WIDTH;
            pSensorInfo->SensorFullResolutionY    =  AR0833_IMAGE_SENSOR_FULL_HEIGHT;       		
			pSensorInfo->SensorCameraPreviewFrameRate=30;  
			break;
		default:
        	pSensorInfo->SensorPreviewResolutionX=AR0833_IMAGE_SENSOR_PV_WIDTH;
       		pSensorInfo->SensorPreviewResolutionY=AR0833_IMAGE_SENSOR_PV_HEIGHT;
            pSensorInfo->SensorFullResolutionX    =  AR0833_IMAGE_SENSOR_FULL_WIDTH;
            pSensorInfo->SensorFullResolutionY    =  AR0833_IMAGE_SENSOR_FULL_HEIGHT;       		
			pSensorInfo->SensorCameraPreviewFrameRate=30;
		break;
	}

	spin_lock(&ar0833mipiraw_drv_lock);
	ar0833.imgMirror = pSensorConfigData->SensorImageMirror ;
	spin_unlock(&ar0833mipiraw_drv_lock);

   	pSensorInfo->SensorOutputDataFormat= SENSOR_OUTPUT_FORMAT_RAW_R;
    pSensorInfo->SensorClockPolarity =SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
    pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;

    pSensorInfo->SensroInterfaceType=SENSOR_INTERFACE_TYPE_MIPI;

    pSensorInfo->CaptureDelayFrame = 1;
    pSensorInfo->PreviewDelayFrame = 2;
    pSensorInfo->VideoDelayFrame = 10;

    pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_8MA;
    pSensorInfo->AEShutDelayFrame = 0;//0;		    /* The frame of setting shutter default 0 for TG int */
    pSensorInfo->AESensorGainDelayFrame = 1 ;//0;     /* The frame of setting sensor gain */
    pSensorInfo->AEISPGainDelayFrame = 2;
	  
    switch (ScenarioId)
    {
        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
            pSensorInfo->SensorClockFreq=24;
            pSensorInfo->SensorClockRisingCount= 0;

            pSensorInfo->SensorGrabStartX = AR0833_PV_X_START;
            pSensorInfo->SensorGrabStartY = AR0833_PV_Y_START;

            pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;
            pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
	     	pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 26;
	    	pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;	// 0 is default 1x
			pSensorInfo->SensorHightSampling = 0;	 // 0 is default 1x
			pSensorInfo->SensorPacketECCOrder = 1;
            break;
        case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
            pSensorInfo->SensorClockFreq=24;
            pSensorInfo->SensorClockRisingCount= 0;

            pSensorInfo->SensorGrabStartX = AR0833_VIDEO_X_START;
            pSensorInfo->SensorGrabStartY = AR0833_VIDEO_Y_START;

            pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;
            pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
	     	pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 26;
	    	pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;	// 0 is default 1x
			pSensorInfo->SensorHightSampling = 0;	 // 0 is default 1x
			pSensorInfo->SensorPacketECCOrder = 1;
            break;
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
            pSensorInfo->SensorClockFreq=24;
            pSensorInfo->SensorClockRisingCount= 0;

            pSensorInfo->SensorGrabStartX = AR0833_FULL_X_START;	//2*AR0833_IMAGE_SENSOR_PV_STARTX;
            pSensorInfo->SensorGrabStartY = AR0833_FULL_Y_START;	//2*AR0833_IMAGE_SENSOR_PV_STARTY;

            pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;
            pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
            pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 26;
            pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;	// 0 is default 1x
			pSensorInfo->SensorHightSampling = 0;	 // 0 is default 1x
			pSensorInfo->SensorPacketECCOrder = 1;

            break;

        default:
			pSensorInfo->SensorClockFreq=24;
            pSensorInfo->SensorClockRisingCount= 0;

            pSensorInfo->SensorGrabStartX = AR0833_PV_X_START;
            pSensorInfo->SensorGrabStartY = AR0833_PV_Y_START;

            pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;
            pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
	     	pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 26;
	    	pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;	// 0 is default 1x
			pSensorInfo->SensorHightSampling = 0;	 // 0 is default 1x
			pSensorInfo->SensorPacketECCOrder = 1;

            break;
    }

    memcpy(pSensorConfigData, &AR0833SensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));

    return ERROR_NONE;
}   /* AR0833GetInfo() */


UINT32 AR0833Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
                                                MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
		spin_lock(&ar0833mipiraw_drv_lock);
		AR0833CurrentScenarioId = ScenarioId;
		spin_unlock(&ar0833mipiraw_drv_lock);
		AR0833DB("AR0833CurrentScenarioId=%d\n",AR0833CurrentScenarioId);

	switch (ScenarioId)
    {
        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
            AR0833Preview(pImageWindow, pSensorConfigData);
            break;
        case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			AR0833Video(pImageWindow, pSensorConfigData);
			break;
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
            AR0833Capture(pImageWindow, pSensorConfigData);
            break;

        default:
            return ERROR_INVALID_SCENARIO_ID;

    }
    return ERROR_NONE;
} /* AR0833Control() */


UINT32 AR0833SetVideoMode(UINT16 u2FrameRate)
{

    kal_uint32 MIN_Frame_length =0,frameRate=0,extralines=0;
    AR0833DB("[AR0833SetVideoMode] frame rate = %d\n", u2FrameRate);

	spin_lock(&ar0833mipiraw_drv_lock);
	VIDEO_MODE_TARGET_FPS=u2FrameRate;
	spin_unlock(&ar0833mipiraw_drv_lock);

	if(u2FrameRate==0)
	{
		AR0833DB("Disable Video Mode or dynimac fps\n");
		return KAL_TRUE;
	}
	if(u2FrameRate >30 || u2FrameRate <5)
	    AR0833DB("error frame rate seting\n");

    if(ar0833.sensorMode == SENSOR_MODE_VIDEO)//video ScenarioId recording
    {
    	if(ar0833.AR0833AutoFlickerMode == KAL_TRUE)
    	{
    		if (u2FrameRate==30)
				frameRate= 306;
			else if(u2FrameRate==15)
				frameRate= 148;//148;
			else
				frameRate=u2FrameRate*10;

			MIN_Frame_length = (ar0833.videoPclk*100000)/(AR0833_VIDEO_PERIOD_PIXEL_NUMS + ar0833.DummyPixels)/frameRate*10;
    	}
		else
			MIN_Frame_length = (ar0833.videoPclk*100000) /(AR0833_VIDEO_PERIOD_PIXEL_NUMS + ar0833.DummyPixels)/u2FrameRate;

		if((MIN_Frame_length <=AR0833_VIDEO_PERIOD_LINE_NUMS))
		{
			MIN_Frame_length = AR0833_VIDEO_PERIOD_LINE_NUMS;
			AR0833DB("[AR0833SetVideoMode]current fps = %d\n", (ar0833.pvPclk*100000)  /(AR0833_PV_PERIOD_PIXEL_NUMS)/AR0833_PV_PERIOD_LINE_NUMS);
		}
		AR0833DB("[AR0833SetVideoMode]current fps (10 base)= %d\n", (ar0833.pvPclk*100000)*10/(AR0833_PV_PERIOD_PIXEL_NUMS + ar0833.DummyPixels)/MIN_Frame_length);
		extralines = MIN_Frame_length - AR0833_VIDEO_PERIOD_LINE_NUMS;
		
		spin_lock(&ar0833mipiraw_drv_lock);
		ar0833.DummyPixels = 0;//define dummy pixels and lines
		ar0833.DummyLines = extralines ;
		spin_unlock(&ar0833mipiraw_drv_lock);
		
		AR0833_SetDummy(ar0833.DummyPixels,extralines);
	}
	AR0833DB("[AR0833SetVideoMode]MIN_Frame_length=%d,ar0833.DummyLines=%d\n",MIN_Frame_length,ar0833.DummyLines);

    return KAL_TRUE;
}

UINT32 AR0833SetAutoFlickerMode(kal_bool bEnable, UINT16 u2FrameRate)
{
	//return ERROR_NONE;

    //AR0833DB("[AR0833SetAutoFlickerMode] frame rate(10base) = %d %d\n", bEnable, u2FrameRate);
	if(bEnable) {   // enable auto flicker
		spin_lock(&ar0833mipiraw_drv_lock);
		ar0833.AR0833AutoFlickerMode = KAL_TRUE;
		spin_unlock(&ar0833mipiraw_drv_lock);
    } else {
    	spin_lock(&ar0833mipiraw_drv_lock);
        ar0833.AR0833AutoFlickerMode = KAL_FALSE;
		spin_unlock(&ar0833mipiraw_drv_lock);
        AR0833DB("Disable Auto flicker\n");
    }

    return ERROR_NONE;
}

UINT32 AR0833SetTestPatternMode(kal_bool bEnable)
{
    AR0833DB("[AR0833SetTestPatternMode] Test pattern enable:%d\n", bEnable);

    return ERROR_NONE;
}

UINT32 AR0833MIPISetMaxFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 frameRate) {
	kal_uint32 pclk;
	kal_int16 dummyLine;
	kal_uint16 lineLength,frameHeight;
		
	AR0833DB("AR0833MIPISetMaxFramerateByScenario: scenarioId = %d, frame rate = %d\n",scenarioId,frameRate);
	switch (scenarioId) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			pclk = 292800000;
			lineLength = AR0833_PV_PERIOD_PIXEL_NUMS;
			frameHeight = (10 * pclk)/frameRate/lineLength;
			dummyLine = frameHeight - AR0833_PV_PERIOD_LINE_NUMS;
			if(dummyLine<0)
				dummyLine = 0;
			spin_lock(&ar0833mipiraw_drv_lock);
			ar0833.sensorMode = SENSOR_MODE_PREVIEW;
			spin_unlock(&ar0833mipiraw_drv_lock);
			AR0833_SetDummy(0, dummyLine);			
			break;			
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			pclk = 292800000;
			lineLength = AR0833_VIDEO_PERIOD_PIXEL_NUMS;
			frameHeight = (10 * pclk)/frameRate/lineLength;
			dummyLine = frameHeight - AR0833_VIDEO_PERIOD_LINE_NUMS;
			if(dummyLine<0)
				dummyLine = 0;
			spin_lock(&ar0833mipiraw_drv_lock);
			ar0833.sensorMode = SENSOR_MODE_VIDEO;
			spin_unlock(&ar0833mipiraw_drv_lock);
			AR0833_SetDummy(0, dummyLine);			
			break;			
			 break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:			
			pclk = 292800000;
			lineLength = AR0833_FULL_PERIOD_PIXEL_NUMS;
			frameHeight = (10 * pclk)/frameRate/lineLength;
			dummyLine = frameHeight - AR0833_FULL_PERIOD_LINE_NUMS;
			if(dummyLine<0)
				dummyLine = 0;
			spin_lock(&ar0833mipiraw_drv_lock);
			ar0833.sensorMode = SENSOR_MODE_CAPTURE;
			spin_unlock(&ar0833mipiraw_drv_lock);
			AR0833_SetDummy(0, dummyLine);			
			break;		
	
		default:
			break;
	}	
	return ERROR_NONE;
}


UINT32 AR0833MIPIGetDefaultFramerateByScenario(MSDK_SCENARIO_ID_ENUM scenarioId, MUINT32 *pframeRate) 
{

	switch (scenarioId) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			 *pframeRate = 300;
			 break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			 *pframeRate = 300;
			break;			
		default:
			break;
	}

	return ERROR_NONE;
}



UINT32 AR0833FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
                                                                UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	   UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	   UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	   UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	   UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	   UINT32 SensorRegNumber;
	   UINT32 i;
	   PNVRAM_SENSOR_DATA_STRUCT pSensorDefaultData=(PNVRAM_SENSOR_DATA_STRUCT) pFeaturePara;
	   MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	   MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;
	   MSDK_SENSOR_GROUP_INFO_STRUCT *pSensorGroupInfo=(MSDK_SENSOR_GROUP_INFO_STRUCT *) pFeaturePara;
	   MSDK_SENSOR_ITEM_INFO_STRUCT *pSensorItemInfo=(MSDK_SENSOR_ITEM_INFO_STRUCT *) pFeaturePara;
	   MSDK_SENSOR_ENG_INFO_STRUCT	  *pSensorEngInfo=(MSDK_SENSOR_ENG_INFO_STRUCT *) pFeaturePara;
	
	   AR0833DB("AR0833_FeatureControl FeatureId(%d)\n",FeatureId);
	
	   switch (FeatureId)
	   {
		   case SENSOR_FEATURE_GET_RESOLUTION:
			   *pFeatureReturnPara16++=AR0833_IMAGE_SENSOR_FULL_WIDTH;
			   *pFeatureReturnPara16=AR0833_IMAGE_SENSOR_FULL_HEIGHT;
			   *pFeatureParaLen=4;
			   break;
		   case SENSOR_FEATURE_GET_PERIOD:
			   *pFeatureReturnPara16++= AR0833_FeatureControl_PERIOD_PixelNum;
			   *pFeatureReturnPara16= AR0833_FeatureControl_PERIOD_LineNum;
			   *pFeatureParaLen=4;
			   break;
		   case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			   switch(AR0833CurrentScenarioId)
			   {   
			       case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
					   *pFeatureReturnPara32 = ar0833.videoPclk * 100000;
					      *pFeatureParaLen=4;
					   break;
				   case MSDK_SCENARIO_ID_CAMERA_ZSD:
				   case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
					   *pFeatureReturnPara32 = ar0833.capPclk * 100000; //19500000;
						  *pFeatureParaLen=4;
					   break;
				   default:
					   *pFeatureReturnPara32 = ar0833.pvPclk * 100000; //19500000;
						  *pFeatureParaLen=4;
					   break;
			   }
			   break;
		   case SENSOR_FEATURE_SET_ESHUTTER:
			   AR0833_SetShutter(*pFeatureData16);
			   break;
		   case SENSOR_FEATURE_SET_NIGHTMODE:
			   AR0833_NightMode((BOOL) *pFeatureData16);
			   break;
		   case SENSOR_FEATURE_SET_GAIN:
			   AR0833MIPI_SetGain((UINT16) *pFeatureData16);
			   break;
		   case SENSOR_FEATURE_SET_FLASHLIGHT:
			   break;
		   case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			  // AR0833_isp_master_clock=*pFeatureData32;
			   break;
		   case SENSOR_FEATURE_SET_REGISTER:
			   AR0833_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
			   break;
		   case SENSOR_FEATURE_GET_REGISTER:
			   pSensorRegData->RegData = AR0833_read_cmos_sensor(pSensorRegData->RegAddr);
			   break;
		   case SENSOR_FEATURE_SET_CCT_REGISTER:
			   SensorRegNumber=FACTORY_END_ADDR;
			   for (i=0;i<SensorRegNumber;i++)
			   {
				   AR0833SensorCCT[i].Addr=*pFeatureData32++;
				   AR0833SensorCCT[i].Para=*pFeatureData32++;
			   }
			   break;
		   case SENSOR_FEATURE_GET_CCT_REGISTER:
			   SensorRegNumber=FACTORY_END_ADDR;
			   if (*pFeatureParaLen<(SensorRegNumber*sizeof(SENSOR_REG_STRUCT)+4))
				   return FALSE;
			   *pFeatureData32++=SensorRegNumber;
			   for (i=0;i<SensorRegNumber;i++)
			   {
				   *pFeatureData32++=AR0833SensorCCT[i].Addr;
				   *pFeatureData32++=AR0833SensorCCT[i].Para;
			   }
			   break;
		   case SENSOR_FEATURE_SET_ENG_REGISTER:
			   SensorRegNumber=ENGINEER_END;
			   for (i=0;i<SensorRegNumber;i++)
			   {
				   AR0833SensorReg[i].Addr=*pFeatureData32++;
				   AR0833SensorReg[i].Para=*pFeatureData32++;
			   }
			   break;
		   case SENSOR_FEATURE_GET_ENG_REGISTER:
			   SensorRegNumber=ENGINEER_END;
			   if (*pFeatureParaLen<(SensorRegNumber*sizeof(SENSOR_REG_STRUCT)+4))
				   return FALSE;
			   *pFeatureData32++=SensorRegNumber;
			   for (i=0;i<SensorRegNumber;i++)
			   {
				   *pFeatureData32++=AR0833SensorReg[i].Addr;
				   *pFeatureData32++=AR0833SensorReg[i].Para;
			   }
			   break;
		   case SENSOR_FEATURE_GET_REGISTER_DEFAULT:
			   if (*pFeatureParaLen>=sizeof(NVRAM_SENSOR_DATA_STRUCT))
			   {
				   pSensorDefaultData->Version=NVRAM_CAMERA_SENSOR_FILE_VERSION;
				   pSensorDefaultData->SensorId=AR0833_SENSOR_ID;
				   memcpy(pSensorDefaultData->SensorEngReg, AR0833SensorReg, sizeof(SENSOR_REG_STRUCT)*ENGINEER_END);
				   memcpy(pSensorDefaultData->SensorCCTReg, AR0833SensorCCT, sizeof(SENSOR_REG_STRUCT)*FACTORY_END_ADDR);
			   }
			   else
				   return FALSE;
			   *pFeatureParaLen=sizeof(NVRAM_SENSOR_DATA_STRUCT);
			   break;
		   case SENSOR_FEATURE_GET_CONFIG_PARA:
			   break;
		   case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
			   //AR0833_camera_para_to_sensor();
			   break;
	
		   case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
			   //AR0833_sensor_to_camera_para();
			   break;
		   case SENSOR_FEATURE_GET_GROUP_COUNT:

			   break;
		   case SENSOR_FEATURE_GET_GROUP_INFO:

			   break;
		   case SENSOR_FEATURE_GET_ITEM_INFO:

			   break;
	
		   case SENSOR_FEATURE_SET_ITEM_INFO:

			   break;
	
		   case SENSOR_FEATURE_GET_ENG_INFO:
			   pSensorEngInfo->SensorId = 221;
			   pSensorEngInfo->SensorType = CMOS_SENSOR;
	
			   pSensorEngInfo->SensorOutputDataFormat = SENSOR_OUTPUT_FORMAT_RAW_R;
	
			   *pFeatureParaLen=sizeof(MSDK_SENSOR_ENG_INFO_STRUCT);
			   break;
		   case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			   // get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			   // if EEPROM does not exist in camera module.
			   *pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			   *pFeatureParaLen=4;
			   break;
	
		   case SENSOR_FEATURE_SET_VIDEO_MODE:
			   AR0833SetVideoMode(*pFeatureData16);
			   break;
		   case SENSOR_FEATURE_CHECK_SENSOR_ID:
			   AR0833GetSensorID(pFeatureReturnPara32);
			   break;
	       case SENSOR_FEATURE_SET_AUTO_FLICKER_MODE:
               AR0833SetAutoFlickerMode((BOOL)*pFeatureData16, *(pFeatureData16+1));
	           break;
           case SENSOR_FEATURE_SET_TEST_PATTERN:
               AR0833SetTestPatternMode((BOOL)*pFeatureData16);
               break;
		   case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
			   AR0833MIPISetMaxFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32, *(pFeatureData32+1));
			   break;
		   case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
			   AR0833MIPIGetDefaultFramerateByScenario((MSDK_SCENARIO_ID_ENUM)*pFeatureData32, (MUINT32 *)(*(pFeatureData32+1)));
			  break;

		   default:
			   break;

    }
    return ERROR_NONE;
}	/* AR0833FeatureControl() */


SENSOR_FUNCTION_STRUCT	SensorFuncAR0833=
{
    AR0833Open,
    AR0833GetInfo,
    AR0833GetResolution,
    AR0833FeatureControl,
    AR0833Control,
    AR0833Close
};

UINT32 AR0833_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
    /* To Do : Check Sensor status here */
    if (pfFunc!=NULL)
        *pfFunc=&SensorFuncAR0833;

    return ERROR_NONE;
}   /* SensorInit() */

