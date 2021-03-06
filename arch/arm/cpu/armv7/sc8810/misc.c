/******************************************************************************
 ** File Name:      msic.c                                             *
 ** Author:         Yong.Li                                              *
 ** DATE:           04/19/2013                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/19/2013     Richard.Yang     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include <asm/arch/sci_types.h>

#include <common.h>
#include <asm/io.h>
#include <asm/arch/regs_adi.h>
#include <asm/arch/regs_ana.h>
#include <asm/arch/adi_hal_internal.h>
#include <asm/arch/sc8810_reg_ahb.h>
#include <asm/arch/sdram_sc7710g2.h>
#include <asm/arch/chip_plf_export.h>
#include <asm/arch/emc_config.h>
#include <asm/arch/misc_api.h>
#include <asm/arch/mfp.h>

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
static unsigned int glb_ana_chipid = 0;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

unsigned int CHIP_PHY_GetChipID(void)
{
#if defined(CONFIG_SC7710G2)
	return CHIP_REG_GET(CHIP_ID);
#else
    return 0;
#endif
}

unsigned int CHIP_PHY_GetANAChipID(void)
{
#if defined(CONFIG_SC7710G2)
	return glb_ana_chipid;
#else
    return 0;
#endif
}

LOCAL void mcu_clock_select(MCU_CLK_SOURCE_E mcu_clk_sel)
{
    uint32 i;

    i = REG32(AHB_ARM_CLK);
    i &= ~(0x3 << 23);
    i |= ((mcu_clk_sel & 0x3) << 23);
    REG32(AHB_ARM_CLK) = i;
}

LOCAL void set_arm_bus_clk_div(uint32 arm_drv, uint32 axi_div, uint32 ahb_div, uint32 dbg_div)
{
    uint32 i;

    // A5 AXI DIV
    i = REG32(CA5_CFG);
    i &= (~(0x3 << 12));
    i |= ((axi_div & 0x3) << 11);
    REG32(CA5_CFG) = i;

    i = REG32(AHB_ARM_CLK);
    i &= (~(0x7 | (0x7 << 4) | (0x7 << 14)));
    i |= (arm_drv & 0x7) | ((ahb_div & 0x7) << 4) | ((dbg_div & 0x7) << 14);

    REG32(AHB_ARM_CLK) = i;

    for(i = 0; i < 50; i++);
}

LOCAL void set_gpu_clock_freq(void)
{
    // GPU AXI 256M
    REG32(GR_GEN2) &= ~(0x3);
}

LOCAL void set_mpll_clock_freq(uint32 clk_freq_hz)
{
    uint32 i;
    uint32 mpll_clk;

    mpll_clk = (clk_freq_hz / 1000000 / 4);

    //APB_GEN1_PCLK M_PLL_CTRL_WE
    REG32(GR_GEN1) |= (1 << 9);

    i = REG32(GR_MPLL_MN);
    i &= ~ 0x7FF;

	i |= (mpll_clk & 0x7FF);

    REG32(GR_MPLL_MN) = i;
    REG32(GR_GEN1) &= ~(1 << 9);
}

LOCAL void set_chip_clock_freq(void)
{
    uint32 arm_drv = 0;
    uint32 axi_div;
    uint32 ahb_div;
    uint32 dbg_div = 0;

    uint32 mpll_clk_freq = EMC_GetPara()->arm_clk;

    if (mpll_clk_freq == CHIP_CLK_26MHZ)
    {
        mcu_clock_select(MCU_CLK_XTL_SOURCE);
        return;
    }
    else if ((mpll_clk_freq >= CHIP_CLK_800MHZ) && (mpll_clk_freq <= CHIP_CLK_1200MHZ))
    {
        axi_div = 1;
        ahb_div = 3; // 1/4
        dbg_div = 7; // 1/8
    }
    else if ((mpll_clk_freq > CHIP_CLK_1200MHZ) && (mpll_clk_freq <= CHIP_CLK_1500MHZ))
    {
        axi_div = 3; // 1/4
        ahb_div = 7; // 1/8
        dbg_div = 7; // 1/8
    }
    else
    {
        SCI_ASSERT(0);
    }

    mcu_clock_select(MCU_CLK_XTL_SOURCE);

    set_gpu_clock_freq();
    set_arm_bus_clk_div(arm_drv, axi_div, ahb_div, dbg_div);
    set_mpll_clock_freq(mpll_clk_freq);

    mcu_clock_select(MCU_CLK_MPLL_SOURCE);
}

__inline LOCAL void set_XOSC32K_config(void)
{
#if defined(CONFIG_SC7710G2)
// from 13.8uA to 7.8uA
    uint16 reg_read;

    reg_read = ADI_Analogdie_reg_read(ANA_RTC_CTRL);

    reg_read &= (~0xFF);
    reg_read |= 0x95;

    ADI_Analogdie_reg_write(ANA_RTC_CTRL, reg_read);
#endif
}


__inline LOCAL void set_mem_volt(void)
{
#if defined(CONFIG_SC7710G2)

    uint16 reg_read;

    reg_read = ADI_Analogdie_reg_read(ANA_DCDC_MEM_CTL0);

    reg_read &= (~7);
    reg_read |= 0x6; // dcdc mem 1.8V

    ADI_Analogdie_reg_write(ANA_DCDC_MEM_CTL0, reg_read);

    reg_read = ADI_Analogdie_reg_read(ANA_LDO_TRIM9);

    reg_read &= (~0x1F);

    ADI_Analogdie_reg_write(ANA_LDO_TRIM9, reg_read);
#endif
}

#ifndef BIT
#define BIT(x)	(1<<(x))
#endif

#define BITS_DCDC_CAL_RST(_x_)     ( (_x_) << 5 & (BIT(5)|BIT(6)|BIT(7)|BIT(8)|BIT(9)) )
#define BITS_DCDC_CAL(_x_)         ( (_x_) << 0 & (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)) )
#define DCDC_CAL(_x_)	BITS_DCDC_CAL(_x_) | BITS_DCDC_CAL_RST(BITS_DCDC_CAL(-1) - (_x_))

__inline LOCAL void Chip_Workaround(void)
{
#if defined(CONFIG_SC7710G2)
	/*FIXME: adjust vcore/varm, about 3mV one step */
	ANA_REG_SET(ANA_DCDC_CORE_CTRL1, DCDC_CAL(10));
	ANA_REG_SET(ANA_DCDC_ARM_CTL1, DCDC_CAL(10));

	/* FIXME: disable otp for a-die internal bug */
	ANA_REG_OR(ANA_MIXED_CTRL, BIT_1/*BIT_OTP_EN_RST*/);
	ANA_REG_OR(ANA_DCDC_OPT_CTL, BIT_0/*BIT_DCDC_OTP_PD*/);

	/* FIXME: enable dcdc wpa current limit
	 * in order to prevent vbat drop when high load
	 */
	ANA_REG_OR(ANA_WPA_DCDC_AP_CTL2, BIT_6/*BIT_WPA_DCDC_CL_CTRL_AP*/);
