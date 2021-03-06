/*
 * ZETALOG's Personal COPYRIGHT
 *
 * Copyright (c) 2020
 *    ZETALOG - "Lv ZHENG".  All rights reserved.
 *    Author: Lv "Zetalog" Zheng
 *    Internet: zhenglv@hotmail.com
 *
 * This COPYRIGHT used to protect Personal Intelligence Rights.
 * Redistribution and use in source and binary forms with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the Lv "Zetalog" ZHENG.
 * 3. Neither the name of this software nor the names of its developers may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 4. Permission of redistribution and/or reuse of souce code partially only
 *    granted to the developer(s) in the companies ZETALOG worked.
 * 5. Any modification of this software should be published to ZETALOG unless
 *    the above copyright notice is no longer declaimed.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ZETALOG AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE ZETALOG OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)xpcs_phy_gen5_16g.h: Synopsys XAUI phy Gen5 16G interface
 * $Id: xpcs_phy_gen5_16g.h,v 1.0 2020-12-20 19:00:00 zhenglv Exp $
 */

#ifndef __XPCS_PHY_GEN5_16G_H_INCLUDE__
#define __XPCS_PHY_GEN5_16G_H_INCLUDE__

/* VR_XS_PCS_DIG_STS */
#define VR_PCS_PSEQ_ACK_HIGH0		0x0
#define VR_PCS_PSEQ_ACK_LOW0		0x1
#define VR_PCS_PSEQ_ACK_HIGH1		0x2
#define VR_PCS_PSEQ_ACK_LOW1		0x3
#define VR_PCS_PSEQ_POWER_GOOD		0x4
#define VR_PCS_PSEQ_POWER_SAVE		0x5
#define VR_PCS_PSEQ_POWER_DOWN		0x6

#define dw_xpcs_is_power_good()			\
	(VR_PCS_PSEQ_POWER_GOOD ==		\
	 VR_PCS_PSEQ_STATE(dw_xpcs_read(XS_PCS_MMD, VR_XS_PCS_DIG_STS)))

#define dw_xpcs_power_init()			\
	while (!dw_xpcs_is_power_good())

#define VR_XS_PMA_Gen5_12G_16G_TX_GENCTRL0		VR(0x0030)
#define VR_XS_PMA_Gen5_12G_16G_TX_GENCTRL1		VR(0x0031)
#define VR_XS_PMA_Gen5_12G_16G_TX_GENCTRL2		VR(0x0032)
#define VR_XS_PMA_Gen5_12G_16G_TX_BOOST_CTRL		VR(0x0033)
#define VR_XS_PMA_Gen5_12G_16G_TX_RATE_CTRL		VR(0x0034)
#define VR_XS_PMA_Gen5_12G_16G_TX_POWER_STATE_CTRL	VR(0x0035)
#define VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL0		VR(0x0036)
#define VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL1		VR(0x0037)
#define VR_XS_PMA_Gen5_12G_16G_TX_STS			VR(0x0040)
#define VR_XS_PMA_Gen5_12G_16G_RX_GENCTRL0		VR(0x0050)
#define VR_XS_PMA_Gen5_12G_16G_RX_GENCTRL1		VR(0x0051)
#define VR_XS_PMA_Gen5_12G_16G_RX_GENCTRL2		VR(0x0052)
#define VR_XS_PMA_Gen5_12G_16G_RX_GENCTRL3		VR(0x0053)
#define VR_XS_PMA_Gen5_12G_16G_RX_RATE_CTRL		VR(0x0054)
#define VR_XS_PMA_Gen5_12G_16G_RX_POWER_STATE_CTRL	VR(0x0055)
#define VR_XS_PMA_Gen5_12G_16G_RX_CDR_CTRL		VR(0x0056)
#define VR_XS_PMA_Gen5_12G_16G_RX_ATTN_CTRL		VR(0x0057)
#define VR_XS_PMA_Gen5_12G_16G_RX_EQ_CTRL4		VR(0x005C)
#define VR_XS_PMA_Gen5_12G_16G_DFE_TAP_CTRL0		VR(0x005E)
#define VR_XS_PMA_Gen5_12G_16G_RX_STS			VR(0x0060)
#define VR_XS_PMA_Gen5_12G_16G_MPLL_CMN_CTRL		VR(0x0070)
#define VR_XS_PMA_Gen5_12G_16G_MPLLA_CTRL0		VR(0x0071)
#define VR_XS_PMA_Gen5_12G_16G_MPLLA_CTRL2		VR(0x0073)
#define VR_XS_PMA_Gen5_12G_16G_MPLLB_CTRL0		VR(0x0074)
#define VR_XS_PMA_Gen5_12G_16G_MPLLB_CTRL2		VR(0x0076)
#define VR_XS_PMA_Gen5_12G_16G_MISC_CTRL0		VR(0x0090)
#define VR_XS_PMA_Gen5_12G_16G_REF_CLK_CTRL		VR(0x0091)
#define VR_XS_PMA_Gen5_12G_16G_VCO_CAL_LD0		VR(0x0092)
#define VR_XS_PMA_Gen5_12G_16G_MISC_STS			VR(0x0098)
#define VR_XS_PMA_Gen5_12G_16G_MISC_CTRL1		VR(0x0099)
#define VR_XS_PMA_Gen5_12G_16G_SRAM			VR(0x009B)

