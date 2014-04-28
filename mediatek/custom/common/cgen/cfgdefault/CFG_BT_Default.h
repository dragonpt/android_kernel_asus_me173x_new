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


#ifndef _CFG_BT_DEFAULT_H
#define _CFG_BT_DEFAULT_H

///#include "../cfgfileinc/CFG_BT_File.h"

// the default value of bt nvram file
#ifdef MTK_COMBO_SUPPORT
static ap_nvram_btradio_mt6610_struct stBtDefault =
{
#if defined(MTK_MT6620)
    {0x00, 0x00, 0x46, 0x66, 0x20, 0x01},
#else
    {0x00, 0x00, 0x46, 0x66, 0x28, 0x01},
#endif
    {0x60, 0x00},
#if defined(MTK_MT6628) && defined(MTK_MERGE_INTERFACE_SUPPORT)
// 6589 PCM_I2S_MERGE_PIN
    {0x63, 0x10, 0x00, 0x00},
#else
    {0x23, 0x10, 0x00, 0x00},
#endif
#if defined(MTK_MT6620)
    {0x06, 0x80, 0x00, 0x06, 0x03, 0x06},
#else
    {0x07, 0x80, 0x00, 0x06, 0x05, 0x07},
#endif
    {0x03, 0x40, 0x1F, 0x40, 0x1F, 0x00, 0x04},
    {0x80, 0x00},
    {0xFF, 0xFF, 0xFF}
};
#else
// MT661x is phased out, current for MT662x
static ap_nvram_btradio_mt6610_struct stBtDefault =
{
#if defined(MTK_MT6622)
    {0x00, 0x00, 0x46, 0x66, 0x22, 0x01}, /*addr*/
#else
    {0x00, 0x00, 0x46, 0x66, 0x26, 0x01}, /*addr*/
#endif
    {0x40}, /*CapId*/
    {0x01}, /*LinkKeyType*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*UintKey*/
    {0x00, 0x02, 0x10}, /*Encryption*/
    {0x00}, /*PinCodeType*/
    {0x60, 0x00}, /*Voice*/
    {0x23, 0x00, 0x00, 0x00}, /*Codec*/
    {0x07, 0x80, 0x00, 0x06, 0x03, 0x07}, /*Radio*/
    {0x03, 0x40, 0x1F, 0x40, 0x1F, 0x00, 0x04}, /*Sleep*/
    {0x80, 0x00}, /*BtFTR*/
    {0x00}, /*ECLK_SEL[1]*/
    {0x00}, /*Reserved1[1]*/
    {0x00, 0x00}, /*Reserved2[2]*/
    {0x00, 0x00, 0x00, 0x00}, /*Reserved3[4]*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} /*Reserved4[16]*/
};
#endif

#endif
