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

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#define REGFLAG_DELAY                                       0XFFE
#define REGFLAG_END_OF_TABLE                                0xFFF   // END OF REGISTERS MARKER


#define LCM_ID_NT35590 (0x90)

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))
//==========fenggy add=========================
//#undef SET_RESET_PIN

//#define SET_RESET_PIN(v) \
//do{ \
//	mt_set_gpio_mode(GPIO_LCM_RST_PIN, GPIO_LCM_RST_PIN_M_GPIO); \
//	mt_set_gpio_dir(GPIO_LCM_RST_PIN, GPIO_DIR_OUT); \
//	if((v) == 0) mt_set_gpio_out(GPIO_LCM_RST_PIN, GPIO_OUT_ZERO); \
//	else mt_set_gpio_out(GPIO_LCM_RST_PIN, GPIO_OUT_ONE); \
//}while(0)

//==========fenggy add end=====================



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


#define   LCM_DSI_CMD_MODE							0

static struct LCM_setting_table
{
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] =
{
	{0xFF,3,{0x12,0x83,0x01}},
	{0x00,1,{0x80}},
	{0xFF,2,{0x12,0x83}},
	{0x00,1,{0xa0}},
	{0xc1,1,{0x02}},	
	{0x00,1,{0x92}},
	{0xFF,2,{0x30,0x02}},
	{0x00,1,{0xA6}},
	{0xB3,1,{0x0B}},
	{0x00,1,{0x80}},
	{0xC0,9,{0x00,0x64,0x00,0x10,0x10,0x00,0x64,0x10,0x10}},
	{0x00,1,{0x90}},
	{0xC0,6,{0x00,0x5C,0x00,0x01,0x00,0x04}},
	{0x00,1,{0xA4}},
	{0xC0,1,{0x22}},
	{0x00,1,{0xB3}},
	{0xC0,2,{0x00,0x50}},
	{0x00,1,{0x81}},
	{0xC1,1,{0x66}},  //0x55
	{0x00,1,{0x90}},
	{0xC4,1,{0x49}},
	{0x00,1,{0xA0}},
	{0xC4,14,{0x05,0x10,0x04,0x02,0x05,0x15,0x11,0x05,0x10,0x05,0x02,0x05,0x15,0x11}},
	{0x00,1,{0xB0}},
	{0xC4,2,{0x00,0x00}},
	{0x00,1,{0xBB}},
	{0xC5,1,{0x80}},
	{0x00,1,{0x91}},
	{0xC5,2,{0x49,0x50}},
	{0x00,1,{0x00}},
	{0xD8,2,{0x9D,0x9D}},
	{0x00,1,{0x00}},
	{0xD9,1,{0x5E}},
	{0x00,1,{0x81}},
	{0xC4,1,{0x82}},
	{0x00,1,{0xB0}},
	{0xC5,2,{0x04,0x38}},  //0xB8 20130304
	{0x00,1,{0x82}},
	{0xF5,2,{0x00,0x00}},
	{0x00,1,{0x82}},
	{0xF4,1,{0x00}},
	{0x00,1,{0x00}},
	{0xD0,1,{0x40}},
	{0x00,1,{0x00}},
	{0xD1,2,{0x00,0x00}},
	{0x00,1,{0x80}},
	{0xC4,1,{0x00}},
	{0x00,1,{0x98}},
	{0xC5,1,{0x10}},
	{0x00,1,{0x81}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x83}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x85}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x87}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x89}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x8B}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x95}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x97}},
	{0xF5,1,{0x15}},
	{0x00,1,{0x99}},
	{0xF5,1,{0x15}},
	{0x00,1,{0xA1}},
	{0xF5,1,{0x15}},
	{0x00,1,{0xA3}},
	{0xF5,1,{0x15}},
	{0x00,1,{0xA5}},
	{0xF5,1,{0x15}},
	{0x00,1,{0xA7}},
	{0xF5,1,{0x15}},
	{0x00,1,{0xAB}},
	{0xF5,1,{0x18}},
	{0x00,1,{0xB1}},
	{0xF5,13,{0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x15,0x08,0x15}},
	{0x00,1,{0xB4}},
	{0xC5,1,{0xC0}},
	{0x00,1,{0x90}}, 			//Mode-3 power ic
	{0xf5,4,{0x02,0x11,0x02,0x11}}, 
	{0x00,1,{0x90}}, 			//3xVPNL 
	{0xc5,1,{0x50}}, 
	{0x00,1,{0x94}}, 			//2xVPNL 
	{0xc5,1,{0x77}}, 