/* VR_XS_PMA_Gen5_12G_16G_SRAM */
#define VR_PMA_EXT_LD_DN				_BV(1)
#define VR_PMA_INIT_DN					_BV(0)

#define dw_xpcs_is_int_sram_done()					\
	(dw_xpcs_read(XS_PMA_MMD, VR_XS_PMA_Gen5_12G_16G_SRAM) &	\
	 VR_PMA_INIT_DN)
#define dw_xpcs_int_sram_init()						\
	do {								\
		while (!dw_xpcs_is_int_sram_done());			\
	} while (0)
#define dw_xpcs_ext_sram_init()						\
	dw_xpcs_write(XS_PMA_MMD, VR_XS_PMA_Gen5_12G_16G_SRAM,		\
		      VR_PMA_EXT_LD_DN)

/* Optional registers */
#ifdef CONFIG_DW_XPCS_KR
#define VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL2		VR(0x0038)
#define VR_XS_PMA_Gen5_12G_16G_TX_EQ_CTRL3		VR(0x0039)
#define VR_XS_PMA_Gen5_12G_16G_EQ_INIT_CTRL0		VR(0x003A)
#define VR_XS_PMA_Gen5_12G_16G_EQ_INIT_CTRL1		VR(0x003B)
#endif /* CONFIG_DW_XPCS_KR */
#ifdef CONFIG_DW_XPCS_EEE
#define VR_XS_PMA_Gen5_12G_16G_EEE_CTRL			VR(0x009A)
#endif /* CONFIG_DW_XPCS_EEE */
#ifndef CONFIG_DW_XPCS_SINGLE_LANE
#define VR_XS_PMA_Gen5_12G_16G_DFE_TAP_CTRL1		VR(0x005F)
#define VR_XS_PMA_Gen5_12G_16G_VCO_CAL_LD1		VR(0x0093)
#define VR_XS_PMA_Gen5_12G_16G_VCO_CAL_LD2		VR(0x0094)
#define VR_XS_PMA_Gen5_12G_16G_VCO_CAL_LD3		VR(0x0095)
#endif /* CONFIG_DW_XPCS_SINGLE_LANE */

