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

/*
 * MD218A voice coil motor driver
 *
 *
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/xlog.h>
#include "A5142AF.h"
#include "../camera/kd_camera_hw.h"

#define LENS_I2C_BUSNUM 1
static struct i2c_board_info __initdata kd_lens_dev={ I2C_BOARD_INFO("A5142AF", 0x6C)};

#define A5142AF_DRVNAME "A5142AF"
#define A5142AF_VCM_WRITE_ID           0x6C
#define A5142AF_DEBUG

#ifdef A5142AF_DEBUG
#define A5142AFDB printk
#else
#define A5142AFDB(x,...)
#endif

static spinlock_t g_A5142AF_SpinLock;
static struct i2c_client * g_pstA5142AF_I2Cclient = NULL;
static dev_t g_A5142AF_devno;
static struct cdev * g_pA5142AF_CharDrv = NULL;
static struct class *actuator_class = NULL;

static int  g_s4A5142AF_Opened = 0;
static long g_i4MotorStatus = 0;
static long g_i4Dir = 0;
static long g_i4Position = 0;
static unsigned long g_u4A5142AF_INF = 0;
static unsigned long g_u4A5142AF_MACRO = 1023;
static unsigned long g_u4TargetPosition = 0;
static unsigned long g_u4CurrPosition   = 0;

extern s32 mt_set_gpio_mode(u32 u4Pin, u32 u4Mode);
extern s32 mt_set_gpio_out(u32 u4Pin, u32 u4PinOut);
extern s32 mt_set_gpio_dir(u32 u4Pin, u32 u4Dir);

extern void A5142MIPI_write_cmos_sensor(kal_uint32 addr, kal_uint32 para);
extern kal_uint16 A5142MIPI_read_cmos_sensor(kal_uint32 addr);

static int s4A5142AF_ReadReg(unsigned short * a_pu2Result)
{
    *a_pu2Result = A5142MIPI_read_cmos_sensor(0x30f2)<<2;
    return 0;
}

static int s4A5142AF_WriteReg(u16 a_u2Data)
{
	a_u2Data = a_u2Data >>2;
	if(a_u2Data == 0)
		return 0 ;
	
	A5142MIPI_write_cmos_sensor(0x30f2,a_u2Data);
	
    return 0;
}



inline static int getA5142AFInfo(__user stA5142AF_MotorInfo * pstMotorInfo)
{
    stA5142AF_MotorInfo stMotorInfo;
    stMotorInfo.u4MacroPosition   = g_u4A5142AF_MACRO;
    stMotorInfo.u4InfPosition     = g_u4A5142AF_INF;
    stMotorInfo.u4CurrentPosition = g_u4CurrPosition;
	if (g_i4MotorStatus == 1)	{stMotorInfo.bIsMotorMoving = TRUE;}
	else						{stMotorInfo.bIsMotorMoving = FALSE;}

	if (g_s4A5142AF_Opened >= 1)	{stMotorInfo.bIsMotorOpen = TRUE;}
	else						{stMotorInfo.bIsMotorOpen = FALSE;}

    if(copy_to_user(pstMotorInfo , &stMotorInfo , sizeof(stA5142AF_MotorInfo)))
    {
        A5142AFDB("[A5142AF] copy to user failed when getting motor information \n");
    }

    return 0;
}

inline static int moveA5142AF(unsigned long a_u4Position)
{
    if((a_u4Position > g_u4A5142AF_MACRO) || (a_u4Position < g_u4A5142AF_INF))
    {
        A5142AFDB("[A5142AF] out of range \n");
        return -EINVAL;
    }

	if (g_s4A5142AF_Opened == 1)
	{
		unsigned short InitPos;
	
		if(s4A5142AF_ReadReg(&InitPos) == 0)
		{
			A5142AFDB("[A5142AF] Init Pos %6d \n", InitPos);
		
			g_u4CurrPosition = (unsigned long)InitPos;
		}
		else
		{
			g_u4CurrPosition = 0;
		}
		
		g_s4A5142AF_Opened = 2;
	}

	if      (g_u4CurrPosition < a_u4Position)	{g_i4Dir = 1;}
	else if (g_u4CurrPosition > a_u4Position)	{g_i4Dir = -1;}
	else										{return 0;}

	if (1)
	{
		g_i4Position = (long)g_u4CurrPosition;
		g_u4TargetPosition = a_u4Position;

		if (g_i4Dir == 1)
		{
			//if ((g_u4TargetPosition - g_u4CurrPosition)<60)
			{		
				g_i4MotorStatus = 0;
				if(s4A5142AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
				{
					g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
				}
				else
				{
					A5142AFDB("[A5142AF] set I2C failed when moving the motor \n");
					g_i4MotorStatus = -1;
				}
			}
			//else
			//{
			//	g_i4MotorStatus = 1;
			//}
		}
		else if (g_i4Dir == -1)
		{
			//if ((g_u4CurrPosition - g_u4TargetPosition)<60)
			{
				g_i4MotorStatus = 0;		
				if(s4A5142AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
				{
					g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
				}
				else
				{
					A5142AFDB("[A5142AF] set I2C failed when moving the motor \n");
					g_i4MotorStatus = -1;
				}
			}
			//else
			//{
			//	g_i4MotorStatus = 1;		
			//}
		}
	}
	else
	{
	g_i4Position = (long)g_u4CurrPosition;
	g_u4TargetPosition = a_u4Position;
	g_i4MotorStatus = 1;
	}

    return 0;
}

inline static int setA5142AFInf(unsigned long a_u4Position)
{
	g_u4A5142AF_INF = a_u4Position;
	return 0;
}

inline static int setA5142AFMacro(unsigned long a_u4Position)
{
	g_u4A5142AF_MACRO = a_u4Position;
	return 0;	
}

////////////////////////////////////////////////////////////////
static long A5142AF_Ioctl(
struct file * a_pstFile,
unsigned int a_u4Command,
unsigned long a_u4Param)
{
    long i4RetValue = 0;

    switch(a_u4Command)
    {
        case A5142AFIOC_G_MOTORINFO :
            i4RetValue = getA5142AFInfo((__user stA5142AF_MotorInfo *)(a_u4Param));
        break;

        case A5142AFIOC_T_MOVETO :
            i4RetValue = moveA5142AF(a_u4Param);
        break;
 
 		case A5142AFIOC_T_SETINFPOS :
			 i4RetValue = setA5142AFInf(a_u4Param);
		break;

 		case A5142AFIOC_T_SETMACROPOS :
			 i4RetValue = setA5142AFMacro(a_u4Param);
		break;
		
        default :
      	     A5142AFDB("[A5142AF] No CMD \n");
            i4RetValue = -EPERM;
        break;
    }

    return i4RetValue;
}

//Main jobs:
// 1.check for device-specified errors, device not ready.
// 2.Initialize the device if it is opened for the first time.
// 3.Update f_op pointer.
// 4.Fill data structures into private_data
//CAM_RESET
static int A5142AF_Open(struct inode * a_pstInode, struct file * a_pstFile)
{
    spin_lock(&g_A5142AF_SpinLock);

    if(g_s4A5142AF_Opened)
    {
        spin_unlock(&g_A5142AF_SpinLock);
        A5142AFDB("[A5142AF] the device is opened \n");
        return -EBUSY;
    }

    g_s4A5142AF_Opened = 1;
		
    spin_unlock(&g_A5142AF_SpinLock);

    return 0;
}

//Main jobs:
// 1.Deallocate anything that "open" allocated in private_data.
// 2.Shut down the device on last close.
// 3.Only called once on last time.
// Q1 : Try release multiple times.
static int A5142AF_Release(struct inode * a_pstInode, struct file * a_pstFile)
{
	unsigned int cnt = 0;

	if (g_s4A5142AF_Opened)
	{
		moveA5142AF(g_u4A5142AF_INF);

		while(g_i4MotorStatus)
		{
			msleep(1);
			cnt++;
			if (cnt>1000)	{break;}
		}
		
    	spin_lock(&g_A5142AF_SpinLock);

	    g_s4A5142AF_Opened = 0;

    	spin_unlock(&g_A5142AF_SpinLock);

    	//hwPowerDown(CAMERA_POWER_VCAM_A,"kd_camera_hw");

		//XGPT_Stop(g_GPTconfig.num);
	}

    return 0;
}

static const struct file_operations g_stA5142AF_fops = 
{
    .owner = THIS_MODULE,
    .open = A5142AF_Open,
    .release = A5142AF_Release,
    .unlocked_ioctl = A5142AF_Ioctl
};

inline static int Register_A5142AF_CharDrv(void)
{
    struct device* vcm_device = NULL;

    //Allocate char driver no.
    if( alloc_chrdev_region(&g_A5142AF_devno, 0, 1,A5142AF_DRVNAME) )
    {
        A5142AFDB("[A5142AF] Allocate device no failed\n");

        return -EAGAIN;
    }

    //Allocate driver
    g_pA5142AF_CharDrv = cdev_alloc();

    if(NULL == g_pA5142AF_CharDrv)
    {
        unregister_chrdev_region(g_A5142AF_devno, 1);

        A5142AFDB("[A5142AF] Allocate mem for kobject failed\n");

        return -ENOMEM;
    }

    //Attatch file operation.
    cdev_init(g_pA5142AF_CharDrv, &g_stA5142AF_fops);

    g_pA5142AF_CharDrv->owner = THIS_MODULE;

    //Add to system
    if(cdev_add(g_pA5142AF_CharDrv, g_A5142AF_devno, 1))
    {
        A5142AFDB("[A5142AF] Attatch file operation failed\n");

        unregister_chrdev_region(g_A5142AF_devno, 1);

        return -EAGAIN;
    }

    actuator_class = class_create(THIS_MODULE, "actuatordrv4");
    if (IS_ERR(actuator_class)) {
        int ret = PTR_ERR(actuator_class);
        A5142AFDB("Unable to create class, err = %d\n", ret);
        return ret;            
    }

    vcm_device = device_create(actuator_class, NULL, g_A5142AF_devno, NULL, A5142AF_DRVNAME);

    if(NULL == vcm_device)
    {
        return -EIO;
    }
    
    return 0;
}

inline static void Unregister_A5142AF_CharDrv(void)
{
    //Release char driver
    cdev_del(g_pA5142AF_CharDrv);

    unregister_chrdev_region(g_A5142AF_devno, 1);
    
    device_destroy(actuator_class, g_A5142AF_devno);

    class_destroy(actuator_class);
}

/* Kirby: add new-style driver { */
static int A5142AF_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info);
static int A5142AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int A5142AF_i2c_remove(struct i2c_client *client);
static const struct i2c_device_id A5142AF_i2c_id[] = {{A5142AF_DRVNAME,0},{}};   
struct i2c_driver A5142AF_i2c_driver = {                       
    .probe = A5142AF_i2c_probe,                                   
    .remove = A5142AF_i2c_remove,                                              
    .driver.name = A5142AF_DRVNAME,                 
    .id_table = A5142AF_i2c_id,                             
};  