//============add===================
	{0x00,1,{0xB9}},
	{0xB0,1,{0x51}},  //20130304
	{0x00,1,{0x82}},
	{0xc4,1,{0x02}},
	{0x00,1,{0x6c}},
	{0xb0,1,{0x03}},
	{0x00,1,{0x94}},
	{0xF5,1,{0x02}},
	{0x00,1,{0xba}},
	{0xF5,1,{0x03}},	
//===========add end==================		
	{0x00,1,{0x80}},
	{0xCB,11,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0x90}},
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0xFF,0x00}},
	{0x00,1,{0xA0}},
	{0xCB,15,{0xFF,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0xB0}},
	{0xCB,15,{0x00,0x00,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0xC0}},
	{0xCB,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x00,0x05,0x05,0x05,0x05,0x05}},
	{0x00,1,{0xD0}},
	{0xCB,15,{0x05,0x05,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05}},
	{0x00,1,{0xE0}},
	{0xCB,14,{0x05,0x00,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00}},
	{0x00,1,{0xF0}},
	{0xCB,11,{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},
	{0x00,1,{0x80}},
	{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x07,0x00,0x0D,0x09,0x0F,0x0B,0x11}},
	{0x00,1,{0x90}},
	{0xCC,15,{0x15,0x13,0x17,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06}},
	{0x00,1,{0xA0}},
	{0xCC,14,{0x08,0x00,0x0E,0x0A,0x10,0x0C,0x12,0x16,0x14,0x18,0x02,0x04,0x00,0x00}},
	{0x00,1,{0xB0}},
	{0xCC,15,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x00,0x0C,0x10,0x0A,0x0E,0x18}},
	{0x00,1,{0xC0}},
	{0xCC,15,{0x14,0x16,0x12,0x08,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03}},
	{0x00,1,{0xD0}},
	{0xCC,14,{0x01,0x00,0x0B,0x0F,0x09,0x0D,0x17,0x13,0x15,0x11,0x07,0x05,0x00,0x00}},
	{0x00,1,{0x80}},
	{0xCE,12,{0x87,0x03,0x28,0x86,0x03,0x28,0x85,0x03,0x28,0x84,0x03,0x28}},
	{0x00,1,{0x90}},
	{0xCE,14,{0x34,0xFC,0x28,0x34,0xFD,0x28,0x34,0xFE,0x28,0x34,0xFF,0x28,0x00,0x00}},
	{0x00,1,{0xA0}},
	{0xCE,14,{0x38,0x07,0x05,0x00,0x00,0x28,0x00,0x38,0x06,0x05,0x01,0x00,0x28,0x00}},
	{0x00,1,{0xB0}},
	{0xCE,14,{0x38,0x05,0x05,0x02,0x00,0x28,0x00,0x38,0x04,0x05,0x03,0x00,0x28,0x00}},
	{0x00,1,{0xC0}},
	{0xCE,14,{0x38,0x03,0x05,0x04,0x00,0x28,0x00,0x38,0x02,0x05,0x05,0x00,0x28,0x00}},
	{0x00,1,{0xD0}},
	{0xCE,14,{0x38,0x01,0x05,0x06,0x00,0x28,0x00,0x38,0x00,0x05,0x07,0x00,0x28,0x00}},
	{0x00,1,{0x80}},
	{0xCF,14,{0x78,0x07,0x04,0xFF,0x00,0x18,0x10,0x78,0x06,0x05,0x00,0x00,0x18,0x10}},
	{0x00,1,{0x90}},
	{0xCF,14,{0x78,0x05,0x05,0x01,0x00,0x18,0x10,0x78,0x04,0x05,0x02,0x00,0x18,0x10}},
	{0x00,1,{0xA0}},
	{0xCF,14,{0x70,0x00,0x05,0x00,0x00,0x18,0x10,0x70,0x01,0x05,0x01,0x00,0x18,0x10}},
	{0x00,1,{0xB0}},
	{0xCF,14,{0x70,0x02,0x05,0x02,0x00,0x18,0x10,0x70,0x03,0x05,0x03,0x00,0x18,0x10}},
	{0x00,1,{0xC0}},
	{0xCF,11,{0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x81,0x00,0x03,0x08}},
	{0x00,1,{0x00}},
	{0xE1,16,{0x00,0x04,0x08,0x0B,0x04,0x0D,0x0C,0x0C,0x03,0x06,0x12,0x0F,0x17,0x30,0x25,0x06}},
	{0x00,1,{0x00}},
	{0xE2,16,{0x00,0x04,0x08,0x0B,0x04,0x0D,0x0C,0x0C,0x03,0x06,0x12,0x0F,0x17,0x30,0x25,0x06}},
	{0x00,1,{0x90}}, 			//Clever CMD1 
	{0xd6,1,{0x00}}, 
	{0x00,1,{0x00}}, 			//CE - Low 
	{0x55,1,{0x80}},
	{0x00,1,{0x00}}, 			
	{0x36,1,{0xc0}},
	{0x35,1,{0x00}},	
	{0x00,1,{0x00}},
	{0xFF,3,{0xFF,0xFF,0xFF}},
	{0x2C,1,{0x00}}, 
	{0x11,1,{0x00}}, 
	{REGFLAG_DELAY, 120, {}},
	{	0X29,0X01,{0X00}},		
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
//                dsi_set_cmdq_dcs(cmd, table[i].count, table[i].para_list, force_update);
        }
    }

}

