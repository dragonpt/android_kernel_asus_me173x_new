#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/time.h>
#include "kd_flashlight.h"
#include <asm/io.h>
#include <asm/uaccess.h>
#include "kd_camera_hw.h"
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/xlog.h>
#include <mach/upmu_common.h>

#include <asm/delay.h>
#include <linux/delay.h>


/******************************************************************************
 * Debug configuration
******************************************************************************/
// availible parameter
// ANDROID_LOG_ASSERT
// ANDROID_LOG_ERROR
// ANDROID_LOG_WARNING
// ANDROID_LOG_INFO
// ANDROID_LOG_DEBUG
// ANDROID_LOG_VERBOSE
//#define TAG_NAME "leds_strobe.c"
//#define PK_DBG_NONE(fmt, arg...)    do {} while (0)
//#define PK_DBG_FUNC(fmt, arg...)    printk( TAG_NAME, KERN_INFO  "%s: " fmt, __FUNCTION__ ,##arg)
//#define PK_WARN(fmt, arg...)        xlog_printk(TAG_NAME, KERN_WARNING  "%s: " fmt, __FUNCTION__ ,##arg)
//#define PK_NOTICE(fmt, arg...)      xlog_printk( TAG_NAME, KERN_NOTICE  "%s: " fmt, __FUNCTION__ ,##arg)
//#define PK_INFO(fmt, arg...)        xlog_printk( TAG_NAME, KERN_INFO  "%s: " fmt, __FUNCTION__ ,##arg)
//#define PK_TRC_FUNC(f)              xlog_printk(ANDROID_LOG_DEBUG  , TAG_NAME,  "<%s>\n", __FUNCTION__);
//#define PK_TRC_VERBOSE(fmt, arg...) xlog_printk(ANDROID_LOG_VERBOSE, TAG_NAME,  fmt, ##arg)
//#define PK_ERROR(fmt, arg...)       xlog_printk(ANDROID_LOG_ERROR  , TAG_NAME, KERN_ERR "%s: " fmt, __FUNCTION__ ,##arg)


//#define DEBUG_LEDS_STROBE
//#ifdef  DEBUG_LEDS_STROBE
	#define PK_DBG printk
	#define PK_VER printk
	#define PK_ERR printk
//#else
//	#define PK_DBG(a,...)
//	#define PK_VER(a,...)
//	#define PK_ERR(a,...)
//#endif

/******************************************************************************
 * local variables
******************************************************************************/
static DEFINE_SPINLOCK(g_strobeSMPLock);
static u32 strobe_Res = 0;
static BOOL g_strobe_On = 0;
static int g_duty=-1;
static int g_step=-1;
static int g_timeOutTimeMs=0;

static struct work_struct workTimeOut;

// ====================fenggy add ========================
static DEFINE_SPINLOCK(g_timer_600ms_lock);
static struct work_struct workTimeOut_600ms;
static struct hrtimer g_timeOutTimer_600ms;
static ktime_t ktime_600ms;
static int flash_led_close_flag=0;
static int flash_led_cnt=0;

void timerStart_600ms();

static void work_timeOutFunc_600ms(struct work_struct *data)
{
    PK_DBG("work_timeOutFunc_600ms flash_led_cnt=%d\n",flash_led_cnt);
	if(1==flash_led_close_flag)
	{
		flash_led_cnt++;
		if(flash_led_cnt>1)
		{
			flash_led_cnt = 10;
			PK_DBG("flash_led_close_flag = 1 flash_led_cnt>2\n");
			mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ZERO);
			mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ONE);	
			timerStart_600ms();
		}
		else
		{
			PK_DBG("flash_led_close_flag = 1 flash_led_cnt<=2\n");
			mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ZERO);
			mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ZERO);
			udelay(30);
			mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ZERO);
			mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ONE);	
			timerStart_600ms();
		}
	}
	else
	{
//		flash_led_cnt = 0;
		PK_DBG("flash_led_close_flag = 0\n");
		mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ZERO);
		mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ZERO);
//		hrtimer_cancel(&g_timeOutTimer_600ms);
	}
}