#define VR_XS_PMA_Gen5_16G_TX_GEN_CTRL3			VR(0x003C)
#define VR_XS_PMA_Gen5_16G_TX_GEN_CTRL4			VR(0x003D)
#define VR_XS_PMA_Gen5_16G_TX_MISC_CTRL0		VR(0x003E)
#define VR_XS_PMA_Gen5_16G_RX_EQ_CTRL0			VR(0x0058)
#define VR_XS_PMA_Gen5_16G_RX_EQ_CTRL5			VR(0x005D)
#define VR_XS_PMA_Gen5_16G_RX_PPM_STS0			VR(0x0061)
#define VR_XS_PMA_Gen5_16G_RX_CDR_CTRL1			VR(0x0064)
#define VR_XS_PMA_Gen5_16G_RX_PPM_CTRL0			VR(0x0065)
#define VR_XS_PMA_Gen5_16G_RX_GEN_CTRL4			VR(0x0068)
#define VR_XS_PMA_Gen5_16G_RX_MISC_CTRL0		VR(0x0069)
#define VR_XS_PMA_Gen5_16G_RX_IQ_CTRL0			VR(0x006B)
#define VR_XS_PMA_Gen5_16G_MPLLA_CTRL1			VR(0x0072)
#define VR_XS_PMA_Gen5_16G_MPLLB_CTRL1			VR(0x0075)
#define VR_XS_PMA_Gen5_16G_MPLLA_CTRL3			VR(0x0077)
#define VR_XS_PMA_Gen5_16G_MPLLB_CTRL3			VR(0x0078)
#define VR_XS_PMA_Gen5_16G_MPLLA_CTRL4			VR(0x0079)
#define VR_XS_PMA_Gen5_16G_MPLLA_CTRL5			VR(0x007A)
#define VR_XS_PMA_Gen5_16G_MPLLB_CTRL4			VR(0x007B)
#define VR_XS_PMA_Gen5_16G_MPLLB_CTRL5			VR(0x007C)
#define VR_XS_PMA_Gen5_16G_VCO_CAL_REF0			VR(0x0096)
#define VR_XS_PMA_Gen5_16G_MISC_CTRL2			VR(0x009C)

/* Optional registers */
#ifndef CONFIG_DW_XPCS_SINGLE_LANE
#define VR_XS_PMA_Gen5_16G_TX_MISC_CTRL1		VR(0x003F)
#define VR_XS_PMA_Gen5_16G_RX_EQ_CTRL1			VR(0x0059)
#define VR_XS_PMA_Gen5_16G_RX_EQ_CTRL2			VR(0x005A)
#define VR_XS_PMA_Gen5_16G_RX_EQ_CTRL3			VR(0x005B)
#define VR_XS_PMA_Gen5_16G_RX_PPM_STS1			VR(0x0062)
#define VR_XS_PMA_Gen5_16G_RX_PPM_CTRL1			VR(0x0066)
#define VR_XS_PMA_Gen5_16G_RX_MISC_CTRL1		VR(0x006A)
#define VR_XS_PMA_Gen5_16G_RX_IQ_CTRL1			VR(0x006C)
#define VR_XS_PMA_Gen5_16G_RX_IQ_CTRL2			VR(0x006D)
#define VR_XS_PMA_Gen5_16G_RX_IQ_CTRL3			VR(0x006E)
#define VR_XS_PMA_Gen5_16G_VCO_CAL_REF1			VR(0x0097)
#endif /* CONFIG_DW_XPCS_SINGLE_LANE */

#ifdef CONFIG_ARCH_IS_DW_XPCS_1000BASE_X
#define VR_MII_Gen5_12G_16G_SRAM			VR(0x80BB)
#endif /* CONFIG_ARCH_IS_DW_XPCS_1000BASE_X */