static void init_lcm_registers(void)
{
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


//static void init_lcm_registers(void)
//{
//	unsigned int data_array[16];

//	//	{0x00,0X01,{0x00}},
////	data_array[0]= 0x00023902;	
////	data_array[1]= 0x00000000;	
////	dsi_set_cmdq(data_array, 2, 1);	
//	data_array[0]= 0x00001500;		
//	dsi_set_cmdq(data_array, 1, 1); 

//	//	{0XFF,0X03,{0X12,0X83,0x01}},
//	data_array[0]= 0x00043902;	
//	data_array[1]= 0x018312ff;	
//	dsi_set_cmdq(data_array, 2, 1);	

//	//	{0x00,0X01,{0x80}}, 
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0XFF,0X02,{0X12,0X83}},
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x008312ff;	
//	dsi_set_cmdq(data_array, 2, 1);	
//	
//	//	{0X00,0X01,{0X92}},
//	data_array[0]= 0x92001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0Xff,0X02,{0x30,0x02}},
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x000230ff;	
//	dsi_set_cmdq(data_array, 2, 1);	

//	//	{0X00,0X01,{0X90}},
//	data_array[0]= 0x90001500;		
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0Xf5,0X04,{0x02,0x11,0x02,0x11}},
//	data_array[0]= 0x00053902;	
//	data_array[1]= 0x021102f5;
//	data_array[2]= 0x00000011;
//	dsi_set_cmdq(data_array, 3, 1);	
//	
//	//	{0X00,0X01,{0X90}},
//	data_array[0]= 0x90001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0Xc5,0X01,{0X50}},
//	data_array[0]= 0x50c51500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0X00,0X01,{0X94}},
//	data_array[0]= 0x94001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0Xc5,0X01,{0X77}},
//	data_array[0]= 0x77c51500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0X00,0X01,{0X80}},
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0Xc0,0x09,{0x00,0x64,0x00, 0x10,0x10,0x00,0x64, 0x10,0x10}},
//	data_array[0]= 0x000a3902;	
//	data_array[1]= 0x006400c0;	
//	data_array[2]= 0x64001010;	
//	data_array[3]= 0x00001010;	
//	dsi_set_cmdq(data_array, 4, 1);
//	
//	//	{0X00,0X01,{0X90}},
//	data_array[0]= 0x90001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xc0,0x06,{0x00,0x5c,0x00,  0x01,0x00,0x04}},
//	data_array[0]= 0x00073902;	
//	data_array[1]= 0x005c00c0;	
//	data_array[2]= 0x00040001;	
//	dsi_set_cmdq(data_array, 3, 1);
//	
//	//	{0X00,0X01,{0Xb3}},
//	data_array[0]= 0xb3001500;	
//	dsi_set_cmdq(data_array, 1, 1);

//	//	{0Xc0,0X02,{0X00,0X50}},
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x005000c0;	
//	dsi_set_cmdq(data_array, 2, 1);
//	
//	//	{0X00,0X01,{0X81}},
//	data_array[0]= 0x81001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xc1,0X01,{0X66}},
//	data_array[0]= 0x55c11500;	    //huxh
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0X00,0X01,{0Xa0}},
//	data_array[0]= 0xa0001500;	
//	dsi_set_cmdq(data_array, 1, 1);

