

#ifndef _GC0339_SENSOR_H
#define _GC0339_SENSOR_H

#define GC0339_DEBUG
#define GC0339_DRIVER_TRACE
//#define GC0339_TEST_PATTEM
//#define SENSORDB printk
//#define SENSORDB(x,...)

#define GC0339_FACTORY_START_ADDR 0
#define GC0339_ENGINEER_START_ADDR 10
 
typedef enum GC0339_group_enum
{
  GC0339_PRE_GAIN = 0,
  GC0339_CMMCLK_CURRENT,
  GC0339_FRAME_RATE_LIMITATION,
  GC0339_REGISTER_EDITOR,
  GC0339_GROUP_TOTAL_NUMS
} GC0339_FACTORY_GROUP_ENUM;

typedef enum GC0339_register_index
{
  GC0339_SENSOR_BASEGAIN = GC0339_FACTORY_START_ADDR,
  GC0339_PRE_GAIN_R_INDEX,
  GC0339_PRE_GAIN_Gr_INDEX,
  GC0339_PRE_GAIN_Gb_INDEX,
  GC0339_PRE_GAIN_B_INDEX,
  GC0339_FACTORY_END_ADDR
} GC0339_FACTORY_REGISTER_INDEX;

typedef enum GC0339_engineer_index
{
  GC0339_CMMCLK_CURRENT_INDEX = GC0339_ENGINEER_START_ADDR,
  GC0339_ENGINEER_END
} GC0339_FACTORY_ENGINEER_INDEX;

typedef struct _sensor_data_struct
{
  SENSOR_REG_STRUCT reg[GC0339_ENGINEER_END];
  SENSOR_REG_STRUCT cct[GC0339_FACTORY_END_ADDR];
} sensor_data_struct;

/* SENSOR PREVIEW/CAPTURE VT CLOCK */
#define GC0339_PREVIEW_CLK                   12000000
#define GC0339_CAPTURE_CLK                    12000000

#define GC0339_COLOR_FORMAT                    SENSOR_OUTPUT_FORMAT_RAW8_B//SENSOR_OUTPUT_FORMAT_RAW_B

#define GC0339_MIN_ANALOG_GAIN				1	/* 1x */
#define GC0339_MAX_ANALOG_GAIN				6	/* 6x */


/* FRAME RATE UNIT */
#define GC0339_FPS(x)                          (10 * (x))

/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
#define GC0339_FULL_PERIOD_PIXEL_NUMS          960
#define GC0339_FULL_PERIOD_LINE_NUMS           660

#define GC0339_VIDEO_PERIOD_PIXEL_NUMS          960
#define GC0339_VIDEO_PERIOD_LINE_NUMS           660

#define GC0339_PV_PERIOD_PIXEL_NUMS            960
#define GC0339_PV_PERIOD_LINE_NUMS             660

/* SENSOR START/END POSITION */
#define GC0339_FULL_X_START                    2
#define GC0339_FULL_Y_START                    2
#define GC0339_IMAGE_SENSOR_FULL_WIDTH         (652 - 8)
#define GC0339_IMAGE_SENSOR_FULL_HEIGHT        (486 - 6)

#define GC0339_VIDEO_X_START                      2
#define GC0339_VIDEO_Y_START                      2
#define GC0339_IMAGE_SENSOR_VIDEO_WIDTH           (652 - 8)
#define GC0339_IMAGE_SENSOR_VIDEO_HEIGHT          (486  - 6)

#define GC0339_PV_X_START                      2
#define GC0339_PV_Y_START                      2
#define GC0339_IMAGE_SENSOR_PV_WIDTH           (652 - 8)
#define GC0339_IMAGE_SENSOR_PV_HEIGHT          (486  - 6)

/* SENSOR READ/WRITE ID */
#define GC0339_WRITE_ID (0x42)
#define GC0339_READ_ID  (0x43)

/* SENSOR ID */
//#define GC0339_SENSOR_ID						(0xc8)

/* SENSOR PRIVATE STRUCT */
typedef enum {
    SENSOR_MODE_INIT = 0,
    SENSOR_MODE_PREVIEW,
    SENSOR_MODE_VIDEO,
    SENSOR_MODE_CAPTURE
} GC0339_SENSOR_MODE;

typedef enum{
	GC0339_IMAGE_NORMAL = 0,
	GC0339_IMAGE_H_MIRROR,
	GC0339_IMAGE_V_MIRROR,
	GC0339_IMAGE_HV_MIRROR
}GC0339_IMAGE_MIRROR;

typedef struct GC0339_sensor_STRUCT
{
	MSDK_SENSOR_CONFIG_STRUCT cfg_data;
	sensor_data_struct eng; /* engineer mode */
	MSDK_SENSOR_ENG_INFO_STRUCT eng_info;
	GC0339_SENSOR_MODE sensorMode;
	GC0339_IMAGE_MIRROR Mirror;
	kal_bool pv_mode;
	kal_bool video_mode;
	kal_bool NightMode;
	kal_uint16 normal_fps; /* video normal mode max fps */
	kal_uint16 night_fps; /* video night mode max fps */
	kal_uint16 FixedFps;
	kal_uint16 shutter;
	kal_uint16 gain;
	kal_uint32 pclk;
	kal_uint16 frame_height;
	kal_uint16 frame_height_BackUp;
	kal_uint16 line_length;  
	kal_uint16 Prv_line_length;
} GC0339_sensor_struct;

typedef enum GC0339_GainMode_Index
{
	GC0339_Analogic_Gain = 0,
	GC0339_Digital_Gain
}GC0339_GainMode_Index;
//export functions
UINT32 GC0339Open(void);
UINT32 GC0339Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 GC0339FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
UINT32 GC0339GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 GC0339GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
UINT32 GC0339Close(void);

#define Sleep(ms) mdelay(ms)

#endif
