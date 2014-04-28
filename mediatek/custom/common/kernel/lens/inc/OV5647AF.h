#ifndef _OV5647AF_H
#define _OV5647AF_H

#include <linux/ioctl.h>
//#include "kd_imgsensor.h"

#define OV5647AF_MAGIC 'A'
//IOCTRL(inode * ,file * ,cmd ,arg )


//Structures
typedef struct {
//current position
unsigned long u4CurrentPosition;
//macro position
unsigned long u4MacroPosition;
//Infiniti position
unsigned long u4InfPosition;
//Motor Status
bool          bIsMotorMoving;
//Motor Open?
bool          bIsMotorOpen;
//Support SR?
bool          bIsSupportSR;
} stOV5647AF_MotorInfo;

//Control commnad
//S means "set through a ptr"
//T means "tell by a arg value"
//G means "get by a ptr"             
//Q means "get by return a value"
//X means "switch G and S atomically"
//H means "switch T and Q atomically"
#define OV5647AFIOC_G_MOTORINFO _IOR(OV5647AF_MAGIC,0,stOV5647AF_MotorInfo)

#define OV5647AFIOC_T_MOVETO _IOW(OV5647AF_MAGIC,1,unsigned long)

#define OV5647AFIOC_T_SETINFPOS _IOW(OV5647AF_MAGIC,2,unsigned long)

#define OV5647AFIOC_T_SETMACROPOS _IOW(OV5647AF_MAGIC,3,unsigned long)

#else
#endif