//	//	{0Xc4,0X0E,{0x05,0x10,0x06,| 0x02,0x05,0x15,0x10, | 0x05,0x10,0x07,0x02, | 0x05,0x15,0x10}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x061005c4;	
//	data_array[2]= 0x10150502;	
//	data_array[3]= 0x02071005;	
//	data_array[4]= 0x00101505;	
//	dsi_set_cmdq(data_array, 5, 1);
//	
//	//	{0X00,0X01,{0Xb0}},
//	data_array[0]= 0xb0001500;	
//	dsi_set_cmdq(data_array, 1, 1);

//	//	{0Xc4,0X02,{0X00,0X00}},
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x000000c4;	
//	dsi_set_cmdq(data_array, 2, 1);
//	
//	//	{0X00,0X01,{0X91}},
//	data_array[0]= 0x91001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xc5,0X02,{0X46,0X40}},
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x004046c5;	
//	dsi_set_cmdq(data_array, 2, 1);
//	
//	//	{0X00,0X01,{0X00}},
//	data_array[0]= 0x00001500;	
//	dsi_set_cmdq(data_array, 1, 1);

//	//	{0Xd8,0X02,{0Xbc,0Xbc}},
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x00bcbcd8;	
//	dsi_set_cmdq(data_array, 2, 1);

//	//	{0X00,0X01,{0X00}},
//	data_array[0]= 0x00001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xd9,0X01,{0X30}},
//	data_array[0]= 0x30d91500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0X00,0X01,{0Xb0}},
//	data_array[0]= 0xb0001500;	
//	dsi_set_cmdq(data_array, 1, 1);

//	//	{0Xc5,0X02,{0X04,0x38}},
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x003804c5;	
//	dsi_set_cmdq(data_array, 2, 1);
//	
//	//	{0X00,0X01,{0Xbb}},
//	data_array[0]= 0xbb001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xc5,0X01,{0X80}},
//	data_array[0]= 0x80c51500;	
//	dsi_set_cmdq(data_array, 1, 1);

//	//	{0X00,0X01,{0Xc3}},
//	data_array[0]= 0xc3001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xf5,0X01,{0X81}},
//	data_array[0]= 0x81f51500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0X00,0X01,{0X82}},
//	data_array[0]= 0x82001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xf4,0X01,{0X00}}, 
//	data_array[0]= 0x00f41500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0X00,0X01,{0X80}},
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xc6,0X01,{0X24}}, 
//	data_array[0]= 0x24c61500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0X00,0X01,{0X00}},
//	data_array[0]= 0x00001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xd0,0X01,{0X40}}, 
//	data_array[0]= 0x40d01500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0X00,0X01,{0X00}},
//	data_array[0]= 0x00001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0Xd1,0X02,{0X00,0X00}},	
//	data_array[0]= 0x00033902;	
//	data_array[1]= 0x000000d1;
//	dsi_set_cmdq(data_array, 2, 1);

////===========================================
////SSD2825_Gen_write_1A_1P(0x00,0x94);  //VCL on   
//	data_array[0]= 0x94001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
////SSD2825_Gen_write_1A_1P(0xF5,0x02);
//	data_array[0]= 0x02f51500;	
//	dsi_set_cmdq(data_array, 1, 1); 

////SSD2825_Gen_write_1A_1P(0x00,0xBA);  //VSP on  
//	data_array[0]= 0xba001500;	
//	dsi_set_cmdq(data_array, 1, 1); 

////SSD2825_Gen_write_1A_1P(0xF5,0x03);
//	data_array[0]= 0x03f51500;	
//	dsi_set_cmdq(data_array, 1, 1); 