enum hrtimer_restart ledTimeOutCallback_600ms(struct hrtimer *timer)
{
	PK_DBG("ledTimeOutCallback_600ms\n");
	
	schedule_work(&workTimeOut_600ms);

    return HRTIMER_NORESTART;
}


void timerInit_600ms(void)
{
	flash_led_cnt =0;
	flash_led_close_flag=0;
	hrtimer_init( &g_timeOutTimer_600ms, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	g_timeOutTimer_600ms.function=ledTimeOutCallback_600ms;

}

void timerStart_600ms()
{
	PK_DBG("timerStart_600ms\n");
	ktime_600ms = ktime_set( 0, 600*1000000 );
	hrtimer_start( &g_timeOutTimer_600ms, ktime_600ms, HRTIMER_MODE_REL );

}

//===================fenggy add end=======================


/*****************************************************************************
Functions
*****************************************************************************/
static void work_timeOutFunc(struct work_struct *data);

int FL_enable(void)
{
	PK_DBG("FL_enable g_step=%d\n",g_duty);
//	upmu_set_rg_bst_drv_1m_ck_pdn(0);
//	upmu_set_flash_en(1);
	if(g_duty >2 )
	{	
		if(flash_led_close_flag == 0)
		{
			flash_led_cnt =0;
			flash_led_close_flag = 1;
			timerStart_600ms();
		mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ZERO);
		mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ONE);		
	}
	}
	else
	{
		mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ZERO);
		mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ONE);	
	}

    return 0;
}

int FL_disable(void)
{
	PK_DBG("FL_disable\n");
//	upmu_set_flash_en(0);
	//upmu_set_rg_bst_drv_1m_ck_pdn(1);
//	flash_led_close_flag = 0;
	mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ZERO);
	mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ZERO);
	if(flash_led_close_flag==1)
	{
		flash_led_close_flag = 0;
		cancel_work_sync(&workTimeOut_600ms);
		hrtimer_cancel(&g_timeOutTimer_600ms);
	}
    return 0;
}

int FL_dim_duty(kal_uint32 duty)
{
	PK_DBG("FL_dim_duty\n");	
//	upmu_set_flash_dim_duty(duty);
    return 0;
}

int FL_step(kal_uint32 step)
{
	PK_DBG("FL_step\n");		
//	int sTab[8]={0,2,4,6,9,11,13,15};
//	upmu_set_flash_sel(sTab[step]);
    return 0;
}

int FL_init(void)
{
	PK_DBG("FL_init\n");
//	upmu_set_flash_dim_duty(0);
//	upmu_set_flash_sel(0);
    mt_set_gpio_mode(GPIO_TORCH_EN_PIN, GPIO_TORCH_EN_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_TORCH_EN_PIN, GPIO_DIR_OUT);	
	mt_set_gpio_out(GPIO_TORCH_EN_PIN, GPIO_OUT_ZERO);

    mt_set_gpio_mode(GPIO_FLASH_EN_PIN, GPIO_FLASH_EN_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_FLASH_EN_PIN, GPIO_DIR_OUT);	
	mt_set_gpio_out(GPIO_FLASH_EN_PIN, GPIO_OUT_ZERO);	
//	FL_disable();
	INIT_WORK(&workTimeOut, work_timeOutFunc);
//fenggy add 
	INIT_WORK(&workTimeOut_600ms, work_timeOutFunc_600ms);
//fenggy add end
    return 0;
}


int FL_uninit(void)
{
	PK_DBG("FL_uninit\n");
	FL_disable();
    return 0;
}

/*****************************************************************************
User interface
*****************************************************************************/


static void work_timeOutFunc(struct work_struct *data)
{
	FL_disable();
    PK_DBG("ledTimeOut_callback\n");
    //printk(KERN_ALERT "work handler function./n");
}
enum hrtimer_restart ledTimeOutCallback(struct hrtimer *timer)
{
	PK_DBG("ledTimeOut_callback\n");
	schedule_work(&workTimeOut);

