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

#define FRAME_WIDTH  										(540)
#define FRAME_HEIGHT 										(960)

#define REGFLAG_DELAY                                       0XFFE
#define REGFLAG_END_OF_TABLE                                0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0

#define LCM_ID_HX8389B 0x89

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)            lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)                   lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
       

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
	
#if 1
	{0xB9,3,{0xFF,0x83,0x89}},
			
	 {REGFLAG_DELAY, 5, {}}, 

	{0XBA, 7, {0x41,0x93,0X00,0X16, 0Xa4,0X10,0X18}}, 			
	 {REGFLAG_DELAY, 5, {}}, 
	{0xc6,1,{0x08}},
			
	 {REGFLAG_DELAY, 5, {}}, 

	{0xCC,1,{0x02}},
			
	 {REGFLAG_DELAY, 5, {}}, 

	{0xB1,19,{0x00,0x00,0x07,0xE8,0x50,0x10,0x11,0xd0,0xf0,0x2f,0x37,0x27,0x27,0x43,0x01,0x58,0xF0,0x00,0xE6}},       
	 {REGFLAG_DELAY, 5, {}}, 

	{0xB2,7,{0x00,0x00,0x78,0x0C,0x07,0x3F,0x80}},	
			
	 {REGFLAG_DELAY, 5, {}}, 

	{0xb4,23,{0x80,0x08,0x00,0x32,0x10,0x04,0x32,0x10,0x00,0x32,0x10,0x00,0x37,0x0a,0x40,0x08,0x37,0x0a,0x40,0x14,0x46,0x50,0x0a}},	
			 
	 {REGFLAG_DELAY, 5, {}}, 

	{0xd5,56,{0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x60,0x00,0x99,0x88,0xAA,0xBB,0x88,0x23,0x88,0x01,0x88,0x67,0x88,0x45,0x01,0x23,0x88,0x88,0x88,0x88,0x88,0x88,0x99,0xBB,0xAA,0x88,0x54,0x88,0x76,0x88,0x10,0x88,0x32,0x32,0x10,0x88,0x88,0x88,0x88,0x88,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00}},
			      
	 {REGFLAG_DELAY, 5, {}}, 


	{0xe0,34,{0x05,0x0D,0x12,0x2E,0x36,0x3F,0x19,0x34,0x06,0x0B,0x10,0x13,0x16,0x14,0x17,0x17,0x16,0x05,0x0D,0x12,0x2E,0x36,0x3F,0x19,0x34,0x06,0x0B,0x10,0x13,0x16,0x14,0x17,0x17,0x16}},
	 {REGFLAG_DELAY, 5, {}},  

	{0xCB,2,{0x07,0x07}},
			   
	 {REGFLAG_DELAY, 5, {}}, 

	{0xBB,4,{0x00,0x00,0xFF,0x80}},	  
			   
	 {REGFLAG_DELAY, 5, {}}, 



	{0xDE,3,{0x05,0x58,0x10}},
			
	 {REGFLAG_DELAY, 5, {}}, 
		

	{0xB6,4,{0x00,0x9a,0x00,0x9a}},		  	  
			   
	 {REGFLAG_DELAY, 5, {}}, 
	                     



	{0x11,0,{ }},
	{REGFLAG_DELAY, 120, {}},
	
	{0x29,0,{ }},
	{REGFLAG_DELAY, 120, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
#else
	{0XB9, 3, {0XFF,0X83,0X89}}, 
	{REGFLAG_DELAY, 10, {}}, 

 
	{0XBA, 7, {0x01,0x92,0X00,0X16, 0XC4,0X00,0X18}}, 
	{REGFLAG_DELAY, 10, {}}, 
 

	{0XDE, 2, {0X05,0X58, 0X10}}, 
	{REGFLAG_DELAY, 10, {}}, 

	{0xB1,19,{0x00,0x00,0x07,0xE8,0x50,0x10,0x11,0xd0,0xf0,0x2f,0x37,0x27,0x27,0x43,0x01,0x58,0xF0,0x00,0xE6}},       

	{REGFLAG_DELAY, 10, {}}, 


	{0xB2,7,{0x00,0x00,0x78,0x0C,0x07,0x3F,0x80}},	

	{REGFLAG_DELAY, 10, {}}, 

	{0xb4,23,{0x80,0x08,0x00,0x32,0x10,0x04,0x32,0x10,0x00,0x32,0x10,0x00,0x37,0x0a,0x40,0x08,0x37,0x0a,0x40,0x14,0x46,0x50,0x0a}},	

	{REGFLAG_DELAY, 10, {}}, 

	{0xd5,56,{0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x60,0x00,0x99,0x88,0xAA,0xBB,0x88,0x23,0x88,0x01,0x88,0x67,0x88,0x45,0x01,0x23,0x88,0x88,0x88,0x88,0x88,0x88,0x99,0xBB,0xAA,0x88,0x54,0x88,0x76,0x88,0x10,0x88,0x32,0x32,0x10,0x88,0x88,0x88,0x88,0x88,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00}},
 
	{REGFLAG_DELAY, 10, {}}, 

	{0xe0,34,{0x01,0x02,0x00,0x15,0x15,0x38,0x22,0x39,0x07,0x0A,0x18,0x1A,0x1C,0x18,0x17,0x12,0x17,0x01,0x02,0x00,0x15,0x15,0x38,0x22,0x39,0x07,0x0A,0x18,0x1A,0x1C,0x18,0x17,0x12,0x17}},


	{0XCC, 1, {0X02}}, 
	{REGFLAG_DELAY, 10, {}}, 

	{0XCB, 2, {0X07, 0X07}}, 
	{REGFLAG_DELAY, 10, {}}, 

	{0XBB, 4, {0X00,0X00,0XFF,0X80}}, 
	{REGFLAG_DELAY, 10, {}}, 

	{0XB6, 4, {0X00,0X9A,0X00,0X9A}}, 
	{REGFLAG_DELAY, 10, {}}, 

	{0xDE,3,{0x05,0x58,0x10}},
			
	 {REGFLAG_DELAY, 5, {}}, 

	// Sleep Out 
	{0x11, 0, {0x00}}, 
	{REGFLAG_DELAY, 150, {}}, 

	// Display ON 
	{0x29, 0, {0x00}}, 
	{REGFLAG_DELAY, 20, {}}, 

 
	{REGFLAG_END_OF_TABLE, 0x00, {}} 

#endif
};



