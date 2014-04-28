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
#include "AR0833AF.h"
#include <linux/xlog.h>

#include "../camera/kd_camera_hw.h"

#define LENS_I2C_BUSNUM 1
static struct i2c_board_info __initdata kd_lens_dev={ I2C_BOARD_INFO("AR0833AF", 0x1D)};


#define AR0833AF_DRVNAME "AR0833AF"
#define AR0833AF_VCM_WRITE_ID           0x6c

#define AR0833DB  printk //(fmt, arg...) printkxlog_printk(ANDROID_LOG_DEBUG, "[AR0833Raw] ",  fmt, ##arg)

static spinlock_t g_AR0833AF_SpinLock;

extern kal_uint16 AR0833_read_cmos_sensor(kal_uint32 addr);
extern void AR0833_write_cmos_sensor(kal_uint32 addr, kal_uint32 para);


static struct i2c_client * g_pstAR0833AF_I2Cclient = NULL;

static dev_t g_AR0833AF_devno;
static struct cdev * g_pAR0833AF_CharDrv = NULL;
static struct class *actuator_class = NULL;

static int  g_s4AR0833AF_Opened = 0;
static long g_i4MotorStatus = 0;
static long g_i4Dir = 0;
static unsigned long g_u4AR0833AF_INF = 0;
static unsigned long g_u4AR0833AF_MACRO = 1023;
static unsigned long g_u4TargetPosition = 0;
static unsigned long g_u4CurrPosition   = 0;

static int g_sr = 3;

extern s32 mt_set_gpio_mode(u32 u4Pin, u32 u4Mode);
extern s32 mt_set_gpio_out(u32 u4Pin, u32 u4PinOut);
extern s32 mt_set_gpio_dir(u32 u4Pin, u32 u4Dir);


static int s4AR0833AF_ReadReg(unsigned short * a_pu2Result)
{
    int  temp = 0;
    //char pBuff[2];

    temp = AR0833_read_cmos_sensor(0x30F2);

    *a_pu2Result = temp*4;
	AR0833DB("s4AR0833AF_ReadReg = %d \n", temp);
    return 0;
}

static int s4AR0833AF_WriteReg(u16 a_u2Data)
{
    AR0833DB("s4AR0833AF_WriteReg = %d \n", a_u2Data);
		
	AR0833_write_cmos_sensor(0x30F2, a_u2Data>>2);

	//AR0833DB("s4AR0833AF_WriteReg = %d \n", a_u2Data);

    return 0;
}

inline static int getAR0833AFInfo(__user stAR0833AF_MotorInfo * pstMotorInfo)
{
    stAR0833AF_MotorInfo stMotorInfo;
    stMotorInfo.u4MacroPosition   = g_u4AR0833AF_MACRO;
    stMotorInfo.u4InfPosition     = g_u4AR0833AF_INF;
    stMotorInfo.u4CurrentPosition = g_u4CurrPosition;
	if (g_i4MotorStatus == 1)	{stMotorInfo.bIsMotorMoving = 1;}
	else						{stMotorInfo.bIsMotorMoving = 0;}

	if (g_s4AR0833AF_Opened >= 1)	{stMotorInfo.bIsMotorOpen = 1;}
	else						{stMotorInfo.bIsMotorOpen = 0;}

    if(copy_to_user(pstMotorInfo , &stMotorInfo , sizeof(stAR0833AF_MotorInfo)))
    {
        AR0833DB("[AR0833AF] copy to user failed when getting motor information \n");
    }

    return 0;
}