    return HRTIMER_NORESTART;
}
static struct hrtimer g_timeOutTimer;
void timerInit(void)
{
	g_timeOutTimeMs=1000; //1s
	hrtimer_init( &g_timeOutTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	g_timeOutTimer.function=ledTimeOutCallback;

}



static int constant_flashlight_ioctl(MUINT32 cmd, MUINT32 arg)
{
	int i4RetValue = 0;
	int ior;
	int iow;
	int iowr;
	ior = _IOR(FLASHLIGHT_MAGIC,0, int);
	iow = _IOW(FLASHLIGHT_MAGIC,0, int);
	iowr = _IOWR(FLASHLIGHT_MAGIC,0, int);
	PK_DBG("constant_flashlight_ioctl() line=%d cmd=%d, ior=%d, iow=%d iowr=%d arg=%d\n",__LINE__, cmd, ior, iow, iowr, arg);
	PK_DBG("constant_flashlight_ioctl() line=%d cmd-ior=%d, cmd-iow=%d cmd-iowr=%d arg=%d\n",__LINE__, cmd-ior, cmd-iow, cmd-iowr, arg);
    switch(cmd)
    {

		case FLASH_IOC_SET_TIME_OUT_TIME_MS:
			PK_DBG("FLASH_IOC_SET_TIME_OUT_TIME_MS: %d\n",arg);
			g_timeOutTimeMs=arg;
		break;


    	case FLASH_IOC_SET_DUTY :
    		PK_DBG("FLASHLIGHT_DUTY: %d\n",arg);
    		g_duty=arg;
    		FL_dim_duty(arg);
    		break;


    	case FLASH_IOC_SET_STEP:
    		PK_DBG("FLASH_IOC_SET_STEP: %d\n",arg);
    		g_step=arg;
    		FL_step(arg);
    		break;

    	case FLASH_IOC_SET_ONOFF :
    		PK_DBG("FLASHLIGHT_ONOFF: %d\n",arg);
    		if(arg==1)
    		{
				if(g_timeOutTimeMs!=0)
	            {
	            	ktime_t ktime;
					ktime = ktime_set( 0, g_timeOutTimeMs*1000000 );
					hrtimer_start( &g_timeOutTimer, ktime, HRTIMER_MODE_REL );
	            }
    			FL_enable();
    			g_strobe_On=1;
    		}
    		else
    		{
    			FL_disable();
				hrtimer_cancel( &g_timeOutTimer );
				g_strobe_On=0;
    		}
    		break;
		default :
    		PK_DBG(" No such command \n");
    		i4RetValue = -EPERM;
    		break;
    }
    return i4RetValue;
}




static int constant_flashlight_open(void *pArg)
{
    int i4RetValue = 0;
    PK_DBG("constant_flashlight_open line=%d\n", __LINE__);

	if (0 == strobe_Res)
	{
	    FL_init();
		timerInit();
		timerInit_600ms();
	}
	spin_lock_irq(&g_strobeSMPLock);

    if(strobe_Res)
    {
        PK_ERR(" busy!\n");
        i4RetValue = -EBUSY;
    }
    else
    {
        strobe_Res += 1;
    }

    spin_unlock_irq(&g_strobeSMPLock);

    return i4RetValue;

}


static int constant_flashlight_release(void *pArg)
{
    PK_DBG(" constant_flashlight_release\n");

    if (strobe_Res)
    {
        spin_lock_irq(&g_strobeSMPLock);

        strobe_Res = 0;

        /* LED On Status */
        g_strobe_On = FALSE;

        spin_unlock_irq(&g_strobeSMPLock);

    	FL_uninit();
    }

    PK_DBG(" Done\n");

    return 0;

}


FLASHLIGHT_FUNCTION_STRUCT	constantFlashlightFunc=
{
	constant_flashlight_open,
	constant_flashlight_release,
	constant_flashlight_ioctl
};


MUINT32 constantFlashlightInit(PFLASHLIGHT_FUNCTION_STRUCT *pfFunc)
{
	PK_DBG(" constantFlashlightInit\n");
    if (pfFunc != NULL)
    {
        *pfFunc = &constantFlashlightFunc;
    }
    return 0;
}



/* LED flash control for high current capture mode*/
ssize_t strobe_VDIrq(void)
{
	PK_DBG(" strobe_VDIrq\n");
    return 0;
}

EXPORT_SYMBOL(strobe_VDIrq);


