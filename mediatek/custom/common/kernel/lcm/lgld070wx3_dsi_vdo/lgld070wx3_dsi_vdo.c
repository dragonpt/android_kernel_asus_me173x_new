#ifndef BUILD_LK
#include <linux/string.h>

#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pm_ldo.h>
#include <platform/mt_pmic.h>
#include <debug.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#include <asm/arch/mt_pm_ldo.h>
#include <asm/arch/mt_pmic.h>
#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#else
#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <mach/mt_gpio.h>
#include <linux/xlog.h>
#include <mach/mt_pm_ldo.h>
//#include <mach/mt_pmic.h>
#include <mach/upmu_common.h>
#endif

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (800)
#define FRAME_HEIGHT (1280)

#define LCM_ID_NT35590 (0x90)

#define LCM_DSI_CMD_MODE									0

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


static void lcd_power_en(unsigned char enabled)
{
#ifndef BUILD_LK
	printk("[DDP] %s : %s\n", __func__, enabled ? "on" : "off");
#endif

	if (enabled) {
		//VGP4 1.8V
		upmu_set_rg_vgp4_en(0x1);
		upmu_set_rg_vgp4_vosel(0x3);
		MDELAY(1);

		 //VGP6 3.3V
		upmu_set_rg_vgp6_en(0x1);
		upmu_set_rg_vgp6_vosel(0x7);
		MDELAY(1);
	} else {
		MDELAY(60);
		//VGP4 1.8V
		upmu_set_rg_vgp4_en(0x0);
		upmu_set_rg_vgp4_vosel(0x0);
		MDELAY(0);

		 //VGP6 3.3V
		upmu_set_rg_vgp6_en(0x0);
		upmu_set_rg_vgp6_vosel(0x0);

		MDELAY(1);
	}
}

static void init_lcm_registers(void)
{
	unsigned int data_array[16];
#ifndef BUILD_LK
	printk("[DDP] %s\n", __func__);
#endif

		data_array[0] = 0x0BAE1500;
		dsi_set_cmdq(data_array, 1, 1);
		data_array[0] = 0xEAEE1500;
		dsi_set_cmdq(data_array, 1, 1);
		data_array[0] = 0x5FEF1500;
		dsi_set_cmdq(data_array, 1, 1);
		data_array[0] = 0x68F21500;
		dsi_set_cmdq(data_array, 1, 1);
		data_array[0] = 0x03A71500;
		dsi_set_cmdq(data_array, 1, 1);

		/*data_array[0] = 0x10CC1500;
		dsi_set_cmdq(data_array, 1, 1);*/ // superdragonpt: TODO Test user feedback: TOP right corner ESD lines.



// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));
    params->type   					= LCM_TYPE_DSI;
    params->width  					= FRAME_WIDTH;
    params->height 					= FRAME_HEIGHT;

    #if (LCM_DSI_CMD_MODE)
	params->dsi.mode   = CMD_MODE;
    #else
    params->dsi.mode   = BURST_VDO_MODE;
    #endif

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_FOUR_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order 	= LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq   	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding     	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format      	= LCM_DSI_FORMAT_RGB888;

    // Video mode settings
    params->dsi.intermediat_buffer_num = 0;

   // Highly depends on LCD driver capability.
   //video mode timing

    params->dsi.PS						= LCM_PACKED_PS_24BIT_RGB888;
    //params->dsi.word_count = 720*3;	
	params->dsi.packet_size=512;
    params->dsi.vertical_sync_active	= 2;
    params->dsi.vertical_backporch		= 12;
    params->dsi.vertical_frontporch		= 10;
    params->dsi.vertical_active_line		= FRAME_HEIGHT;

    params->dsi.horizontal_sync_active	= 2;
    params->dsi.horizontal_backporch	= 40;
    params->dsi.horizontal_frontporch	= 40;
    params->dsi.horizontal_active_pixel	= FRAME_WIDTH;

		// Bit rate calculation
		params->dsi.pll_select=1;
		//1 Every lane speed
		params->dsi.PLL_CLOCK = LCM_DSI_6589_PLL_CLOCK_240_5; //LCM_DSI_6589_PLL_CLOCK_208;
    params->dsi.compatibility_for_nvk 	= 1;

}

static void lcm_init_power(void)
{
	lcd_power_en(1);
	MDELAY(1);
	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(1);
	SET_RESET_PIN(1);
	MDELAY(115);
}

static void lcm_init(void)
{

#ifndef BUILD_LK
    printk("[LCM] superdragonpt NT35521 lcm init() enter\n");
#else
	init_lcm_registers();
#endif

}

static void lcm_suspend_power(void)
{
#ifndef BUILD_LK
	printk("[DDP] %s\n", __func__);
#endif

		MDELAY(105);
		MDELAY(1);

		lcd_power_en(0);
		MDELAY(1);
}


static void lcm_suspend(void)
{
	unsigned int data_array[16];
#ifndef BUILD_LK
	printk("[DDP] %s\n", __func__);
#endif
		/* set display off */
		data_array[0] = 0x00280500;
		dsi_set_cmdq(data_array, 1, 1);
		MDELAY(10);

		/* enter sleep mode */
		data_array[0] = 0x00100500;
		dsi_set_cmdq(data_array, 1, 1);
	}


static void lcm_resume_power(void)
{
#ifndef BUILD_LK
	printk("[DDP] %s\n", __func__);
#endif
		MDELAY(8);
		lcd_power_en(1);
		MDELAY(1);
	}

static void lcm_resume(void)
{
	unsigned int data_array[16];

#ifndef BUILD_LK
	printk("[DDP] %s\n", __func__);
#endif

		/* exit sleep mode */
		data_array[0] = 0x00110500;
		dsi_set_cmdq(data_array, 1, 1);
		MDELAY(10);

		/* set display on */
		data_array[0] = 0x00290500;
		dsi_set_cmdq(data_array, 1, 1);
		MDELAY(50);

		init_lcm_registers();
		MDELAY(8);

}




LCM_DRIVER lgld070wx3_dsi_vdo_lcm_drv=
{
    .name           = "lgld070wx3_dsi_vdo_lcm_drv",
	.set_util_funcs		= lcm_set_util_funcs,
	.get_params		= lcm_get_params,
	.init				= lcm_init,
	.suspend			= lcm_suspend,
	.resume			= lcm_resume,
	.suspend_power	= lcm_suspend_power,
	.resume_power	= lcm_resume_power,
	.init_power		= lcm_init_power,
};
