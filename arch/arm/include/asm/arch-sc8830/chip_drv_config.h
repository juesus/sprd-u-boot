/******************************************************************************
 ** File Name:    chip_drv_config.h                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2010  Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.zhan      Create.                                     *
 ******************************************************************************/
#ifndef _CHIP_DRV_CONFIG_H_
#define _CHIP_DRV_CONFIG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

//Special Chip platfrom io
#include "sprd_plf_io.h"
//Chip module config header files.About some variant param. configs for the module.
#include "sprd_module_config.h"
//end Chip Module config

//Chip Global Function(Reg., BASE, Address,)definitions
#include "sprd_reg_base.h"
//All chip module reg.
#include "sprd_module_ver.h"

#define CHIP_PLATFORM_DEFINED

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End