////===========================================
//	
//	//	{0X00,0X01,{0X80}},
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0xcb,0x0b,{0x00,0x00,0x00,-0x00,0x00,0x00,0x00,-0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x000c3902;	
//	data_array[1]= 0x000000cb;	
//	data_array[2]= 0x00000000;
//	data_array[3]= 0x00000000;
//	dsi_set_cmdq(data_array, 4, 1);
//	
//	//	{0X00,0X01,{0X90}}, 
//	data_array[0]= 0x90001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0xCB,0x0F,{0x00,0x00,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x000000cb;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0Xa0}}, 
//	data_array[0]= 0xa0001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0xCB,0x0F,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x000000cb;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0Xb0}}, 
//	data_array[0]= 0xb0001500;	
//	dsi_set_cmdq(data_array, 1, 1);

//	
//	//	{0xCB,0x0F,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x000000cb;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0Xc0}}, 
//	data_array[0]= 0xc0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCB,0x0F,{0x05,0x00,0x05=,0x05,0x05,0x05,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x050005cb;	
//	data_array[2]= 0x00050505;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0Xd0}}, 
//	data_array[0]= 0xd0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCB,0x0F,{0x00,0x00,0x00,=0x05,0x00,0x05,0x05,=0x05,0x00,0x05,0x05,=0x05,0x05,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x000000cb;	
//	data_array[2]= 0x05050005;	
//	data_array[3]= 0x05050005;	
//	data_array[4]= 0x00000505;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0Xe0}}, 
//	data_array[0]= 0xe0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCB,0x0E,{0x00,0x00,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x05, =0x00,0x05,0x05}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x000000cb;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x05000000;	
//	data_array[4]= 0x00050500;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0Xf0}}, 
//	data_array[0]= 0xf0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCB,0x0b,{0xff,0xff,0xff,=0xff,0xff,0xff,0xff,=0xff,0xff,0xff,0xff}},
//	data_array[0]= 0x000c3902;	
//	data_array[1]= 0xffffffcb;	
//	data_array[2]= 0xffffffff;	
//	data_array[3]= 0xffffffff;	
//	dsi_set_cmdq(data_array, 4, 1);

//	//	{0X00,0X01,{0X80}},
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCC,0x0F,{0x02,0x00,0x0a,=0x0e,0x0c,0x10,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x0a0002cc;	
//	data_array[2]= 0x00100c0e;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0X90}}, 
//	data_array[0]= 0x90001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCC,0x0F,{0x00,0x00,0x00,=0x06,0x00,0x2e,0x2d,=0x01,0x00,0x09,0x0d,=0x0b,0x0f,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x000000cc;	
//	data_array[2]= 0x2d2e0006;	
//	data_array[3]= 0x0d090001;	
//	data_array[4]= 0x00000f0b;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XA0}}, 
//	data_array[0]= 0xa0001500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0xCC,0x0E,{0x00,0x00,0x00,=0x00,0x00,0x00,0x00=,0x00,0x00,0x00,0x05,=0x00,0x2e,0x2d}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x000000cc;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x05000000;	
//	data_array[4]= 0x002d2e00;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XB0}}, 
//	data_array[0]= 0xb0001500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0xCC,0x0F,{0x05,0x00,0x0f,=0x0b,0x0d,0x09,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x0f0005cc;	
//	data_array[2]= 0x00090d0b;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XC0}}, 
//	data_array[0]= 0xc0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCC,0x0F,{0x00,0x00,0x00,=0x01,0x00,0x2d,0x2e,=0x06,0x00,0x10,0x0c,=0x0e,0x0a,0x00,0x00}},
//	data_array[0]= 0x00103902;	
//	data_array[1]= 0x000000cc;	
//	data_array[2]= 0x2e2d0001;	
//	data_array[3]= 0x0c100006;	
//	data_array[4]= 0x00000a0e;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XD0}}, 
//	data_array[0]= 0xd0001500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0xCC,0x0E,{0x00,0x00,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x02,=0x00,0x2d,0x2e}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x000000cc;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x02000000;	
//	data_array[4]= 0x002e2d00;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0X80}}, 
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	//	{0xCE,0x0C,{0x87,0x03,0x18,=0x86,0x03,0x18,0x00,=0x00,0x00,0x00,0x00,=0x00}},
//	data_array[0]= 0x000d3902;	
//	data_array[1]= 0x180387ce;	
//	data_array[2]= 0x00180386;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0X90}}, 
//	data_array[0]= 0x90001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCE,0x0E,{0x34,0xfe,0x18,=0x34,0xff,0x18,0x00=,0x00,0x00,0x00,0x00,=0x00,0x00,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x18fe34ce;	
//	data_array[2]= 0x0018ff34;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XA0}}, 
//	data_array[0]= 0xa0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCE,0x0E,{0x38,0x03,0x05,=0x00,0x00,0x18,0x00,=0x38,0x02,0x05,0x01,=0x00,0x18,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x050338ce;	
//	data_array[2]= 0x00180000;	
//	data_array[3]= 0x01050238;	
//	data_array[4]= 0x00001800;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XB0}}, 
//	data_array[0]= 0xb0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCE,0x0E,{0x38,0x01,0x05,=0x02,0x00,0x18,0x00,=0x38,0x00,0x05,0x03,=0x00,0x18,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x050138ce;	
//	data_array[2]= 0x00180002;	
//	data_array[3]= 0x03050038;	
//	data_array[4]= 0x00001800;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XC0}}, 
//	data_array[0]= 0xc0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCE,0x0E,{0x30,0x00,0x05,=0x04,0x00,0x18,0x00,=0x30,0x01,0x05,0x05,=0x00,0x18,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x050030ce;	
//	data_array[2]= 0x00180004;	
//	data_array[3]= 0x05050130;	
//	data_array[4]= 0x00001800;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XD0}}, 
//	data_array[0]= 0xd0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCE,0x0E,{0x30,0x02,0x05,=0x06,0x00,0x18,0x00,=0x30,0x03,0x05,0x07,=0x00,0x18,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x050230ce;	
//	data_array[2]= 0x00180006;	
//	data_array[3]= 0x07050330;	
//	data_array[4]= 0x00001800;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0X80}}, 
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCF,0x0E,{0x00,0x00,0x00=,0x00,0x00,0x00,0x00,=0x00,0x00,0x00,0x00,=0x00,0x00,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x000000cf;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0X90}}, 
//	data_array[0]= 0x90001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCF,0x0E,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x000000cf;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XA0}}, 
//	data_array[0]= 0xa0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCF,0x0E,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x000000cf;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XB0}}, 
//	data_array[0]= 0xb0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCF,0x0E,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
//	data_array[0]= 0x000f3902;	
//	data_array[1]= 0x000000cf;	
//	data_array[2]= 0x00000000;	
//	data_array[3]= 0x00000000;	
//	data_array[4]= 0x00000000;	
//	dsi_set_cmdq(data_array, 5, 1);

//	//	{0X00,0X01,{0XC0}}, 
//	data_array[0]= 0xc0001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xCF,0x0A,{0x01,0x01,0x20,==0x20,0x00,0x00,0x01,==0x01,0x00,0x00}},
//	data_array[0]= 0x000b3902;	
//	data_array[1]= 0x200101cf;	
//	data_array[2]= 0x01000020;	
//	data_array[3]= 0x00000001;	
//	dsi_set_cmdq(data_array, 4, 1);

//	//	{0X00,0X01,{0XB5}}, 
//	data_array[0]= 0xb5001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0xC5,0x06,{0x37,0xf1,0xfd,=0x37,0xf1,0xfd}},
//	data_array[0]= 0x00073902;	
//	data_array[1]= 0xfdf137c5;	
//	data_array[2]= 0x00fdf137;	
//	dsi_set_cmdq(data_array, 3, 1);

//	//	{0X00,0X01,{0X00}}, 
//	data_array[0]= 0x00001500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0xE1,0x10,{0x10,0x15,0x1B,=0x11,0x0A,0x1D,0x0D,=0x0D,0x01,0x05,0x02,=0x07,0x0F,0x1F,0x1C,=0x08}},
//	data_array[0]= 0x00113902;	
//	data_array[1]= 0x1b1510e1;	
//	data_array[2]= 0x0d1d0a11;	
//	data_array[3]= 0x0205010d;	
//	data_array[4]= 0x1c1f0f07;	
//	data_array[5]= 0x00000008;	
//	dsi_set_cmdq(data_array, 6, 1);

//	//	{0X00,0X01,{0X00}}, 
//	data_array[0]= 0x00001500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0xE2,0x10,{0x10,0x15,0x1B,=0x11,0x0A,0x1D,0x0D,=0x0D,0x01,0x05,0x02,=0x07,0x0F,0x1F,0x1C,=0x08}},
//	data_array[0]= 0x00113902;	
//	data_array[1]= 0x1b1510e2;	
//	data_array[2]= 0x0d1d0a11;	
//	data_array[3]= 0x0205010d;	
//	data_array[4]= 0x1c1f0f07;	
//	data_array[5]= 0x00000008;	
//	dsi_set_cmdq(data_array, 6, 1);

//	//	{0X00,0X01,{0X90}}, 
//	data_array[0]= 0x90001500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0XB3,0X01,{0X02}}, 
//	data_array[0]= 0x02b31500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0X00,0X01,{0X92}}, 
//	data_array[0]= 0x92001500;	
//	dsi_set_cmdq(data_array, 1, 1);		
//	
//	//	{0XB3,0X01,{0X40}}, 
//	data_array[0]= 0x40b31500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0X00,0X01,{0X80}}, 
//	data_array[0]= 0x80001500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0Xf6,0X01,{0X01}}, 
//	data_array[0]= 0x01f61500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
////=======================#ifdef DISPLAY_DIRECTION_0_MODE
////	//	{0x36, 1 ,{0x00}},// Display Direction 0
////	data_array[0]= 0x00023902;	
////	data_array[1]= 0x00000036;
////	dsi_set_cmdq(data_array, 2, 1);	
////	
////	//	{0x35, 1 ,{0x00}},// TE( Fmark ) Signal On
////	data_array[0]= 0x00023902;	
////	data_array[1]= 0x00000035;
////	dsi_set_cmdq(data_array, 2, 1);	
////	
////	//	{0x44, 2 ,{0x01,0x22}},// TE( Fmark ) Signal Output Position
////	data_array[0]= 0x00033902;	
////	data_array[1]= 0x00220144;
////	dsi_set_cmdq(data_array, 2, 1);	

////#ifdef DISPLAY_DIRECTION_180_MODE
////	{0x36, 1 ,{0xD0}},// Display Direction 180
//	data_array[0]= 0xd0361500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
////	{0x35, 1 ,{0x00}},// TE( Fmark ) Signal On
////	data_array[0]= 0x00351500;	
////	dsi_set_cmdq(data_array, 1, 1);	

////	{0x44, 2 ,{0x01,0xFF}},// TE( Fmark ) Signal Output Position
////	data_array[0]= 0x00033902;	
////	data_array[1]= 0x00ff0144;
////	dsi_set_cmdq(data_array, 2, 1);	

////======================================================end
//	//	{0X21,0X01,{0X00}}, 
//	data_array[0]= 0x00210500;	
//	dsi_set_cmdq(data_array, 1, 1);	
//	
//	//	{0X11,0X00,{}}, 
////	data_array[0]= 0x00023902;	
////	data_array[1]= 0x00000011;
////	dsi_set_cmdq(data_array, 2, 1);	
//	data_array[0]= 0x00110500;	
//	dsi_set_cmdq(data_array, 1, 1); 

//	MDELAY(200);
//	//	{REGFLAG_DELAY, 200, {}},
//	//	{0X29,0X00,{}}, 
////	data_array[0]= 0x00023902;	
////	data_array[1]= 0x00000029;
////	dsi_set_cmdq(data_array, 2, 1);	
//	data_array[0]= 0x00290500;	
//	dsi_set_cmdq(data_array, 1, 1); 

//	//	{REGFLAG_DELAY, 200, {}},
//	MDELAY(200);

//}

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
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		// enable tearing-free
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

        #if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
        #else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
//		params->dsi.mode   = BURST_VDO_MODE;
		//params->dsi.mode   = SYNC_EVENT_VDO_MODE; 
		
        #endif
	
		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
//		params->dsi.LANE_NUM				= LCM_THREE_LANE;
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
//		params->dsi.packet_size=256;
		params->dsi.packet_size=128;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
//		params->dsi.word_count=720*3;	

		
//		params->dsi.vertical_sync_active		=2;//	= 4;//=2;//	= 4;//3;  //---3
//		params->dsi.vertical_backporch			=14;//	= 14; //=20;//	= 14; //14; //---14
//		params->dsi.vertical_frontporch			=16;//	= 16; //=35;//	= 16; // 8;  //----8
//		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

//		params->dsi.horizontal_sync_active		=2;//	=2;//=2;//	=2;//= 2;  //----2
//		params->dsi.horizontal_backporch		=34;//	= 42; //=18;//	= 42; //----28
//		params->dsi.horizontal_frontporch		=24;//	= 44; //=20;//	= 44; //----50
//		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;


//		params->dsi.vertical_sync_active		= 4;//=2;// = 4;//3;  //---3
//		params->dsi.vertical_backporch			= 14; //=20;//	= 14; //14; //---14
//		params->dsi.vertical_frontporch 		= 16; //=35;//	= 16; // 8;  //----8
//		params->dsi.vertical_active_line				= FRAME_HEIGHT; 
//		
//		params->dsi.horizontal_sync_active		=2;//=2;//	=2;//= 2;  //----2
//		params->dsi.horizontal_backporch		= 42; //=18;//	= 42; //----28
//		params->dsi.horizontal_frontporch		= 44; //=20;//	= 44; //----50
//		params->dsi.horizontal_active_pixel 			= FRAME_WIDTH;

		params->dsi.vertical_sync_active				= 4;
		params->dsi.vertical_backporch					= 20;//14;
		params->dsi.vertical_frontporch					= 20;//16;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 10;//8;
		params->dsi.horizontal_backporch				= 150;//104;
		params->dsi.horizontal_frontporch				= 150;//24;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;  // ÆµÂÊ ((150+150+10)+720)*((20+20+4)+1280)*60*24=
																			// 1030 x 1324 x60 x 24 / 4

        //	params->dsi.HS_PRPR=6;
//	    params->dsi.LPX=8; 
//		//params->dsi.HS_PRPR=5;
//		//params->dsi.HS_TRAIL=13;
//		params->dsi.CLK_TRAIL = 10;
		// Bit rate calculation
		//1 Every lane speed
		params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		params->dsi.fbk_div =19;//34; // 19;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	

//		params->dsi.HS_TRAIL	= 10;//10;//min max(n*8*UI, 60ns+n*4UI)  HS_TRAIL is low set 10
//		params->dsi.HS_ZERO    = 7;//8;//min 105ns+6*UI 
//		params->dsi.HS_PRPR    = 4;//min 40ns+4*UI; max 85ns+6UI 
//		params->dsi.LPX 	   = 12;//min 50ns 
//		params->dsi.TA_SACK=0x01;	
//		params->dsi.TA_GET=50;
//		params->dsi.TA_SURE=15;  
//		params->dsi.TA_GO	   = 12;//12;//4*LPX
//		params->dsi.CLK_TRAIL	= 10;//5;//min 60ns   CLK_TRAIL is low	set 10
//		params->dsi.CLK_ZERO	= 16;//18;//min 300ns-38ns
//		params->dsi.LPX_WAIT	= 10;
//		params->dsi.CONT_DET	= 0; 
//		params->dsi.CLK_HS_PRPR = 4;//min 38ns; max 95ns
//		params->dsi.LPX = 6;
//		params->dsi.HS_PRPR = 6;
//		params->dsi.HS_ZERO = 11;
//		params->dsi.CLK_HS_POST = 15;
//		params->dsi.CLK_HS_PRPR = 7;

}