static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 10, {}},
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] = {
	// Display off sequence
	{0x28, 0, {0x00}},

    // Sleep Mode On
	{0x10, 0, {0x00}},

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
    params->dsi.vertical_backporch   = 5;
    params->dsi.vertical_frontporch  = 5;
    params->dsi.vertical_active_line     = FRAME_HEIGHT;

    params->dsi.horizontal_sync_active   = 10;
    params->dsi.horizontal_backporch     = 46;
    params->dsi.horizontal_frontporch    = 21;
    params->dsi.horizontal_active_pixel  = FRAME_WIDTH;

	params->dsi.compatibility_for_nvk = 0;		// this parameter would be set to 1 if DriverIC is NTK's and when force match DSI clock for NTK's

        //	params->dsi.HS_PRPR=6;
	   //params->dsi.LPX=4; 
		//params->dsi.HS_PRPR=5;
		//params->dsi.HS_TRAIL=13;
	//	params->dsi.CLK_TRAIL = 10;
    // Bit rate calculation
		//1 Every lane speed
		params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		params->dsi.fbk_div = 16;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	

}

static void lcm_init_register(void)
{
        unsigned int data_array[16];	

		
	 data_array[0] = 0x00043902;                          
        data_array[1] = 0x8983FFB9;
        dsi_set_cmdq(&data_array, 2, 1);

        MDELAY(5);  
		
	data_array[0] = 0x00083902;                          
       data_array[1] = 0x009341BA;
	data_array[2] = 0x1810A416;
        dsi_set_cmdq(&data_array, 3, 1);
        MDELAY(5);  
		
	
	 data_array[0] = 0x00023902;                          
        data_array[1] = 0x000008C6;
        dsi_set_cmdq(&data_array, 2, 1);
        MDELAY(5);  

	 data_array[0] = 0x00023902;                          
        data_array[1] = 0x00000ACC;
        dsi_set_cmdq(&data_array, 2, 1);
        MDELAY(5);  

	data_array[0] = 0x00143902;                          
       data_array[1] = 0x070000B1;
	data_array[2] = 0x111050E8; 
	data_array[3] = 0x2921F898; 
	data_array[4] = 0x01432727; 
	data_array[5] = 0xE600F058;   			       
        dsi_set_cmdq(&data_array, 6, 1);	
        MDELAY(5);  
		
	data_array[0] = 0x00083902;                          
       data_array[1] = 0x780000B2;
	data_array[2] = 0x803F070C;
        dsi_set_cmdq(&data_array, 3, 1);
        MDELAY(5);  


	data_array[0] = 0x00183902;                          
       data_array[1] = 0x000880B4;
	data_array[2] = 0x32041032; 
	data_array[3] = 0x10320010; 
	data_array[4] = 0x400A3700; 
	data_array[5] = 0x400A3708;
	data_array[6] = 0x0a504614;   			
        dsi_set_cmdq(&data_array, 7, 1);	
        MDELAY(5);  



	data_array[0] = 0x00393902;                          
       data_array[1] = 0x000000D5;
	data_array[2] = 0x00000100; 
	data_array[3] = 0x99006000; 
	data_array[4] = 0x88BBAA88; 
	data_array[5] = 0x88018823;
	data_array[6] = 0x01458867; 
       data_array[7] = 0x88888823;
	data_array[8] = 0x99888888; 
	data_array[9] = 0x5488AABB; 
	data_array[10] = 0x10887688; 
	data_array[11] = 0x10323288;
	data_array[12] = 0x88888888; 
	data_array[13] = 0x00040088; 
	data_array[14] = 0x00000000;
	data_array[15] = 0x00000000; 	
        dsi_set_cmdq(&data_array, 16, 1);	
        MDELAY(5);  


	data_array[0] = 0x00233902;                          
       data_array[1] = 0x0D0801E0;
	data_array[2] = 0x1436251F; 
	data_array[3] = 0x0C0A0639; 
	data_array[4] = 0x0F0E100F; 
	data_array[5] = 0x08011D1D;
	data_array[6] = 0x36251F0D; 
       data_array[7] = 0x0A063914;
	data_array[8] = 0x0E100F0C; 
	data_array[9] = 0x001D1D0F; 
        dsi_set_cmdq(&data_array, 10, 1);	
        MDELAY(5);  

		   
	 data_array[0] = 0x00033902;                          
        data_array[1] = 0x000707CB;
        dsi_set_cmdq(&data_array, 2, 1);
        MDELAY(5);  

	data_array[0] = 0x00053902;                          
       data_array[1] = 0xFF0000BB;
	data_array[2] = 0x00000080;
        dsi_set_cmdq(&data_array, 3, 1);
        MDELAY(5);  


	data_array[0] = 0x00033902;                          
       data_array[1] = 0x105805DE;
        dsi_set_cmdq(&data_array, 2, 1);
        MDELAY(5);  
		
	  	  
	data_array[0] = 0x00053902;                          
       data_array[1] = 0x008E00B6;
	data_array[2] = 0x0000008E;
        dsi_set_cmdq(&data_array, 3, 1);
        MDELAY(5);  

	data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000236;
        dsi_set_cmdq(&data_array, 2, 1);
        MDELAY(5);  
	                     
	data_array[0] = 0x00110500; // Sleep Out
	dsi_set_cmdq(&data_array, 1, 1);

	MDELAY(150);

	data_array[0] = 0x00290500; // Display On
	dsi_set_cmdq(&data_array, 1, 1); 
        MDELAY(120);  		

}




