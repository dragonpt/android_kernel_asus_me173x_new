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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH                                         (540)
#define FRAME_HEIGHT                                        (960)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#define REGFLAG_DELAY                                       0XFFE
#define REGFLAG_END_OF_TABLE                                0xFFF   // END OF REGISTERS MARKER




#define LCM_ID_OTM9605A 0x9605

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))


#define UDELAY(n)                                           (lcm_util.udelay(n))
#define MDELAY(n)                                           (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)            lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)                   lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


#define   LCM_DSI_CMD_MODE							0

static struct LCM_setting_table
{
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] =
{

	{0x00,1,{0x00}},
	{0xFF,3,{0x96,0x05,0x01}},	

	
	{0x00,1,{0x80}},
	{0xFF,2,{0x96,0x05}},	

	
	{0x00,1,{0x92}}, // mipi 2 lane
	{0xFF,2,{0x10,0x02}},	

	{0x00,1,{0xB4}},	
	{0xC0,1,{0x50}},//inversion	

	
	{0x00,1,{0x80}},
	{0xC1,2,{0x36,0x66}}, //70Hz

	
	{0x00,1,{0x89}},
	{0xC0,1,{0x01}},// TCON OSC turbo mode	

	
	{0x00,1,{0xA0}},
	{0xC1,1,{0x02}},	


	{0x00,1,{0x80}},
	{0xC5,4,{0x08,0x00,0xA0,0x11}},	


	{0x00,1,{0x90}},
	{0xC5,3,{0xD6,0x57,0x01}},	//VGH=14V, VGL=-11V	

	{0x00,1,{0xB0}},
	{0xC5,2,{0x05,0x28}},
		
	{0x00,1,{0x00}},	//GVDD/NGVDD
	{0xD8,2,{0x77,0x77}},

	{0x00,1,{0x00}},	//Vcom setting
	{0xD9,1,{0x57}},

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	//Inrush Current Test
	{0x00,1,{0xA6}},
	{0xC1,1,{0x01}},	

	{0x00,1,{0xA2}}, // pl_width, pch_dri_pch_nop
	{0xC0,3,{0x0C,0x05,0x02}},	


	//GOA mapping
	{0x00,1,{0x80}},	//GOA mapping
	{0xCB,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},																
	
	{0x00,1,{0x90}},	//GOA mapping
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},    	
	
	{0x00,1,{0xA0}},																																						
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},    
	
	{0x00,1,{0xB0}},	
	{0xCB,10,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},															

	{0x00,1,{0xC0}},	
	{0xCB,15,{0x04,0x04,0x04,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x04,0x04,0x08}},    	

	{0x00,1,{0xD0}},	
	{0xCB,15,{0x08,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x08,0x04,0x08,0x04,0x08,0x04}},    
	
	{0x00,1,{0xE0}}, 
	{0xCB,10,{0x08,0x04,0x04,0x04,0x08,0x08,0x00,0x00,0x00,0x00}},


	{0x00,1,{0xF0}}, 
	{0xCB,10,{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},

	{0x00,1,{0x80}},
	{0xCC,10,{0x26,0x25,0x21,0x22,0x00,0x0F,0x00,0x0D,0x00,0x0B}},															

	{0x00,1,{0x90}},
	{0xCC,15,{0x00,0x09,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x25,0x21,0x22,0x00}},    

	{0x00,1,{0xA0}},
	{0xCC,15,{0x10,0x00,0x0E,0x00,0x0C,0x00,0x0A,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00}},	


	{0x00,1,{0x80}},	//GOA VST Setting																																				
	{0xCE,12,{0x8B,0x03,0x06,0x8A,0x03,0x06,0x89,0x03,0x06,0x88,0x03,0x06}},    								
	
	{0x00,1,{0x90}}, //GOA VEND and Group Setting
	{0xCE,14,{0xF0,0x00,0x00,0xF0,0x00,0x00,0xF0,0x00,0x00,0xF0,0x00,0x00,0x00,0x00}},    				
	
	{0x00,1,{0xA0}},	//GOA CLK1 and GOA CLK2 Setting
	{0xCE,14,{0x38,0x03,0x03,0xC0,0x00,0x06,0x00,0x38,0x02,0x03,0xC1,0x00,0x06,0x00}},    			
	
	{0x00,1,{0xB0}},	//GOA CLK3 and GOA CLK4 Setting																																					//Address Shift
	{0xCE,14,{0x38,0x01,0x03,0xC2,0x00,0x06,0x00,0x38,0x00,0x03,0xC3,0x00,0x06,0x00}},    			
	
	{0x00,1,{0xC0}},	//GOA CLKB1 and GOA CLKB2 Setting																																				
	{0xCE,14,{0x38,0x07,0x03,0xBC,0x00,0x06,0x00,0x38,0x06,0x03,0xBD,0x00,0x06,0x00}},    			
	
	{0x00,1,{0xD0}},	//GOA CLKB3 and GOA CLKB4 Setting																																				
	{0xCE,14,{0x38,0x05,0x03,0xBE,0x00,0x06,0x00,0x38,0x04,0x03,0xBF,0x00,0x06,0x00}},    			

	{0x00,1,{0x80}},	//GOA CLKC1 and GOA CLKC2 Setting																																				
	{0xCF,14,{0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},    			
	
	{0x00,1,{0x90}},	//GOA CLKC3 and GOA CLKC4 Setting																																					
	{0xCF,14,{0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},    			
	
	{0x00,1,{0xA0}},	//GOA CLKD1 and GOA CLKD2 Setting
	{0xCF,14,{0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},    			
	
	{0x00,1,{0xB0}},	//GOA CLKD3 and GOA CLKD4 Setting
	{0xCF,14,{0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},    			
	
	{0x00,1,{0xC0}},	//GOA ECLK Setting and GOA Other Options1 and GOA Signal Toggle Option Setting																																				
	{0xCF,10,{0x02,0x02,0x20,0x20,0x00,0x00,0x01,0x02,0x00,0x02}},


	{0x00,1,{0x00}},																					
	{0xE1,16,{0x04,0x0B,0x10,0x0D,0x06,0x0E,0x0A,0x09,0x05,0x08,0x0E,0x08,0x0F,0x14,0x0F,0x0A}},		

	{0x00,1,{0x00}},
	{0xE2,16,{0x04,0x0B,0x10,0x0D,0x06,0x0D,0x0A,0x09,0x05,0x08,0x0E,0x08,0x0F,0x14,0x0F,0x0A}},
	


	{0x00,1,{0x00}}, 
	{0xff,3,{0xff,0xff,0xff}},
	

	
	{0x11,0,{ }},
	{REGFLAG_DELAY, 120, {}},
	{0x29,0,{ }},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] =
{
    // Display off sequence
    {0x28, 0, {0x00}},

    // Sleep Mode On
    {0x10, 0, {0x00}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_out_setting[] =
{
    // Sleep Out
    {0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
    {0x29, 0, {0x00}},
    {REGFLAG_DELAY, 10, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++)
    {

        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd)
        {

            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;

            case REGFLAG_END_OF_TABLE :
                break;

            default:
				
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
             //   dsi_set_cmdq_dcs(cmd, table[i].count, table[i].para_list, force_update);
        }
    }

}

static void init_lcm_registers(void)
{
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


// ---------------------------------------------------------------------------
// LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{

    memset(params, 0, sizeof(LCM_PARAMS));

    params->type = LCM_TYPE_DSI;

    params->width = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;

    // enable tearing-free
    params->dbi.te_mode = LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity     = LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode = CMD_MODE;
#else
    params->dsi.mode = SYNC_PULSE_VDO_MODE;
#endif

    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM     = LCM_TWO_LANE;
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

    // Highly depends on LCD driver capability.
    // Not support in MT6573
    params->dsi.packet_size=256;

    // Video mode setting
		params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

    params->dsi.vertical_sync_active     = 5;
    params->dsi.vertical_backporch   = 15;
    params->dsi.vertical_frontporch  = 15;
    params->dsi.vertical_active_line     = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active   = 8; //20
    params->dsi.horizontal_backporch     = 26; //46
    params->dsi.horizontal_frontporch    = 21;
    params->dsi.horizontal_active_pixel  = FRAME_WIDTH;

	params->dsi.compatibility_for_nvk = 0;		// this parameter would be set to 1 if DriverIC is NTK's and when force match DSI clock for NTK's

        //	params->dsi.HS_PRPR=6;
	//    params->dsi.LPX=8; 
		//params->dsi.HS_PRPR=5;
		//params->dsi.HS_TRAIL=13;
	//	params->dsi.CLK_TRAIL = 10;
    // Bit rate calculation
		//1 Every lane speed
		params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		params->dsi.fbk_div =15; // 19;  16  // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	

}
static unsigned int lcm_compare_id(void)
{
	unsigned char lcd_id = 0;
	unsigned int id=0;
	unsigned char buffer[5];
	unsigned int array[16];  

    	SET_RESET_PIN(1);
	MDELAY(10);   	
    	SET_RESET_PIN(0);
	MDELAY(10);
	
    	SET_RESET_PIN(1);
	MDELAY(20);  

	lcd_id =  mt_get_gpio_in(GPIO_LCM_ID_PIN);

#ifdef BUILD_LK
	printf("zhuoshineng LK otm9605a debug: otm9605a id = %d\n",lcd_id);
#else
	printk("zhuoshineng kernel otm9605a horse debug: otm9605a id = %d\n", lcd_id);
#endif
	array[0]=0x00043902;
	array[1]=0x010596FF;// page enable
	dsi_set_cmdq(&array, 2, 1);
	MDELAY(10);
	
	array[0] = 0x00063700;// return byte number
	dsi_set_cmdq(&array, 1, 1);
	MDELAY(10);

	read_reg_v2(0xA1, buffer, 5);
	id = buffer[2]; //we only need ID
#ifdef BUILD_LK
		printf("zhuoshineng otm9605a buffer[0] = 0x%x,buffer[1] = 0x%x,buffer[2] = 0x%x,buffer[3] = 0x%x,buffer[4] = 0x%x\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
#else

		printk("zhuoshineng otm9605a buffer[0] = 0x%x,buffer[1] = 0x%x,buffer[2] = 0x%x,buffer[3] = 0x%x,buffer[4] = 0x%x\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);

#endif


	if(id == 0x96)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
static void lcm_init(void)
{

    SET_RESET_PIN(1);
	
    SET_RESET_PIN(0);
	MDELAY(100);
	
    SET_RESET_PIN(1);
	MDELAY(100);      

	init_lcm_registers();

}


static void lcm_suspend(void)
{
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
	MDELAY(20);
    push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_resume(void)
{
	SET_RESET_PIN(1);
	MDELAY(20);
	init_lcm_registers();
}

#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);

    unsigned int data_array[16];

    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
    data_array[2]= (x1_LSB);
	dsi_set_cmdq(&data_array, 3, 1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(&data_array, 3, 1);

	data_array[0]= 0x00290508; //HW bug, so need send one HS packet
	dsi_set_cmdq(&data_array, 1, 1);
	
	data_array[0]= 0x002c3909;
	dsi_set_cmdq(&data_array, 1, 0);

}
#endif




static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
//#if 1
	char  buffer[3];
	int   array[4];

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x0a, buffer, 1);
	printk("jacob test lcm_esd_check buffer[0]=0x%x\n",buffer[0]);
	if(buffer[0]==0x9c)
	{
		return FALSE;
	}
	else
	{	
		return TRUE;
	}
#else
	return FALSE;
#endif
}



static unsigned int lcm_esd_recover(void)
{
#ifndef BUILD_LK
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);

	SET_RESET_PIN(1);
	MDELAY(10); 	
	init_lcm_registers();
#endif
	return TRUE;
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER otm9605a_dsi_vdo_lg_lcm_drv =
{
    .name           = "otm9605a_dsi_vdo_lg",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
    .esd_check   = lcm_esd_check,
    .esd_recover   = lcm_esd_recover,	    
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
};
