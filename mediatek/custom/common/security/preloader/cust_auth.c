/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2011. All rights reserved.
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


#include "sec_platform.h"
#include "sec_auth.h"
#include "sec_error.h"

#define SHA1_HASH_LEN       (20)
#define SHA256_HASH_LEN     (32)
#define RSA1024_SIG_LEN     (128)
#define RSA2048_SIG_LEN     (256)

/**************************************************************************
 *  HASH (should support SHA1 and SHA256)
 **************************************************************************/
int sec_hash (U8* data_buf, U32 data_len, U8* hash_buf, U32 hash_len)
{
    if(SHA1_HASH_LEN == hash_len)
    {
        /* =============== */
        /* SHA1            */
        /* =============== */
        
        /* TODO : use sec_hash to generate hash value */    
        /* customer needs to customized this function */
        return ERR_HASH_IMAGE_FAIL;        
    }
    else if(SHA256_HASH_LEN == hash_len)
    {
        /* =============== */
        /* SHA256          */
        /* =============== */
        
        /* TODO : use sec_hash to generate hash value */    
        /* customer needs to customized this function */
        return ERR_HASH_IMAGE_FAIL;        
    }
    else
    {
        ASSERT(0);
    }
}

/**************************************************************************
 *  RSA (should support RSA1024 and RSA2048)
 **************************************************************************/
int sec_auth (U8* data_buf, U32 data_len, U8* sig_buf, U32 sig_len)
{
    if(RSA1024_SIG_LEN == sig_len)
    {
        U8 sha1sum[SHA1_HASH_LEN] = {0};    
        
        /* =============== */
        /* RSA1024         */
        /* =============== */        

        /* SHA1 */        
        if( sec_hash(data_buf, data_len, sha1sum, SHA1_HASH_LEN) != 0 )
        {
            /* return verify failed */
            return -1;
        }        
        
        /* TODO : use sec_hash to generate hash value */    
        /* customer needs to customized this function */
        return ERR_AUTH_IMAGE_VERIFY_FAIL;        
    }
    else if(RSA2048_SIG_LEN == sig_len)
    {
        U8 sha256sum[SHA256_HASH_LEN] = {0};
    
        /* =============== */
        /* RSA2048         */
        /* =============== */        

        /* SHA256 */      
        if( sec_hash(data_buf, data_len, sha256sum, SHA256_HASH_LEN) != 0 )
        {
            /* return verify failed */
            return -1;
        }        
        
        /* TODO : use sec_hash to generate hash value */    
        /* customer needs to customized this function */
        return ERR_AUTH_IMAGE_VERIFY_FAIL;        
    }
    else
    {
        ASSERT(0);
    }
}

/**************************************************************************
 *  DA AUTH INIT
 **************************************************************************/
U32 da_auth_init (void)
{
    /* customer needs to customized this function */
    return SEC_OK;
}

/**************************************************************************
 *  IMAGE AUTH INIT
 **************************************************************************/
U32 img_auth_init (void)
{       
    /* customer needs to customized this function */
    return SEC_OK;   
}
