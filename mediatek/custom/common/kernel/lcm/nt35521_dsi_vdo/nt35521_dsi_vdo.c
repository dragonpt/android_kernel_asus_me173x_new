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

#define LCM_ID_NT35521 (0x80)

#define LCM_DSI_CMD_MODE									0

#define GPIO_LCD_PANEL_RESETINNO    GPIO142 //reset
#define GPIO_LCD_PANEL_BKLTEN       GPIO177 //Backlight Enable
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

static void lcd_backlight_en(unsigned char enabled)
{
	if (enabled) {
		mt_set_gpio_mode(GPIO_LCD_PANEL_BKLTEN, GPIO_MODE_00);
		mt_set_gpio_dir(GPIO_LCD_PANEL_BKLTEN, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_LCD_PANEL_BKLTEN, GPIO_OUT_ONE);
	} else {
		mt_set_gpio_mode(GPIO_LCD_PANEL_BKLTEN, GPIO_MODE_00);
		mt_set_gpio_dir(GPIO_LCD_PANEL_BKLTEN, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_LCD_PANEL_BKLTEN, GPIO_OUT_ZERO);
	}
}

static void init_lcm_registers(void)
{
	unsigned int data_array[16];
#ifndef BUILD_LK
	printk("[DDP] %s\n", __func__);
#endif

	data_array[0] =0x00010500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);

	data_array[0] = 0x00053902;
	data_array[1] = 0xA555AAFF;
	data_array[2] = 0x00000080;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00033902;
	data_array[1] = (((FRAME_HEIGHT/2)&0xFF) << 16) | (((FRAME_HEIGHT/2)>>8) << 8) | 0x44;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(20);

	data_array[0] = 0x00351500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);

	data_array[0]=0x703A1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x0000116F;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000020F7;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x066F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0xA0F71500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x196F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x12F71500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x086F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x40FA1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x116F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x01F31500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x52AA55F0;
	data_array[2] = 0x00000008;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000168B1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x08B61500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x026F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x08B81500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x005454BB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000505BC;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x01C71500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x0CB002BD;
	data_array[2] = 0x0000000A;
	dsi_set_cmdq(data_array, 3, 1);

	//Page 1 relative
	data_array[0] = 0x00063902;
	data_array[1] = 0x52AA55F0;
	data_array[2] = 0x00000108;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000505B0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000505B1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x00013ABC;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x00013EBD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00CA1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x04C01500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x80BE1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x002828B3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001212B4;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003434B9;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001414BA;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x52AA55F0;
	data_array[2] = 0x00000208;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x02EE1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x150609EF;
	data_array[2] = 0x00000018;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x000000B0;
	data_array[2] = 0x00170008;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x066F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x002500B0;
	data_array[2] = 0x00450030;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x0C6F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x005600B0;
	data_array[2] = 0x0000007A;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x00A300B1;
	data_array[2] = 0x002001E7;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x066F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x017A01B1;
	data_array[2] = 0x00C501C2;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x0C6F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x020602B1;
	data_array[2] = 0x0000005F;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x029202B2;
	data_array[2] = 0x00FC02D0;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x066F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x033503B2;
	data_array[2] = 0x008B035D;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x0C6F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x03A203B2;
	data_array[2] = 0x000000BF;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x03E803B3;
	data_array[2] = 0x000000FF;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x000000BC;
	data_array[2] = 0x00180008;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x066F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x002700BC;
	data_array[2] = 0x00490032;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x0C6F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x005C00BC;
	data_array[2] = 0x00000083;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x00AF00BD;
	data_array[2] = 0x002A01F3;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x066F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x018401BD;
	data_array[2] = 0x00CD01CA;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x0C6F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x020E02BD;
	data_array[2] = 0x00000065;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x029802BE;
	data_array[2] = 0x000003D4;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x066F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x033703BE;
	data_array[2] = 0x008D035F;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x0C6F1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x03A403BE;
	data_array[2] = 0x000000BF;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x03E803BF;
	data_array[2] = 0x000000FF;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x52AA55F0;
	data_array[2] = 0x00000608;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001700B0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001516B1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001314B2;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001112B3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x002D10B4;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000801B5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003109B6;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131B7;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131B8;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131B9;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131BA;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131BB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131BC;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000931BD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000108BE;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x00102DBF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001211C0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001413C1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001615C2;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000017C3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131E5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001700C4;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001516C5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001314C6;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001112C7;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x002D10C8;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000801C9;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003109CA;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131CB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131CC;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131CD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131CE;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131CF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131D0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000931D1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000108D2;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x00102DD3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001211D4;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001413D5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001615D6;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000017D7;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131E6;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x000000D8;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x000000D9;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00E71500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x52AA55F0;
	data_array[2] = 0x00000308;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000020B0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000020B1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x420005B2;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x420005B6;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x420053BA;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x420053BB;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x40C41500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x52AA55F0;
	data_array[2] = 0x00000508;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000617B0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00B81500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x010103BD;
	data_array[2] = 0x00000100;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000617B1;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000100B9;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000617B2;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000100BA;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000617B3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x00000ABB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000617B4;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000617B5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000314B6;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000000B7;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000102BC;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x05C01500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0xA5C41500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003003C8;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x005103C9;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x030500D1;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x090500D2;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x02E51500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x02E61500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x02E71500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x02E91500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x33ED1500;
	dsi_set_cmdq(&data_array,1,1);

	data_array[0] = 0x00110500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);
	data_array[0] =0x00290500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00130500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000051;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00002453;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000FF51;
	dsi_set_cmdq(data_array, 2, 1);
	}
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
		params->dsi.PLL_CLOCK = LCM_DSI_6589_PLL_CLOCK_208;
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
		mt_set_gpio_mode(GPIO_LCD_PANEL_RESETINNO, GPIO_MODE_00);
		mt_set_gpio_dir(GPIO_LCD_PANEL_RESETINNO, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_LCD_PANEL_RESETINNO, GPIO_OUT_ZERO);
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

		lcd_backlight_en(0);
		MDELAY(1);

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

		mt_set_gpio_mode(GPIO_LCD_PANEL_RESETINNO, GPIO_MODE_00);
		mt_set_gpio_dir(GPIO_LCD_PANEL_RESETINNO, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_LCD_PANEL_RESETINNO, GPIO_OUT_ONE);

		mt_set_gpio_mode(GPIO_LCD_PANEL_RESETINNO, GPIO_MODE_00);
		mt_set_gpio_dir(GPIO_LCD_PANEL_RESETINNO, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_LCD_PANEL_RESETINNO, GPIO_OUT_ZERO);
		MDELAY(10);

		mt_set_gpio_mode(GPIO_LCD_PANEL_RESETINNO, GPIO_MODE_00);
		mt_set_gpio_dir(GPIO_LCD_PANEL_RESETINNO, GPIO_DIR_OUT);
		mt_set_gpio_out(GPIO_LCD_PANEL_RESETINNO, GPIO_OUT_ONE);
		MDELAY(120);

		init_lcm_registers();
		MDELAY(8);

		lcd_backlight_en(1);

}


LCM_DRIVER nt35521_dsi_vdo_lcm_drv=
{
    .name           = "nt35521_dsi_vdo_lcm_drv",
	.set_util_funcs		= lcm_set_util_funcs,
	.get_params		= lcm_get_params,
	.init				= lcm_init,
	.suspend			= lcm_suspend,
	.resume			= lcm_resume,
	.suspend_power	= lcm_suspend_power,
	.resume_power	= lcm_resume_power,
	.init_power		= lcm_init_power,
	//.compare_id		= lcm_compare_id,
};