#if 0
static int A5142AF_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) {         
    strcpy(info->type, A5142AF_DRVNAME);                                                         
    return 0;                                                                                       
}    
#endif
                                                                                              
static int A5142AF_i2c_remove(struct i2c_client *client) {
    return 0;
}

/* Kirby: add new-style driver {*/
static int A5142AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int i4RetValue = 0;

    A5142AFDB("[A5142AF] Attach I2C \n");

    /* Kirby: add new-style driver { */
    g_pstA5142AF_I2Cclient = client;
    /* Kirby: } */

    //Register char driver
    i4RetValue = Register_A5142AF_CharDrv();

    if(i4RetValue){
        A5142AFDB("[A5142AF] register char device failed!\n");
        return i4RetValue;
    }

    spin_lock_init(&g_A5142AF_SpinLock);

    A5142AFDB("[A5142AF] Attached!! \n");

    return 0;
}

static int A5142AF_probe(struct platform_device *pdev)
{
	A5142AFDB("A5142AF_probe\n");

    return i2c_add_driver(&A5142AF_i2c_driver);
}

static int A5142AF_remove(struct platform_device *pdev)
{
    i2c_del_driver(&A5142AF_i2c_driver);
    return 0;
}

static int A5142AF_suspend(struct platform_device *pdev, pm_message_t mesg)
{
    return 0;
}

static int A5142AF_resume(struct platform_device *pdev)
{
    return 0;
}

// platform structure
static struct platform_driver g_stA5142AF_Driver = {
    .probe		= A5142AF_probe,
    .remove	= A5142AF_remove,
    .suspend	= A5142AF_suspend,
    .resume	= A5142AF_resume,
    .driver		= {
        .name	= "lens_actuator4",
        .owner	= THIS_MODULE,
    }
};

static int __init A5142AF_i2C_init(void)
{
	A5142AFDB("A5142AF_i2C_init\n");
 //i2c_register_board_info(LENS_I2C_BUSNUM, &kd_lens_dev, 1);
    if(platform_driver_register(&g_stA5142AF_Driver)){
        A5142AFDB("failed to register A5142AF driver\n");
        return -ENODEV;
    }

    return 0;
}

static void __exit A5142AF_i2C_exit(void)
{
	platform_driver_unregister(&g_stA5142AF_Driver);
}

module_init(A5142AF_i2C_init);
module_exit(A5142AF_i2C_exit);

MODULE_DESCRIPTION("A5142AF lens module driver");
MODULE_AUTHOR("Gipi Lin <Gipi.Lin@Mediatek.com>");
MODULE_LICENSE("GPL");


