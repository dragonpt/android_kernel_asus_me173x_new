#ifndef __LINUX_FT5X06_EX_FUN_H__
#define __LINUX_FT5X06_EX_FUN_H__

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <mach/irqs.h>

#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/string.h>

#define IC_FT5X06	0
#define IC_FT5606	1
#define IC_FT5316	2

#define FT_UPGRADE_AA	0xAA
#define FT_UPGRADE_55 	0x55
#define FT_UPGRADE_EARSE_DELAY		2000

/*upgrade config of FT5606*/
#define FT5606_UPGRADE_AA_DELAY 		50
#define FT5606_UPGRADE_55_DELAY 		10
#define FT5606_UPGRADE_ID_1			0x79
#define FT5606_UPGRADE_ID_2			0x06
#define FT5606_UPGRADE_READID_DELAY 	100

/*upgrade config of FT5316*/
#define FT5316_UPGRADE_AA_DELAY 		50
#define FT5316_UPGRADE_55_DELAY 		40
#define FT5316_UPGRADE_ID_1			0x79
#define FT5316_UPGRADE_ID_2			0x07
#define FT5316_UPGRADE_READID_DELAY 	1

/*upgrade config of FT5x06(x=2,3,4)*/
#define FT5X06_UPGRADE_AA_DELAY 		50
#define FT5X06_UPGRADE_55_DELAY 		30
#define FT5X06_UPGRADE_ID_1			0x79
#define FT5X06_UPGRADE_ID_2			0x03
#define FT5X06_UPGRADE_READID_DELAY 	1


#define DEVICE_IC_TYPE	IC_FT5316

#define    FTS_PACKET_LENGTH        128
#define    FTS_SETTING_BUF_LEN        128

//#define    AUTO_CLB


/*create sysfs for debug*/
int ft5x0x_create_sysfs(struct i2c_client * client);

void ft5x0x_release_mutex(void);

/*
*ft5x0x_write_reg- write register
*@client: handle of i2c
*@regaddr: register address
*@regvalue: register value

*
*/
int ft5x0x_write_reg(struct i2c_client * client,u8 regaddr, u8 regvalue);

int ft5x0x_read_reg(struct i2c_client * client,u8 regaddr, u8 * regvalue);

#endif