static void lcm_init(void)
{

	SET_RESET_PIN(1);
//	MDELAY(50);
	SET_RESET_PIN(0);
	MDELAY(100);

	SET_RESET_PIN(1);
	MDELAY(100); 	 

	SET_RESET_PIN(0);
	MDELAY(100);

	SET_RESET_PIN(1);
	MDELAY(100); 	    

	init_lcm_registers();

}


static void lcm_suspend(void)
{
//	unsigned int data_array[16];

//    SET_RESET_PIN(0);
//    MDELAY(100);
//    SET_RESET_PIN(1);
//	MDELAY(100);

	push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);
    SET_RESET_PIN(1);
    MDELAY(100);
    SET_RESET_PIN(0);
	MDELAY(100);

//	data_array[0]=0x00280500; // Display Off
//	dsi_set_cmdq(data_array, 1, 1);
//	
//	data_array[0] = 0x00100500; // Sleep In
//	dsi_set_cmdq(data_array, 1, 1);

}


static void lcm_resume(void)
{
//	SET_RESET_PIN(1);
////	MDELAY(50);
//	SET_RESET_PIN(0);
//	MDELAY(100);

//	SET_RESET_PIN(1);
//	MDELAY(100); 	 

//	SET_RESET_PIN(0);
//	MDELAY(100);

//	SET_RESET_PIN(1);
//	MDELAY(100); 	

//	init_lcm_registers();
	lcm_init();
}

