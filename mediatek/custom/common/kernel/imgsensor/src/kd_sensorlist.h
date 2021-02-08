//s_add new sensor driver here
//export funtions
//UINT32 EXISP_MAIN_MIPI_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
//UINT32 EXISP_SUB_MIPI_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 A5142_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 HM2056_MIPI_RAWSensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 MT9M114_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 GC0339_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);



//! Add Sensor Init function here
//! Note:
//! 1. Add by the resolution from ""large to small"", due to large sensor
//!    will be possible to be main sensor.
//!    This can avoid I2C error during searching sensor.
//! 2. This file should be the same as mediatek\custom\common\hal\imgsensor\src\sensorlist.cpp
ACDK_KD_SENSOR_INIT_FUNCTION_STRUCT kdSensorList[MAX_NUM_OF_SUPPORT_SENSOR+1] =
{
//#if defined(EXISP_MAIN_MIPI_YUV)
//    {EXISPMAIN_SENSOR_ID, SENSOR_DRVNAME_EXISP_MAIN_MIPI_YUV, EXISP_MAIN_MIPI_YUV_SensorInit},
//#endif
//#if defined(EXISP_SUB_MIPI_YUV)
//    {EXISPSUB_SENSOR_ID, SENSOR_DRVNAME_EXISP_SUB_MIPI_YUV, EXISP_SUB_MIPI_YUV_SensorInit},
//#endif
#if defined(A5142_MIPI_RAW)
  {A5142MIPI_SENSOR_ID, SENSOR_DRVNAME_A5142_MIPI_RAW,A5142_MIPI_RAW_SensorInit},
#endif
#if defined(HM2056_MIPI_RAW)
    {HM2056MIPI_SENSOR_ID, SENSOR_DRVNAME_HM2056_MIPI_RAW,HM2056_MIPI_RAWSensorInit},
#endif
#if defined(MT9M114_MIPI_RAW)
		{MT9M114MIPI_SENSOR_ID, SENSOR_DRVNAME_MT9M114_MIPI_RAW,MT9M114_MIPI_RAW_SensorInit},
#endif
#if defined(GC0339_RAW)
    {GC0339_SENSOR_ID, SENSOR_DRVNAME_GC0339_RAW, GC0339_RAW_SensorInit},
#endif



/*  ADD sensor driver before this line */
    {0,{0},NULL}, //end of list
};
//e_add new sensor driver here
