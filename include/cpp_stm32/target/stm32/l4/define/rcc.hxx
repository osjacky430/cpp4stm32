#pragma once

#include <array>
#include <cstdint>
#include <utility>

#include "cpp_stm32/detail/algorithm.hxx"
#include "cpp_stm32/utility/literal_op.hxx"
#include "cpp_stm32/utility/macro.hxx"

namespace cpp_stm32::rcc {

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(MsiRange, /**/
																		 std::pair{100_KHz, 0b000}, std::pair{200_KHz, 0b0001}, std::pair{400_KHz, 0b0010},
																		 std::pair{800_KHz, 0b0011}, std::pair{1_MHz, 0b0100}, std::pair{2_MHz, 0b0101},
																		 std::pair{4_MHz, 0b0110}, std::pair{8_MHz, 0b0111}, std::pair{16_MHz, 0b1000},
																		 std::pair{24_MHz, 0b1001}, std::pair{32_MHz, 0b1010}, std::pair{48_MHz, 0b1011});

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(HPRE, /**/
																		 std::pair{1, 0b0000}, std::pair{2, 0b1000}, std::pair{4, 0b1001},
																		 std::pair{8, 0b1010}, std::pair{16, 0b1011}, std::pair{64, 0b1100},
																		 std::pair{128, 0b1101}, std::pair{256, 0b1110}, std::pair{512, 0b1111});

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(PPRE, /**/
																		 std::pair{1, 0b000}, std::pair{2, 0b100}, std::pair{4, 0b101}, std::pair{8, 0b110},
																		 std::pair{16, 0b111});

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(PllM, /**/
																		 std::pair{1, 0b000}, std::pair{2, 0b001}, std::pair{3, 0b010}, std::pair{4, 0b011},
																		 std::pair{5, 0b100}, std::pair{6, 0b101}, std::pair{7, 0b110},
																		 std::pair{8, 0b111});

SETUP_LOOKUP_TABLE_WITH_BOUND(PllN, 8, 86);

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(PllP, std::pair{7, 0}, std::pair{17, 1});

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(PllQ, /**/
																		 std::pair{2, 0b00}, std::pair{4, 0b01}, std::pair{6, 0b10}, std::pair{8, 0b11}, );

SETUP_LOOKUP_TABLE_WITH_KEY_VAL_PAIR(PllR, /**/
																		 std::pair{2, 0b00}, std::pair{4, 0b01}, std::pair{6, 0b10}, std::pair{8, 0b11}, );

/**
 * @enum 		PeriphClk
 * @brief		It is used @ref rcc::enable_periph_clk and @ref rcc::reset_periph_clk
 */
enum class PeriphClk : std::uint32_t {

	GpioA,
	GpioB,
	/*GpioC ~ GpioF*/

	Usart2,
	Pwr,

	Spi1,
	Usart1,
};

/**
 * 	@enum 	SysClk
 * 	@brief	Four different clock sources can be used to drive the system clock:
 * 					- HSI16 (High Speed Internal) 16 MHz RC oscillator clock
 * 					- MSI (Multi Speed Internal) RC oscillator clock
 * 					- HSE (High Speed External) oscillator clock, from 4 to 48 MHz
 * 					- PLL Clock
 */
enum class SysClk : std::uint32_t { Msi, Hsi16, Hse, Pll };

/**
 *  @enum 	ClkSrc
 */
enum class ClkSrc : std::uint32_t { Hsi480, Msi, Hsi160, Hse, Pll, PllSai, Lse, Lsi };

/**
 * @brief		This function transform rcc enum to its equivalent sys enum
 * @param 	t_rcc  see ::ClkSrc
 */
static constexpr auto to_sys_clk = [](ClkSrc t_rcc) {
	switch (t_rcc) {
		case ClkSrc::Hsi160:
			return SysClk::Hsi16;
		case ClkSrc::Msi:
			return SysClk::Msi;
		case ClkSrc::Hse:
			return SysClk::Hse;
		case ClkSrc::Pll:
			return SysClk::Pll;
		default:
			break;
	}
};

/**
 *
 */
template <ClkSrc Clk>
static constexpr bool is_ext_clk = (Clk == ClkSrc::Hse || Clk == ClkSrc::Lse);

template <ClkSrc Clk>
static constexpr bool is_pll_clk_src = (Clk == ClkSrc::Hse || Clk == ClkSrc::Hsi160 || Clk == ClkSrc::Msi);

}	 // namespace cpp_stm32::rcc
