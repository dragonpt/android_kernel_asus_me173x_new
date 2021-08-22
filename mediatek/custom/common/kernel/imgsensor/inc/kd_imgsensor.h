#ifndef _KD_IMGSENSOR_H
#define _KD_IMGSENSOR_H

#include <linux/ioctl.h>

#ifndef ASSERT
    #define ASSERT(expr)        BUG_ON(!(expr))
#endif

#define IMGSENSORMAGIC 'i'
//IOCTRL(inode * ,file * ,cmd ,arg )
//S means "set through a ptr"
//T means "tell by a arg value"
//G means "get by a ptr"
//Q means "get by return a value"
//X means "switch G and S atomically"
//H means "switch T and Q atomically"

/*******************************************************************************
*
********************************************************************************/

/*******************************************************************************
*
********************************************************************************/

//sensorOpen
//This command will TBD
#define KDIMGSENSORIOC_T_OPEN            _IO(IMGSENSORMAGIC,0)
//sensorGetInfo
//This command will TBD
#define KDIMGSENSORIOC_X_GETINFO            _IOWR(IMGSENSORMAGIC,5,ACDK_SENSOR_GETINFO_STRUCT)
//sensorGetResolution
//This command will TBD
#define KDIMGSENSORIOC_X_GETRESOLUTION      _IOWR(IMGSENSORMAGIC,10,ACDK_SENSOR_RESOLUTION_INFO_STRUCT)
//sensorFeatureControl
//This command will TBD
#define KDIMGSENSORIOC_X_FEATURECONCTROL    _IOWR(IMGSENSORMAGIC,15,ACDK_SENSOR_FEATURECONTROL_STRUCT)
//sensorControl
//This command will TBD
#define KDIMGSENSORIOC_X_CONTROL            _IOWR(IMGSENSORMAGIC,20,ACDK_SENSOR_CONTROL_STRUCT)
//sensorClose
//This command will TBD
#define KDIMGSENSORIOC_T_CLOSE            _IO(IMGSENSORMAGIC,25)
//sensorSearch 
#define KDIMGSENSORIOC_T_CHECK_IS_ALIVE     _IO(IMGSENSORMAGIC, 30) 
//set sensor driver
#define KDIMGSENSORIOC_X_SET_DRIVER         _IOWR(IMGSENSORMAGIC,35,SENSOR_DRIVER_INDEX_STRUCT)
//get socket postion
#define KDIMGSENSORIOC_X_GET_SOCKET_POS     _IOWR(IMGSENSORMAGIC,40,u32)
//set I2C bus 
#define KDIMGSENSORIOC_X_SET_I2CBUS     _IOWR(IMGSENSORMAGIC,45,u32)
//set I2C bus 
#define KDIMGSENSORIOC_X_RELEASE_I2C_TRIGGER_LOCK     _IO(IMGSENSORMAGIC,50)


/*******************************************************************************
*
********************************************************************************/
/* SENSOR CHIP VERSION */

/*******************************************************************************
              superdragonpt: For Asus Memo Pad HD7 - ME173X:

Asus uses External ISP IC SPCA7002 for camera Image Signal Processing
#define EXISPMAIN_SENSOR_ID                     0x00   //This is a dummy for the External Camera ISP IC SPCA7002 (?)
#define EXISPSUB_SENSOR_ID                      0x00   //This is a dummy for the External Camera ISP IC SPCA7002 (?)
********************************************************************************/

#define A5142MIPI_SENSOR_ID                     0x4800
#define HM2056MIPI_SENSOR_ID					0x2056
#define MT9M114MIPI_SENSOR_ID                   0x2481
#define GC0339_SENSOR_ID				        0xC8 //0xc8

/* CAMERA DRIVER NAME */
#define CAMERA_HW_DEVNAME            "kd_camera_hw"

/* SENSOR DEVICE DRIVER NAME */
//#define SENSOR_DRVNAME_EXISP_MAIN_MIPI_YUV   "exisp_main_yuv"
      //SENSOR_DRVNAME_EXISP_MAIN_MIPI_YUV    exisp_main_yuv
//#define SENSOR_DRVNAME_EXISP_SUB_MIPI_YUV   "exisp_sub_yuv"
      //SENSOR_DRVNAME_EXISP_SUB_MIPI_YUV    exisp_sub_yuv
#define SENSOR_DRVNAME_A5142_MIPI_RAW   "a5142mipiraw"
      //SENSOR_DRVNAME_A5142_MIPI_RAW    a5142mipiraw
#define SENSOR_DRVNAME_HM2056_MIPI_RAW				"hm2056mipiraw"
      //SENSOR_DRVNAME_HM2056_MIPI_RAW               hm2056mipiraw
#define SENSOR_DRVNAME_MT9M114_MIPI_RAW		"mt9m114mipiraw"
      //SENSOR_DRVNAME_MT9M114_MIPI_RAW      mt9m114mipiraw
#define SENSOR_DRVNAME_GC0339_RAW      "gc0339raw"
      //SENSOR_DRVNAME_GC0339_RAW       gc0339raw


/*******************************************************************************
*
********************************************************************************/

void KD_IMGSENSOR_PROFILE_INIT(void); 
void KD_IMGSENSOR_PROFILE(char *tag); 

#define mDELAY(ms)     mdelay(ms) 
#define uDELAY(us)       udelay(us) 
#endif //_KD_IMGSENSOR_H
