/******************************************************************************
 ** File Name:      syscnt_drv.h                                                 *
 ** Author:         Nick.zhao                                                  *
 ** DATE:           01/20/2007                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic interfaces of System Counter     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/20/2007     Nick.zhao        Create.                                   *
 ******************************************************************************/
#ifndef _SYSCNT_DRV_H_
#define _SYSCNT_DRV_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip.h"

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
/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
#define MIN_SYS_TIMER_TIME       0x1

//define the syscnt timer callback type
typedef void (*SYSCNT_CALLBACK) (uint32);


/**---------------------------------------------------------------------------*
 **                      Function  Prototype
 **---------------------------------------------------------------------------*/
/*********************************************************************/
//  Description: Initialize the System counter
//  Input:
//      param         not use
//  Return:
//      None
//  Note:
//      None
/*********************************************************************/
PUBLIC void Syscnt_Init (void);


/*********************************************************************/
//  Description: Set the syscnt timer
//  Input:
//      time     The time to be set (ms)
//      st_fun   The callback function
//      param    The callback function's parameter
//  Return:
//      0        Successfully
//  Note:
//      None
/*********************************************************************/
PUBLIC uint32 Syscnt_SetTimer (uint32 time, SYSCNT_CALLBACK st_fun, uint32 param);

/*********************************************************************/
//  Description: the Syscnt timer's isr
//  Input:
//      none
//  Return:
//      none
//  Note:
//      None
/*********************************************************************/
PUBLIC void Syscnt_ISR (uint32 int_num);

/*********************************************************************/
//  Description: Clear the current timer
//  Input:
//  Return:
//  Note:
//      None
/*********************************************************************/
void Syscnt_ClearTimer (void);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif /* SYSCNT_H  */

/* End Of File  */