//static int esd_cnt=0;
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
	lcm_init();
//	MDELAY(100);
//	lcm_init();
	printk("jacob test lcm_esd_recover\n");
#endif
	return TRUE;
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

static unsigned int lcm_compare_id(void)
{
	return 1;
//	unsigned int id=0;
//	unsigned char buffer[2];
//	unsigned int array[16];  

//	SET_RESET_PIN(1);
//	SET_RESET_PIN(0);
//	MDELAY(1);
//	
//	SET_RESET_PIN(1);
//	MDELAY(20); 

//	array[0] = 0x00023700;// read id return two byte,version and id
//	dsi_set_cmdq(array, 1, 1);
//	
//	read_reg_v2(0xF4, buffer, 2);
//	id = buffer[0]; //we only need ID
//    #ifdef BUILD_LK
//		printf("%s, LK nt35590 debug: nt35590 id = 0x%08x\n", __func__, id);
//    #else
//		printk("%s, kernel nt35590 horse debug: nt35590 id = 0x%08x\n", __func__, id);
//    #endif

//    if(id == LCM_ID_NT35590)
//    	return 1;
//    else
//        return 0;
}


LCM_DRIVER otm1283a_hd720_vdo_movego_lcm_drv = 
{
    .name			= "otm1283a_hd720_vdo_movego",
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
