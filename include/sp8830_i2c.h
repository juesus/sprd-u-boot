/******************************************************************************
 ** File Name:      I2C_drv_new.h                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.                                   *
 ******************************************************************************/
#ifndef _I2C_DRVAPI_H_
#define _I2C_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#include "asm/arch/sci_types.h"

/**---------------------------------------------------------------------------*
**                               Micro Define                                                           **
**---------------------------------------------------------------------------*/
typedef struct
{
    uint32 id;
    uint32 freq;
    uint32 bus;
    uint8 slave_addr;
    uint8 reg_addr_num;
    uint8 check_ack;
    uint8 no_stop;
} I2C_DEV;

/**---------------------------------------------------------------------------*
 **                      I2C IOCTL
 **---------------------------------------------------------------------------*/
#define I2C_CTL_G_FREQ 0x20 /*get frequency*/
#define I2C_CTL_S_FREQ 0x21 /*set frequency*/
#define I2C_CTL_G_PORT 0x22 /*get port*/
#define I2C_CTL_S_PORT 0x23 /*set port*/
#define I2C_CTL_STOP_BUS 0x24 /*stop i2c bus*/

/**---------------------------------------------------------------------------*
 **                      Function  Prototype
 **---------------------------------------------------------------------------*/
/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
int32 I2C_HAL_Open (I2C_DEV *dev);

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
int32 I2C_HAL_Close (uint32 handle);

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
uint32 I2C_HAL_Read (uint32 handle, uint8 *reg_addr, uint8 *buffer, uint32 bytes);

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
uint32 I2C_HAL_Write (uint32 handle, uint8 *reg_addr, uint8 *buffer, uint32 bytes);

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
uint32 I2C_HAL_Ioctl (uint32 handle, uint32 cmd, uint32 *arg);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/* End Of File  */