// Copyright (c) 2020 by osjacky430.
// All Rights Reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the Lesser GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// Lesser GNU General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <array>
#include <utility>

#include "cpp_stm32/target/stm32/f4/memory/memory_map.hxx"
#include "cpp_stm32/utility/bit.hxx"
#include "cpp_stm32/utility/constexpr_algo.hxx"
#include "cpp_stm32/utility/register.hxx"

// temporary
#include "cpp_stm32/target/stm32/common/macro.hxx"

namespace cpp_stm32::stm32::f4 {

static constexpr auto RCC_BASE = memory_at(PeriphAddr::Ahb1Base, 0x3800U);

template <std::uint32_t Val>
static constexpr auto Frequency_t = std::integral_constant<std::uint32_t, Val>;

template <std::uint32_t Val>
static constexpr auto Frequency_v = Frequency_t<Val>{};

template <std::uint32_t Val>
using DivisionFactor_t = std::integral_constant<std::uint32_t, Val>;

template <std::uint32_t Val>
static constexpr auto DivisionFactor_v = DivisionFactor_t<Val>{};

enum class RccOsc : std::uint32_t;

/**
 * @defgroup	RCC_CR_GROUP		RCC Control Register Group
 *
 * @{
 */

SETUP_REGISTER_INFO(RccCrBitInfo, /**/
										Bit<1>{BitPos_t(0)}, Bit<1>{BitPos_t(1)}, Bit<1>{BitPos_t(16)}, Bit<1>{BitPos_t(17)},
										Bit<1>{BitPos_t(18)}, Bit<1>{BitPos_t(19)}, Bit<1>{BitPos_t(24)}, Bit<1>{BitPos_t(25)},
										Bit<1>{BitPos_t(26)}, Bit<1>{BitPos_t(27)}, Bit<1>{BitPos_t(28)}, Bit<1>{BitPos_t(29)})

enum class RccCrBit {
	HsiOn,
	HsiRdy,
	HseOn,
	HseRdy,
	HseByp,
	CssOn,
	PllOn,
	PllRdy,
	PllSaiOn,
	PllSaiRdy,
};

static constexpr Register<RccCrBitInfo, RccCrBit> RCC_CR{RCC_BASE, 0};

/** @}*/

/**
 * @defgroup	RCC_PLLCFGR_GROUP		RCC PLL Configuration Register Group
 * @{
 */

/**
 * @class 	PllM
 * @brief		Utility class used to check whether division factor is reasonable.
 * @note		PllM valid number lies between 2 and 63
 */
SETUP_LOOKUP_TABLE_WITH_BOUND(PllM, 2, 63);

/**
 * @class		PllN
 * @brief		Utility class used to check whether division factor is reasonable.
 * @note		PllN valid number lies between 50 and 432
 */
SETUP_LOOKUP_TABLE_WITH_BOUND(PllN, 50, 432);

/**
 * @class 	PllP
 * @brief		Utility class used to check whether division factor is reasonable.
 * @note 		PllP valid number:
 *
 * 					 DivFactor |   RegVal
 * 				  -----------|-----------
 * 					     2		 |	 0b00
 * 							 4		 |	 0b01
 * 							 6		 |   0b10
 * 							 8		 |	 0b11
 */
SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(PllP, /**/
																		 std::pair{2, 0b00}, std::pair{4, 0b01}, std::pair{6, 0b10}, std::pair{8, 0b11}, );

/**
 * @class 	PllQ
 * @brief		Utility class used to check whether division factor is reasonable.
 * @note		PllQ valid number lies between 2 and 15
 */
SETUP_LOOKUP_TABLE_WITH_BOUND(PllQ, 2, 15);

/**
 * @class 	PllR
 * @brief		Utility class used to check whether division factor is reasonable.
 * @note 		PllR valid number lies between 2 and 7
 */
SETUP_LOOKUP_TABLE_WITH_BOUND(PllR, 2, 7);

SETUP_REGISTER_INFO(RccPllcfgrBitInfo, /**/
										Bit<6, PllM>{BitPos_t{0}}, Bit<9, PllN>{BitPos_t{6}}, Bit<2, PllP>{BitPos_t{16}},
										Bit<1, RccOsc>{BitPos_t{22}}, Bit<4, PllQ>{BitPos_t{24}}, Bit<3, PllR>{BitPos_t{28}})

enum class RccPllCfgBit { PLLM, PLLN, PLLP, PLLSRC, PLLQ, PLLR };

static constexpr Register<RccPllcfgrBitInfo, RccPllCfgBit> RCC_PLLCFGR{RCC_BASE, 0x04U};
/** @}*/

/**
 * @defgroup	RCC_CFG_GROUP		RCC Configuration Group
 *
 * @{
 */

enum class SysClk : std::uint32_t;

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(HPRE, /**/
																		 std::pair{1, 0b0000}, std::pair{2, 0b1000}, std::pair{4, 0b1001},
																		 std::pair{8, 0b1010}, std::pair{16, 0b1011}, std::pair{64, 0b1100},
																		 std::pair{128, 0b1101}, std::pair{256, 0b1110}, std::pair{512, 0b1111});

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(PPRE, /**/
																		 std::pair{1, 0b000}, std::pair{2, 0b100}, std::pair{4, 0b101}, std::pair{8, 0b110},
																		 std::pair{16, 0b111});

SETUP_REGISTER_INFO(RccCfgBitInfo, /**/
										Bit<2, SysClk>{BitPos_t(0)}, Bit<2, SysClk>{BitPos_t(2)}, Bit<4, HPRE>{BitPos_t(4)},
										Bit<3, PPRE>{BitPos_t(10)}, Bit<3, PPRE>{BitPos_t(13)})

enum class RccCfgBit { SW, SWS, HPRE, PPRE1, PPRE2 };

static constexpr Register<RccCfgBitInfo, RccCfgBit> RCC_CFGR{RCC_BASE, 0x08U};
/**@} */

/**
 * @defgroup	RCC_AHB1RST_GROUP		RCC AHB1 Reset Register Group
 * @{
 */

SETUP_REGISTER_INFO(RccAhb1RstInfo, /**/
										Bit<1>{BitPos_t(0)}, Bit<1>{BitPos_t(1)}, Bit<1>{BitPos_t(2)}, Bit<1>{BitPos_t(3)},
										Bit<1>{BitPos_t(4)}, Bit<1>{BitPos_t(5)}, Bit<1>{BitPos_t(6)}, Bit<1>{BitPos_t(7)},
										Bit<1>{BitPos_t(12)}, Bit<1>{BitPos_t(18)}, Bit<1>{BitPos_t(21)}, Bit<1>{BitPos_t(22)},
										Bit<1>{BitPos_t(29)}, Bit<1>{BitPos_t(30)})

enum class RccAhb1RstBit {
	GpioARst,
	GpioBRst,
	GpioCRst,
	GpioDRst,
	GpioERst,
	GpioFRst,
	GpioGRst,
	GpioHRst,
	CrcRst,
	BkpSRAMRst,
	Dma1Rst,
	Dma2Rst,
	OtgHsRst
};

static constexpr Register<RccAhb1RstInfo, RccAhb1RstBit> RCC_AHB1RST{RCC_BASE, 0x10U};
/**@}*/

/**
 * @defgroup	RCC_APB1RST_GROUP		RCC APB1 Reset Register Group
 * @{
 */

SETUP_REGISTER_INFO(RccApb1RstInfo, Binary<>{BitPos_t{17}}, Binary<>{BitPos_t{28}})

enum class RccApb1RstBit { Usart2Rst, PwrRst };

static constexpr Register<RccApb1RstInfo, RccApb1RstBit> RCC_APB1RST{RCC_BASE, 0x20U};

/**@}*/

/**
 * @defgroup RCC_APB2RST_GROUP		RCC APB2 Reset Register Group
 * @{
 */

SETUP_REGISTER_INFO(RccApb2RstInfo, Binary<>{BitPos_t{4}})

enum class RccApb2RstBit { Usart1Rst };

static constexpr Register<RccApb2RstInfo, RccApb2RstBit> RCC_APB2RST{RCC_BASE, 0x24U};

/**@}*/

/**
 * @defgroup	RCC_AHB1ENR_GROUP		RCC AHB1 Enable Register Group
 * @{
 */

SETUP_REGISTER_INFO(RccAhb1EnrInfo, /**/
										Bit<1>{BitPos_t(0)}, Bit<1>{BitPos_t(1)}, Bit<1>{BitPos_t(2)}, Bit<1>{BitPos_t(3)},
										Bit<1>{BitPos_t(4)}, Bit<1>{BitPos_t(5)}, Bit<1>{BitPos_t(6)}, Bit<1>{BitPos_t(7)},
										Bit<1>{BitPos_t(12)}, Bit<1>{BitPos_t(18)}, Bit<1>{BitPos_t(21)}, Bit<1>{BitPos_t(22)},
										Bit<1>{BitPos_t(29)}, Bit<1>{BitPos_t(30)})

enum class RccAhb1EnrBit {
	GpioAEn,
	GpioBEn,
	GpioCEn,
	GpioDEn,
	GpioEEn,
	GpioFEn,
	GpioGEn,
	GpioHEn,
	CrcEn,
	BkpSRAMEn,
	Dma1En,
	Dma2En,
	OtgHsEn,
	OtgHsULPIEn
};

static constexpr Register<RccAhb1EnrInfo, RccAhb1EnrBit> RCC_AHB1ENR{RCC_BASE, 0x30U};

/**@}*/

/**
 * @defgroup	RCC_APB1ENR_GROUP		RCC APB1 Enable Register Group
 * @{
 */

SETUP_REGISTER_INFO(RccApb1EnrInfo, /**/
										Binary<>{BitPos_t{17}}, Binary<>{BitPos_t{28}})

enum class RccApb1EnrBit { Usart2En, PwrEn };

static constexpr Register<RccApb1EnrInfo, RccApb1EnrBit> RCC_APB1ENR{RCC_BASE, 0x40U};

/**@}*/

/**
 * @defgroup RCC_APB2ENR_GROUP		RCC APB2 Enable Register Group
 * @{
 */

SETUP_REGISTER_INFO(RccApb2EnrInfo, Binary<>{BitPos_t{4}})

enum class RccApb2EnrBit { Usart1En };

static constexpr Register<RccApb2EnrInfo, RccApb2EnrBit> RCC_APB2ENR{RCC_BASE, 0x44U};

/**@}*/

/**
 * @defgroup	RCC_BDCR_GROUP		RCC Backup Domain Control Register Group
 *
 * @{
 */

SETUP_REGISTER_INFO(RccBdcrBitInfo, /**/
										Bit<1>{BitPos_t(0)}, Bit<1>{BitPos_t(1)}, Bit<1>{BitPos_t(2)}, Bit<1>{BitPos_t(3)})

enum class RccBdcrBit {
	LSEON,
	LSERDY,
	LseByp,
	LSEMOD,
};

static constexpr Register<RccBdcrBitInfo, RccBdcrBit> RCC_BDCR{RCC_BASE, 0x70U};

/**@}*/

}	 // namespace cpp_stm32::stm32::f4
