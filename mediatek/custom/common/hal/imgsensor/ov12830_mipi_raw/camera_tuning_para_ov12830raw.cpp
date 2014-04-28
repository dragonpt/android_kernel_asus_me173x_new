#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov12830raw.h"
#include "camera_info_ov12830raw.h"
#include "camera_custom_AEPlinetable.h"
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    	}
    },
    ISPPca: {
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
    },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
    }
    }},
    ISPCcmPoly22:{
        69450,    // i4R_AVG
        14560,    // i4R_STD
        95975,    // i4B_AVG
        19414,    // i4B_STD
        { // i4P00[9]
            4877500, -2577500, 262500, -730000, 3400000, -112500, -72500, -2497500, 5125000
        },
        { // i4P10[9]
            947311, -1240138, 297025, 30713, 103847, -139014, 38688, -453316, 424013
        },
        { // i4P01[9]
            391730, -633838, 249639, -111494, -47591, 154011, -40245, -1164211, 1209723
        },
        { // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        { // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        { // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }        
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1195,   // u4MinGain, 1024 base =  1x
            16384,  // u4MaxGain, 16x
            100,     // u4MiniISOGain, ISOxx
            128,    // u4GainStepUnit, 1x/8
            33,     // u4PreExpUnit
            30,     // u4PreMaxFrameRate
            33,     // u4VideoExpUnit
            30,     // u4VideoMaxFrameRate
            1024,   // u4Video2PreRatio, 1024 base = 1x
            17,    // u4CapExpUnit 
            18,    // u4CapMaxFrameRate
            1024,   // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
         },
         // rHistConfig
        {
            2,   // u4HistHighThres
            40,  // u4HistLowThres
            2,   // u4MostBrightRatio
            1,   // u4MostDarkRatio
            160, // u4CentralHighBound
            20,  // u4CentralLowBound
            {240, 230, 220, 210, 200}, // u4OverExpThres[AE_CCT_STRENGTH_NUM]
            {86, 108, 128, 148, 170},  // u4HistStretchThres[AE_CCT_STRENGTH_NUM]
            {18, 22, 26, 30, 34}       // u4BlackLightThres[AE_CCT_STRENGTH_NUM]
        },
        // rCCTConfig
        {
            TRUE,            // bEnableBlackLight
            TRUE,            // bEnableHistStretch
            FALSE,           // bEnableAntiOverExposure
            TRUE,            // bEnableTimeLPF
            TRUE,            // bEnableCaptureThres
            TRUE,            // bEnableVideoThres
            TRUE,            // bEnableStrobeThres
            50,    // u4AETarget
            0,    // u4StrobeAETarget
            50,                // u4InitIndex
            4,                 // u4BackLightWeight
            32,                // u4HistStretchWeight
            4,                 // u4AntiOverExpWeight
            0,    // u4BlackLightStrengthIndex
            4,    // u4HistStretchStrengthIndex
            2,                 // u4AntiOverExpStrengthIndex
            2,                 // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8}, // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM]
            90,                // u4InDoorEV = 9.0, 10 base
            -9,    // i4BVOffset delta BV = value/10 
            64,                 // u4PreviewFlareOffset
            64,                 // u4CaptureFlareOffset
            3,                 // u4CaptureFlareThres
            64,                 // u4VideoFlareOffset
            3,                 // u4VideoFlareThres
            32,                 // u4StrobeFlareOffset
            2,                 // u4StrobeFlareThres
            160,                 // u4PrvMaxFlareThres
            0,                 // u4PrvMinFlareThres
            160,                 // u4VideoMaxFlareThres
            0,                 // u4VideoMinFlareThres            
            18,                // u4FlatnessThres              // 10 base for flatness condition.
            75                 // u4FlatnessStrength
         }
    },
    // AWB NVRAM
{								
	// AWB calibration data							
	{							
            // rUnitGain (unit gain: 1.0 = 512)
		{						
                0,    // i4R
                0,    // i4G
                0    // i4B
		},						
            // rGoldenGain (golden sample gain: 1.0 = 512)
		{						
                0,    // i4R
                0,    // i4G
                0    // i4B
		},						
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
		{						
                0,    // i4R
                0,    // i4G
                0    // i4B
		},						
		// rD65Gain (D65 WB gain: 1.0 = 512)						
		{						
                907,    // i4R
                512,    // i4G
                696    // i4B
		}						
	},							
	// Original XY coordinate of AWB light source							
	{							
		// Strobe						
		{						
                98,    // i4X
                -325    // i4Y
		},						
		// Horizon						
		{						
                -383,    // i4X
                -316    // i4Y
		},						
		// A						
		{						
                -282,    // i4X
                -338    // i4Y
		},						
		// TL84						
		{						
                -127,    // i4X
                -318    // i4Y
		},						
		// CWF						
		{						
                -134,    // i4X
                -379    // i4Y
		},						
		// DNP						
		{						
                -9,    // i4X
                -348    // i4Y
		},						
		// D65						
		{						
                98,    // i4X
                -325    // i4Y
		},						
		// DF						
		{						
                -87,    // i4X
                -358    // i4Y
		}						
	},							
	// Rotated XY coordinate of AWB light source							
	{							
		// Strobe						
		{						
                98,    // i4X
                -325    // i4Y
		},						
		// Horizon						
		{						
                -383,    // i4X
                -316    // i4Y
		},						
		// A						
		{						
                -282,    // i4X
                -338    // i4Y
		},						
		// TL84						
		{						
                -127,    // i4X
                -318    // i4Y
		},						
		// CWF						
		{						
                -134,    // i4X
                -379    // i4Y
		},						
		// DNP						
		{						
                -9,    // i4X
                -348    // i4Y
		},						
		// D65						
		{						
                98,    // i4X
                -325    // i4Y
		},						
		// DF						
		{						
                -87,    // i4X
                -358    // i4Y
		}						
	},							
	// AWB gain of AWB light source							
	{							
		// Strobe						
		{						
                907,    // i4R
                512,    // i4G
                696    // i4B
		},						
		// Horizon						
		{						
                512,    // i4R
                561,    // i4G
                1444    // i4B
		},						
		// A						
		{						
                552,    // i4R
                512,    // i4G
                1186    // i4B
		},						
		// TL84						
		{						
                663,    // i4R
                512,    // i4G
                936    // i4B
		},						
		// CWF						
		{						
                713,    // i4R
                512,    // i4G
                1027    // i4B
		},						
		// DNP						
		{						
                811,    // i4R
                512,    // i4G
                830    // i4B
		},						
		// D65						
		{						
                907,    // i4R
                512,    // i4G
                696    // i4B
		},						
		// DF						
		{						
                739,    // i4R
                512,    // i4G
                935    // i4B
		}						
	},							
	// Rotation matrix parameter							
	{							
            0,    // i4RotationAngle
            256,    // i4Cos
            0    // i4Sin
	},							
	// Daylight locus parameter							
	{							
            -128,    // i4SlopeNumerator
		128	// i4SlopeDenominator					
	},							
	// AWB light area							
	{							
            // Strobe:FIXME
		{						
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
		},						
		// Tungsten						
		{						
            -177,    // i4RightBound
            -827,    // i4LeftBound
            -277,    // i4UpperBound
            -377    // i4LowerBound
		},						
		// Warm fluorescent						
		{						
            -177,    // i4RightBound
            -827,    // i4LeftBound
            -377,    // i4UpperBound
            -497    // i4LowerBound
		},						
		// Fluorescent						
		{						
            -59,    // i4RightBound
            -177,    // i4LeftBound
            -253,    // i4UpperBound
            -348    // i4LowerBound
		},						
		// CWF						
		{						
            -59,    // i4RightBound
            -177,    // i4LeftBound
            -348,    // i4UpperBound
            -429    // i4LowerBound
		},						
		// Daylight						
		{						
            123,    // i4RightBound
            -59,    // i4LeftBound
            -245,    // i4UpperBound
            -405    // i4LowerBound
		},						
		// Shade						
		{						
            483,    // i4RightBound
            123,    // i4LeftBound
            -245,    // i4UpperBound
            -405    // i4LowerBound
		},						
		// Daylight Fluorescent						
		{						
            123,    // i4RightBound
            -59,    // i4LeftBound
            -405,    // i4UpperBound
            -565    // i4LowerBound
		}						
	},							
	// PWB light area							
	{							
		// Reference area						
		{						
            483,    // i4RightBound
            -827,    // i4LeftBound
            0,    // i4UpperBound
            -565    // i4LowerBound
		},						
		// Daylight						
		{						
            148,    // i4RightBound
            -59,    // i4LeftBound
            -245,    // i4UpperBound
            -405    // i4LowerBound
		},						
		// Cloudy daylight						
		{						
            248,    // i4RightBound
            73,    // i4LeftBound
            -245,    // i4UpperBound
            -405    // i4LowerBound
		},						
		// Shade						
		{						
            348,    // i4RightBound
            73,    // i4LeftBound
            -245,    // i4UpperBound
            -405    // i4LowerBound
		},						
		// Twilight						
		{						
            -59,    // i4RightBound
            -219,    // i4LeftBound
            -245,    // i4UpperBound
            -405    // i4LowerBound
		},						
		// Fluorescent						
		{						
            148,    // i4RightBound
            -234,    // i4LeftBound
            -268,    // i4UpperBound
            -429    // i4LowerBound
		},						
		// Warm fluorescent						
		{						
            -182,    // i4RightBound
            -382,    // i4LeftBound
            -268,    // i4UpperBound
            -429    // i4LowerBound
		},						
		// Incandescent						
		{						
            -182,    // i4RightBound
            -382,    // i4LeftBound
            -245,    // i4UpperBound
            -405    // i4LowerBound
		},						
		// Gray World						
		{						
			5000,	// i4RightBound				
			-5000,	// i4LeftBound				
			5000,	// i4UpperBound				
			-5000	// i4LowerBound				
		}						
	},							
	// PWB default gain							
	{							
		// Daylight						
		{						
            844,    // i4R
            512,    // i4G
            748    // i4B
		},						
		// Cloudy daylight						
		{						
            988,    // i4R
            512,    // i4G
            640    // i4B
		},						
		// Shade						
		{						
            1057,    // i4R
            512,    // i4G
            598    // i4B
		},						
		// Twilight						
		{						
            659,    // i4R
            512,    // i4G
            960    // i4B
		},						
		// Fluorescent						
		{						
            774,    // i4R
            512,    // i4G
            870    // i4B
		},						
		// Warm fluorescent						
		{						
            560,    // i4R
            512,    // i4G
            1202    // i4B
		},						
		// Incandescent						
		{						
            543,    // i4R
            512,    // i4G
            1165    // i4B
		},						
		// Gray World						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		}						
	},							
	// AWB preference color							
	{							
		// Tungsten						
		{						
            0,    // i4SliderValue
            6130    // i4OffsetThr
		},						
		// Warm fluorescent						
		{						
            0,    // i4SliderValue
            4746    // i4OffsetThr
		},						
		// Shade						
		{						
            0,    // i4SliderValue
            1341    // i4OffsetThr
		},						
		// Daylight WB gain						
		{						
            795,    // i4R
            512,    // i4G
            518    // i4B
		},						
		// Preference gain: strobe						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		},						
		// Preference gain: tungsten						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		},						
		// Preference gain: warm fluorescent						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		},						
		// Preference gain: fluorescent						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		},						
		// Preference gain: CWF						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		},						
		// Preference gain: daylight						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		},						
		// Preference gain: shade						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		},						
		// Preference gain: daylight fluorescent						
		{						
            512,    // i4R
            512,    // i4G
            512    // i4B
		}						
	},							
        {// CCT estimation
            {// CCT
			2300,	// i4CCT[0]				
			2850,	// i4CCT[1]				
			4100,	// i4CCT[2]				
			5100,	// i4CCT[3]				
			6500 	// i4CCT[4]				
		},						
            {// Rotated X coordinate
                -481,    // i4RotatedXCoordinate[0]
                -380,    // i4RotatedXCoordinate[1]
                -225,    // i4RotatedXCoordinate[2]
                -107,    // i4RotatedXCoordinate[3]
			0 	// i4RotatedXCoordinate[4]				
		}						
	}							
    },
	{0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T)};

    if (CameraDataType > CAMERA_DATA_AE_PLINETABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        default:
            break;
    }
    return 0;
}};  //  NSFeature


