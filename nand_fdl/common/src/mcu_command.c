#include "mcu_command.h"
#include "packet.h"
#include "fdl_nand.h"
#include <linux/string.h>
#include "sci_types.h"
#include "sc_reg.h"

typedef void (*BOOT_ENTRY) (void);

/*****************************************************************************/
//  Description:    This function Reset MCU
//  Author:         Haifeng.Yang
//  Note:
/*****************************************************************************/
void WDG_ResetMCU (void)
{
#ifdef PLATFORM_SC8800H
    //When WDG reset the system, the HWRST register will be cleared, so we
    //can save HWRST register's value to DMA register(0X201007E0) first and
    //restore the HWRST register when system restarts.

    //ENABLE MCU READ OR WRITE DMA REGISTERS
    * (volatile uint32 *) AHB_CTL0 |= 0x40;
    * (volatile uint32 *) GR_HWRST1 = * (volatile uint32 *) GR_HWRST;
#endif

#ifndef PLATFORM_SC8800G // modify later
    // Enable watchdog programming
    CHIP_REG_OR (GR_GEN0, GEN0_WDG_EN);//lint !e718

#if defined (PLATFORM_SC6600L)
    CHIP_REG_OR (GR_GEN1, GEN1_WDG_RTC_EN);
#elif defined (PLATFORM_SC8800H)
    CHIP_REG_OR (GR_CLK_EN, 0x4);
#elif defined (PLATFORM_SC6800H)
    CHIP_REG_OR (GR_GEN0, GEN0_WDG_RTC_EN);
#endif
    // Unlock wdg load regiter
    CHIP_REG_SET (WDG_LOCK, WDG_UNLOCK_KEY);//lint !e718

    CHIP_REG_SET (WDG_CTL, (REG32 (WDG_CTL) & ~BIT_0)); //lint !e718//Reset Mode

    CHIP_REG_SET (WDG_LOAD, 0x50);

    // enable wdg timer clock
    CHIP_REG_SET (WDG_CTL, (REG32 (WDG_CTL) | BIT_1));

    // Lock wdg load regiter1
    CHIP_REG_SET (WDG_LOCK, WDG_LOCK_KEY);

    // Disable watchdog programming
    CHIP_REG_AND (GR_GEN0, ~GEN0_WDG_EN);//lint !e718
#endif

    while (1);
}
/* Sorry, I don't know what this function is for.
 *
 * Ming.Zhang 2005-09-22
 */
static void FDL_ResetMcuClock (void)
{
#ifdef MCU_CLK_52M
#define REG(r)      (*((volatile unsigned int*)(r)))

    REG (0x8b000018) |= 1 << 9; // write pll enable
    REG (0x8b000024)  = 0x1E05; // M/N = 5, so clock = 78M
    REG (0x8b000018) &= ~ (1 << 9); // write pll disable

    REG (GR_PCTL) = (unsigned int) (0x0A55);
#endif /* MCU_CLK_52M */
}

int FDL_McuResetBoot (PACKET_T *pakcet, void *arg)
{
    int i;
    BOOT_ENTRY boot_entry = (BOOT_ENTRY) 0; /* The address of ROM Code */

    FDL_SendAckPacket (BSL_REP_ACK);

    /* Wait until all characters are sent out. */
    for (i=0; i<0x0A000; i++)
    {
        /* Do nothing */;
    }

    FDL_ResetMcuClock();

    /* Jump to ROM code */
    (*boot_entry) (); /*lint !e413*/

    /* We should not go here */
    return 0;
}

int FDL_McuResetNormal (PACKET_T *packet, void *arg)
{
    int i;
    BOOT_ENTRY boot_entry = (BOOT_ENTRY) 0x40000000; /* Start of internal RAM */
#if 0

    /* Copy NBL to internal RAM */
    if (NAND_SUCCESS != nand_read_NBL ( (void *) boot_entry))
    {
        send_ack_packet (BSL_REP_OPERATION_FAILED);
        return 0;
    }

#endif
    FDL_SendAckPacket (BSL_REP_ACK);

    /* Wait until all characters are sent out. */
    for (i=0; i<0x0A000; i++)
    {
        /* Do nothing */;
    }

    FDL_ResetMcuClock();

#ifdef PLATFORM_SC8800G // modify later    
    /* Jump to NBL */
    (*boot_entry) ();
#else
    WDG_ResetMCU();
#endif
    /* We should not go here */
    return 0;
}

int FDL_McuReadChipType (PACKET_T *packet, void *arg)
{
    unsigned int id;


    id   =  * (unsigned int *) (0x4FFC);

    if (0x660000B6 != id)
    {
        // @Richard We should check if we want to support 6600C
        id = 0x6600b500;
    }
    else
    {
        id = 0x6600b700;
    }

    packet->packet_body.type = BSL_REP_READ_CHIP_TYPE;
    packet->packet_body.size = 4;
    memcpy (packet->packet_body.content, &id, sizeof (unsigned int));

    FDL_SendPacket (packet);
    return 1;
}