#ifdef CONFIG_DW_XPCS_VS_MII_MMD
#ifdef CONFIG_ARCH_IS_DW_XPCS_1000BASE_X
#define VR_MII_Gen5_12G_16G_TX_GENCTRL0		VR(0x0030)
#define VR_MII_Gen5_12G_16G_TX_GENCTRL1		VR(0x0031)
#define VR_MII_Gen5_12G_16G_TX_GENCTRL2		VR(0x0032)
#define VR_MII_Gen5_12G_16G_TX_BOOST_CTRL	VR(0x0033)
#define VR_MII_Gen5_12G_16G_TX_RATE_CTRL	VR(0x0034)
#define VR_MII_Gen5_12G_16G_TX_POWER_STATE_CTRL	VR(0x0035)
#define VR_MII_Gen5_12G_16G_TX_EQ_CTRL0		VR(0x0036)
#define VR_MII_Gen5_12G_16G_TX_EQ_CTRL1		VR(0x0037)
#define VR_MII_Gen5_12G_16G_TX_STS		VR(0x0040)
#define VR_MII_Gen5_12G_16G_RX_GENCTRL0		VR(0x0050)
#define VR_MII_Gen5_12G_16G_RX_GENCTRL1		VR(0x0051)
#define VR_MII_Gen5_12G_16G_RX_GENCTRL2		VR(0x0052)
#define VR_MII_Gen5_12G_16G_RX_GENCTRL3		VR(0x0053)
#define VR_MII_Gen5_12G_16G_RX_RATE_CTRL	VR(0x0054)
#define VR_MII_Gen5_12G_16G_RX_POWER_STATE_CTRL	VR(0x0055)
#define VR_MII_Gen5_12G_16G_RX_CDR_CTRL		VR(0x0056)
#define VR_MII_Gen5_12G_16G_RX_ATTN_CTRL	VR(0x0057)
#define VR_MII_Gen5_12G_16G_RX_EQ_CTRL4		VR(0x005C)
#define VR_MII_Gen5_12G_16G_MPLL_CMN_CTRL	VR(0x0070)
#define VR_MII_Gen5_12G_16G_MPLLA_CTRL0		VR(0x0071)
#define VR_MII_Gen5_12G_16G_MPLLA_CTRL2		VR(0x0073)
#define VR_MII_Gen5_12G_16G_MPLLB_CTRL0		VR(0x0074)
#define VR_MII_Gen5_12G_16G_MPLLB_CTRL2		VR(0x0076)
#define VR_MII_Gen5_12G_16G_MISC_CTRL0		VR(0x0090)
#define VR_MII_Gen5_12G_16G_REF_CLK_CTRL	VR(0x0091)
#define VR_MII_Gen5_12G_16G_VCO_CAL_LD0		VR(0x0092)
#define VR_MII_Gen5_12G_16G_MISC_STS		VR(0x0098)
#define VR_MII_Gen5_12G_16G_MISC_CTRL1		VR(0x0099)
#define VR_MII_Gen5_12G_16G_EEE_CTRL		VR(0x009A)

#define VR_MII_Gen5_16G_TX_GEN_CTRL3		VR(0x003C)
#define VR_MII_Gen5_16G_TX_GEN_CTRL4		VR(0x003D)
#define VR_MII_Gen5_16G_TX_MISC_CTRL0		VR(0x003E)
#define VR_MII_Gen5_16G_RX_EQ_CTRL0		VR(0x0058)
#define VR_MII_Gen5_16G_RX_EQ_CTRL5		VR(0x005D)
#define VR_MII_Gen5_16G_RX_PPM_STS0		VR(0x0061)
#define VR_MII_Gen5_16G_RX_CDR_CTRL1		VR(0x0064)
#define VR_MII_Gen5_16G_RX_PPM_CTRL0		VR(0x0065)
#define VR_MII_Gen5_16G_RX_GEN_CTRL4		VR(0x0068)
#define VR_MII_Gen5_16G_RX_MISC_CTRL0		VR(0x0069)
#define VR_MII_Gen5_16G_RX_IQ_CTRL0		VR(0x006B)
#define VR_MII_Gen5_16G_MPLLA_CTRL1		VR(0x0072)
#define VR_MII_Gen5_16G_MPLLB_CTRL1		VR(0x0075)
#define VR_MII_Gen5_16G_MPLLA_CTRL3		VR(0x0077)
#define VR_MII_Gen5_16G_MPLLB_CTRL3		VR(0x0078)
#define VR_MII_Gen5_16G_MPLLA_CTRL4		VR(0x0079)
#define VR_MII_Gen5_16G_MPLLA_CTRL5		VR(0x007A)
#define VR_MII_Gen5_16G_MPLLB_CTRL4		VR(0x007B)
#define VR_MII_Gen5_16G_MPLLB_CTRL5		VR(0x007C)
#define VR_MII_Gen5_16G_VCO_CAL_REF0		VR(0x0096)
#define VR_MII_Gen5_16G_MISC_CTRL2		VR(0x009C)
#endif /* CONFIG_ARCH_IS_DW_XPCS_1000BASE_X */
#endif /* CONFIG_DW_XPCS_VS_MII_MMD */

#endif /* __XPCS_PHY_GEN5_16G_H_INCLUDE__ */