inline static int moveAR0833AF(unsigned long a_u4Position)
{
    int ret = 0;
    
    if((a_u4Position > g_u4AR0833AF_MACRO) || (a_u4Position < g_u4AR0833AF_INF))
    {
        AR0833DB("[AR0833AF] out of range \n");
        return -EINVAL;
    }

    if (g_s4AR0833AF_Opened == 1)
    {
        unsigned short InitPos;
        ret = s4AR0833AF_ReadReg(&InitPos);
	    
        spin_lock(&g_AR0833AF_SpinLock);
        if(ret == 0)
        {
            AR0833DB("[AR0833AF] Init Pos %6d \n", InitPos);
            g_u4CurrPosition = (unsigned long)InitPos;
        }
        else
        {		
            g_u4CurrPosition = 0;
        }
        g_s4AR0833AF_Opened = 2;
        spin_unlock(&g_AR0833AF_SpinLock);
    }

    if (g_u4CurrPosition < a_u4Position)
    {
        spin_lock(&g_AR0833AF_SpinLock);	
        g_i4Dir = 1;
        spin_unlock(&g_AR0833AF_SpinLock);	
    }
    else if (g_u4CurrPosition > a_u4Position)
    {
        spin_lock(&g_AR0833AF_SpinLock);	
        g_i4Dir = -1;
        spin_unlock(&g_AR0833AF_SpinLock);			
    }
    else										{return 0;}

    spin_lock(&g_AR0833AF_SpinLock);    
    g_u4TargetPosition = a_u4Position;
    spin_unlock(&g_AR0833AF_SpinLock);	

    //AR0833DB("[AR0833AF] move [curr] %d [target] %d\n", g_u4CurrPosition, g_u4TargetPosition);

            spin_lock(&g_AR0833AF_SpinLock);
            g_sr = 3;
            g_i4MotorStatus = 0;
            spin_unlock(&g_AR0833AF_SpinLock);	
		
            if(s4AR0833AF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
            {
                spin_lock(&g_AR0833AF_SpinLock);		
                g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
                spin_unlock(&g_AR0833AF_SpinLock);				
            }
            else
            {
                AR0833DB("[AR0833AF] set I2C failed when moving the motor \n");			
                spin_lock(&g_AR0833AF_SpinLock);
                g_i4MotorStatus = -1;
                spin_unlock(&g_AR0833AF_SpinLock);				
            }

    return 0;
}

inline static int setAR0833AFInf(unsigned long a_u4Position)
{
    spin_lock(&g_AR0833AF_SpinLock);
    g_u4AR0833AF_INF = a_u4Position;
    spin_unlock(&g_AR0833AF_SpinLock);	
    return 0;
}

inline static int setAR0833AFMacro(unsigned long a_u4Position)
{
    spin_lock(&g_AR0833AF_SpinLock);
    g_u4AR0833AF_MACRO = a_u4Position;
    spin_unlock(&g_AR0833AF_SpinLock);	
    return 0;	
}

////////////////////////////////////////////////////////////////
static long AR0833AF_Ioctl(
struct file * a_pstFile,
unsigned int a_u4Command,
unsigned long a_u4Param)
{
    long i4RetValue = 0;

    switch(a_u4Command)
    {
        case AR0833AFIOC_G_MOTORINFO :
            i4RetValue = getAR0833AFInfo((__user stAR0833AF_MotorInfo *)(a_u4Param));
			AR0833DB("[AR0833AF] getAR0833AFInfo \n");
        break;

        case AR0833AFIOC_T_MOVETO :
            i4RetValue = moveAR0833AF(a_u4Param);
			AR0833DB("[AR0833AF] moveAR0833AF \n");
        break;
 
        case AR0833AFIOC_T_SETINFPOS :
            i4RetValue = setAR0833AFInf(a_u4Param);
			AR0833DB("[AR0833AF] setAR0833AFInf \n");
        break;

        case AR0833AFIOC_T_SETMACROPOS :
            i4RetValue = setAR0833AFMacro(a_u4Param);
			AR0833DB("[AR0833AF] setAR0833AFMacro \n");
        break;
		
        default :
      	    AR0833DB("[AR0833AF] No CMD \n");
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
static int AR0833AF_Open(struct inode * a_pstInode, struct file * a_pstFile)
{
    spin_lock(&g_AR0833AF_SpinLock);
    AR0833DB("[AR0833AF] AR0833AF_Open \n");

    if(g_s4AR0833AF_Opened)
    {
        spin_unlock(&g_AR0833AF_SpinLock);
        AR0833DB("[AR0833AF] the device is opened \n");
        return -EBUSY;
    }

    g_s4AR0833AF_Opened = 1;
		
    spin_unlock(&g_AR0833AF_SpinLock);
	
    //AR0833_write_cmos_sensor(0x30F4, 0x0000);   // vcm_step_time (R/W)
	//Programmable counter to define how many system clocks for each step time.
	//vcm step time = Tsysclk * 16 * (vcm_step_time[15:0] + 1)
	//Legal values: [0, 65535].

    //AR0833_write_cmos_sensor(0x30F0, 0x8000);   // vcm_control (R/W)
    //bit15: en,
    //bit3:VCM is not disabled in standby state,
    //bit2~0:vcm_slew=0: mode 0, refresh the code directly to target code
	//       vcm_slew>0: mode 1, increment/decrement 1 code every step transition time to target code step transition time = Tsysclk
	AR0833_write_cmos_sensor(0x30F0, 0x8000);

    return 0;
}

//Main jobs:
// 1.Deallocate anything that "open" allocated in private_data.
// 2.Shut down the device on last close.
// 3.Only called once on last time.
// Q1 : Try release multiple times.
static int AR0833AF_Release(struct inode * a_pstInode, struct file * a_pstFile)
{
    if (g_s4AR0833AF_Opened)
    {
        AR0833DB("[AR0833AF] feee \n");
        g_sr = 5;

        if (g_u4CurrPosition > 700)  {
            s4AR0833AF_WriteReg(700);
            msleep(3);
        }

        if (g_u4CurrPosition > 600)  {
            s4AR0833AF_WriteReg(600);
            msleep(3);
        }

        if (g_u4CurrPosition > 500)  {
            s4AR0833AF_WriteReg(500);
            msleep(3);
        }

        if (g_u4CurrPosition > 400)  {
            s4AR0833AF_WriteReg(400);
            msleep(3);
        }

        if (g_u4CurrPosition > 300)  {
            s4AR0833AF_WriteReg(300);
            msleep(3);
        }

        if (g_u4CurrPosition > 200)  {
	        s4AR0833AF_WriteReg(200);
            msleep(3);
        }

        if (g_u4CurrPosition > 100)   {
	        s4AR0833AF_WriteReg(100);
            msleep(3);
        }
            	            	    	    
        spin_lock(&g_AR0833AF_SpinLock);
        g_s4AR0833AF_Opened = 0;
        spin_unlock(&g_AR0833AF_SpinLock);
		AR0833_write_cmos_sensor(0x30F0, 0x0000);
    }

    return 0;
}

static const struct file_operations g_stAR0833AF_fops = 
{
    .owner = THIS_MODULE,
    .open = AR0833AF_Open,
    .release = AR0833AF_Release,
    .unlocked_ioctl = AR0833AF_Ioctl
};

inline static int Register_AR0833AF_CharDrv(void)
{
    struct device* vcm_device = NULL;

    //Allocate char driver no.
    if( alloc_chrdev_region(&g_AR0833AF_devno, 0, 1,AR0833AF_DRVNAME) )
    {
        AR0833DB("[AR0833AF] Allocate device no failed\n");
        return -EAGAIN;
    }

    //Allocate driver
    g_pAR0833AF_CharDrv = cdev_alloc();

    if(NULL == g_pAR0833AF_CharDrv)
    {
        unregister_chrdev_region(g_AR0833AF_devno, 1);

        AR0833DB("[AR0833AF] Allocate mem for kobject failed\n");

        return -ENOMEM;
    }

    //Attatch file operation.
    cdev_init(g_pAR0833AF_CharDrv, &g_stAR0833AF_fops);

    g_pAR0833AF_CharDrv->owner = THIS_MODULE;

    //Add to system
    if(cdev_add(g_pAR0833AF_CharDrv, g_AR0833AF_devno, 1))
    {
        AR0833DB("[AR0833AF] Attatch file operation failed\n");

        unregister_chrdev_region(g_AR0833AF_devno, 1);

        return -EAGAIN;
    }

    actuator_class = class_create(THIS_MODULE, "actuatordrv5");
    if (IS_ERR(actuator_class)) {
        int ret = PTR_ERR(actuator_class);
        AR0833DB("Unable to create class, err = %d\n", ret);
        return ret;            
    }

    vcm_device = device_create(actuator_class, NULL, g_AR0833AF_devno, NULL, AR0833AF_DRVNAME);

    if(NULL == vcm_device)
    {
        return -EIO;
    }
    
    return 0;
}

inline static void Unregister_AR0833AF_CharDrv(void)
{
    //Release char driver
    cdev_del(g_pAR0833AF_CharDrv);

    unregister_chrdev_region(g_AR0833AF_devno, 1);
    
    device_destroy(actuator_class, g_AR0833AF_devno);

    class_destroy(actuator_class);
}

//////////////////////////////////////////////////////////////////////

static int AR0833AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int AR0833AF_i2c_remove(struct i2c_client *client);
static const struct i2c_device_id AR0833AF_i2c_id[] = {{AR0833AF_DRVNAME,0},{}};   
struct i2c_driver AR0833AF_i2c_driver = {                       
    .probe = AR0833AF_i2c_probe,                                   
    .remove = AR0833AF_i2c_remove,                           
    .driver.name = AR0833AF_DRVNAME,                 
    .id_table = AR0833AF_i2c_id,                             
};  

#if 0 
static int AR0833AF_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) {         
    strcpy(info->type, AR0833AF_DRVNAME);                                                         
    return 0;                                                                                       
}      
#endif 
static int AR0833AF_i2c_remove(struct i2c_client *client) {
    return 0;
}

/* Kirby: add new-style driver {*/
static int AR0833AF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int i4RetValue = 0;

    /* Kirby: add new-style driver { */
    g_pstAR0833AF_I2Cclient = client;
    
    //g_pstAR0833AF_I2Cclient->addr = g_pstAR0833AF_I2Cclient->addr >> 1;
    
    //Register char driver
    i4RetValue = Register_AR0833AF_CharDrv();

    if(i4RetValue){

        AR0833DB("[AR0833AF] register char device failed!\n");

        return i4RetValue;
    }

    spin_lock_init(&g_AR0833AF_SpinLock);

    AR0833DB("[AR0833AF] Attached!! \n");

    return 0;
}

static int AR0833AF_probe(struct platform_device *pdev)
{
    AR0833DB("[AR0833AF] AR0833AF_probe!! \n");

    return i2c_add_driver(&AR0833AF_i2c_driver);
}

static int AR0833AF_remove(struct platform_device *pdev)
{
    i2c_del_driver(&AR0833AF_i2c_driver);
    return 0;
}

static int AR0833AF_suspend(struct platform_device *pdev, pm_message_t mesg)
{
    return 0;
}

static int AR0833AF_resume(struct platform_device *pdev)
{
    return 0;
}

// platform structure
static struct platform_driver g_stAR0833AF_Driver = {
    .probe		= AR0833AF_probe,
    .remove	= AR0833AF_remove,
    .suspend	= AR0833AF_suspend,
    .resume	= AR0833AF_resume,
    .driver		= {
        .name	= "lens_actuator5",
        .owner	= THIS_MODULE,
    }
};

static int __init AR0833AF_i2C_init(void)
{
	// i2c_register_board_info(LENS_I2C_BUSNUM, &kd_lens_dev, 1);
	AR0833DB("[AR0833AF] AR0833AF_i2C_init!! \n");

	if(platform_driver_register(&g_stAR0833AF_Driver)){
        AR0833DB("failed to register AR0833AF driver\n");
        return -ENODEV;
    }

    return 0;
}

static void __exit AR0833AF_i2C_exit(void)
{
	platform_driver_unregister(&g_stAR0833AF_Driver);
}

module_init(AR0833AF_i2C_init);
module_exit(AR0833AF_i2C_exit);

MODULE_DESCRIPTION("AR0833AF lens module driver");
MODULE_AUTHOR("KY Chen <ky.chen@Mediatek.com>");
MODULE_LICENSE("GPL");