#endif
}

static void sc_init_chipid(void)
{
#ifdef CONFIG_SC7710G2
	unsigned int reg_val;

	glb_ana_chipid = (ADI_Analogdie_reg_read(ANA_CHIP_ID_HIGH) << 16) |
					ADI_Analogdie_reg_read(ANA_CHIP_ID_LOW);

	if (glb_ana_chipid == ANA_CHIP_ID_BA) {
		/* enable audif register in A-die APB */
		reg_val = ANA_REG_GET(ANA_AUDIO_CTL);

		ANA_REG_OR(ANA_AUDIO_CTL, BIT_15 | BIT_0);

		if (!(ANA_REG_GET(VOICE_BAND_CODEC_BEGIN + 0xC0) & BIT_6)) {
			glb_ana_chipid = (unsigned int)ANA_CHIP_ID_BB;
		}

		ANA_REG_SET(ANA_AUDIO_CTL, reg_val);
	}
#endif
}


PUBLIC void Chip_Init(void)
{
	Chip_Workaround();

	sc_init_chipid();

    if (CHIP_PHY_GetANAChipID() == ANA_CHIP_ID_AA)
    {
        set_mem_volt();
        set_XOSC32K_config();
    }

    set_chip_clock_freq();

    sdram_init();

    return;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
