#ifdef BUILD_LK
#else
#include <linux/string.h>
#endif

#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(800) 
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0XFFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_ID_NT35521										(0x5521)

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

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void init_lcm_registers(void)
{
	unsigned int data_array[16];

	data_array[0] = 0x00011500;  //software reset					 
	dsi_set_cmdq(data_array, 1, 1);
	
	MDELAY(20);
};

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

    params->dsi.mode   = BURST_VDO_MODE; //cesar ok

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_FOUR_LANE; //cesar ok
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order 	= LCM_COLOR_ORDER_RGB;  //cesar ok
    params->dsi.data_format.trans_seq   	= LCM_DSI_TRANS_SEQ_MSB_FIRST; //cesar ok
    params->dsi.data_format.padding     	= LCM_DSI_PADDING_ON_LSB;//cesar ok
    params->dsi.data_format.format      	= LCM_DSI_FORMAT_RGB888;//cesar ok

    // Video mode settings
    params->dsi.intermediat_buffer_num = 0;//cesar ok

   // Highly depends on LCD driver capability.
   //video mode timing

    params->dsi.PS						= LCM_PACKED_PS_24BIT_RGB888;//cesar ok
	params->dsi.packet_size=512;                                     //cesar ok

    params->dsi.vertical_sync_active	= 2;//cesar ok
    params->dsi.vertical_backporch		= 12;//cesar ok
    params->dsi.vertical_frontporch		= 10;//cesar ok
    params->dsi.vertical_active_line		= FRAME_HEIGHT;

    params->dsi.horizontal_sync_active	= 2;//cesar ok
    params->dsi.horizontal_backporch	= 40;//cesar ok
    params->dsi.horizontal_frontporch	= 40;//cesar ok
    params->dsi.horizontal_active_pixel	= FRAME_WIDTH;

    //improve clk quality
    params->dsi.PLL_CLOCK 				= LCM_DSI_6589_PLL_CLOCK_208;//cesar k
    params->dsi.compatibility_for_nvk 	= 1; //cesar

//cesar: for TEST
		params->dsi.CLK_ZERO = 47;
		params->dsi.HS_ZERO = 36;

}

//cesar
//lcm init, suspend & resume are still not working alright, so its disabled for now

#if 0
static void lcm_init(void)
{
	MDELAY(50); 
	
	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(1);
	SET_RESET_PIN(1);


	init_lcm_registers();
}

static void lcm_suspend(void)
{
	unsigned int data_array[16];

	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);
	
	data_array[0] = 0x00111500; // Sleep In
	dsi_set_cmdq(data_array, 1, 1);

	MDELAY(160);
	//SET_RESET_PIN(0);
}


static void lcm_resume(void)
{
	unsigned int data_array[16];
	lcm_init();

	data_array[0] = 0x00101500; // Sleep Out
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);

	data_array[0] = 0x00290500; // Display On
	dsi_set_cmdq(data_array, 1, 1);

}
         
#endif
static unsigned int lcm_compare_id(void)
{
	return 1;
}

LCM_DRIVER nt35521_dsi_vdo_lcm_drv=
{
    .name           = "nt35521_dsi_vdo_lcm_drv",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	//.init           = lcm_init,
	//.suspend        = lcm_suspend,
	//.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
};