static void lcm_init(void)
{
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(1);
    SET_RESET_PIN(1);
    MDELAY(10);//Must over 6 ms,SPEC request
	//push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
	lcm_init_register();
}


static void lcm_suspend(void)
{


	push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(1);	
	MDELAY(10);	
	SET_RESET_PIN(0);
}


static void lcm_resume(void)
{

   	SET_RESET_PIN(1);
    	MDELAY(10);//Must over 6 ms,SPEC request
	lcm_init_register();
	//push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
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
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(&data_array, 7, 0);

}
#endif



static unsigned int lcm_compare_id(void)
{
	unsigned int id=0;
	unsigned int id1=0;	
	unsigned char buffer[2];
	unsigned int array[16];  
	unsigned char lcd_id = 0;
	
    	SET_RESET_PIN(1);
	MDELAY(10);   	
    	SET_RESET_PIN(0);
	MDELAY(10);
	
    	SET_RESET_PIN(1);
	MDELAY(20);  
	
	array[0]=0x00043902;
	array[1]=0x8983FFB9;// page enable
	dsi_set_cmdq(&array, 2, 1);
	MDELAY(10);

	array[0] = 0x00023700;// return byte number
	dsi_set_cmdq(&array, 1, 1);
	MDELAY(10);

	read_reg_v2(0xF4, buffer, 2);
	id = buffer[0]; 

	lcd_id =  mt_get_gpio_in(GPIO_LCM_ID_PIN);	
	
#ifdef BUILD_LK
	printf("zhuoshineng LK HX8389 debug: HX8389 lcd_id = %d\n",lcd_id);
	printf("zhuoshineng LK HX8389 debug: HX8389 id = %x\n",id);

#else
	printk("zhuoshineng kernel HX8389  debug: HX8389 lcd_id= %d\n", lcd_id);
	printk("zhuoshineng kernel HX8389  debug: HX8389 id = %x\n", id);
#endif


	if(LCM_ID_HX8389B == id)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

LCM_DRIVER hx8389b_qhd_dsi_vdo_drv = 
{
       .name			= "hx8389b_qhd_dsi_vdo",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
        .compare_id     = lcm_compare_id,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
};

