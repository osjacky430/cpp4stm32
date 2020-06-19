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

#include <cmath>

#include "cpp_stm32/target/stm32/f4/flash.hxx"
#include "cpp_stm32/target/stm32/f4/pwr.hxx"
#include "cpp_stm32/target/stm32/f4/rcc.hxx"

#include "cpp_stm32/detail/algorithm.hxx"
#include "cpp_stm32/utility/literal_op.hxx"

// user specific include
#include "sys_info.hpp"

namespace cpp_stm32::sys {

/**
 * @defgroup DEVICE_VOLTAGE_DEF   Device voltage
 * @{
 */

#if !defined(DEVICE_VDD)
#define DEVICE_VDD 3.3f
#endif

static constexpr auto STM32_VDD = DEVICE_VDD;

static_assert(1.8f <= STM32_VDD && STM32_VDD <= 3.6f);

/**@}*/

/**
 * @defgroup HSI_CLK_DEF    HSI Clock
 * @{
 */

#if !defined(HSI_CLK_FREQ)
#define HSI_CLK_FREQ (16 MHz)
#endif

static_assert(HSI_CLK_FREQ == 16_MHz);

/**@}*/

/**
 * @defgroup HSE_CLK_DEF    HSE Clock
 * @{
 */

#if !defined(HSE_CLK_FREQ)
#define HSE_CLK_FREQ (8 MHz)
#endif

#if !defined(HSE_BYPASS_CLK_SRC)
#define HSE_BYPASS_CLK_SRC false
#endif

static constexpr auto HSE_CLK_FREQ_MIN = 4_MHz;
static constexpr auto HSE_CLK_FREQ_MAX = 25_MHz;

static_assert(HSE_CLK_FREQ_MIN <= HSE_CLK_FREQ && HSE_CLK_FREQ <= HSE_CLK_FREQ_MAX);

/**@}*/

/**
 * @defgroup PLL_VCO_DEF    PLL VCO
 * @{
 */

static constexpr auto RECOMMEND_VCO_INPUT_FREQ = 2_MHz;
static constexpr auto VCO_OUTPUT_FREQ_MIN			 = 100_MHz;
static constexpr auto VCO_OUTPUT_FREQ_MAX			 = 432_MHz;

/**@}*/

/**
 * @defgroup VOS_MAX_FREQ_DEF    Max HCLK Freq under different VOS with/without overdrive
 * @{
 */

static constexpr auto AHB_VOS_SCALE1_MAX_FREQ							 = 120_MHz;
static constexpr auto AHB_VOS_SCALE2_MAX_FREQ_NO_OVERDRIVE = 144_MHz;
static constexpr auto AHB_VOS_SCALE2_MAX_FREQ_OVERDRIVE		 = 168_MHz;

/**@}*/

static constexpr auto APB1_FREQ_MAX_NO_OVERDRIVE = 42_MHz;
static constexpr auto APB1_FREQ_MAX_OVERDRIVE		 = 45_MHz;
static constexpr auto APB2_FREQ_MAX_NO_OVERDRIVE = 84_MHz;
static constexpr auto APB2_FREQ_MAX_OVERDRIVE		 = 90_MHz;

static_assert(APB1_CLK_FREQ <= APB1_FREQ_MAX_OVERDRIVE);

/**
 * @class   Clock
 * @brief
 */
class Clock {
 private:
	static constexpr std::uint64_t AHB_CLK_FREQ_ = AHB_CLK_FREQ;

	static constexpr auto CALC_ADVANCE_BUS_DIV_FACTOR = []() {
		using rcc::HPRE, rcc::PPRE, rcc::DivisionFactor_v;
		return std::tuple{
			HPRE{DivisionFactor_v<SYS_CLK_FREQ / AHB_CLK_FREQ>},
			PPRE{DivisionFactor_v<AHB_CLK_FREQ / APB1_CLK_FREQ>},
			PPRE{DivisionFactor_v<AHB_CLK_FREQ / APB2_CLK_FREQ>},
		};
	};

	template <rcc::ClkSrc PllSrc>
	static constexpr auto CALC_PLL_DIV_FACTOR() {
		using rcc::ClkSrc, rcc::PllM, rcc::PllN, rcc::PllP, rcc::PllQ, rcc::PllR, rcc::DivisionFactor_v;
		constexpr auto pll_input_clk_freq = (PllSrc == ClkSrc::Hse ? HSE_CLK_FREQ : HSI_CLK_FREQ);
		constexpr auto pllm = static_cast<std::uint32_t>(std::ceil(pll_input_clk_freq / RECOMMEND_VCO_INPUT_FREQ));
		constexpr auto vco_input_freq = pll_input_clk_freq / pllm;
		constexpr auto plln_over_pllp = SYS_CLK_FREQ / vco_input_freq;
		constexpr auto max_plln				= std::floor(VCO_OUTPUT_FREQ_MAX / vco_input_freq);
		constexpr auto min_plln				= std::ceil(VCO_OUTPUT_FREQ_MIN / vco_input_freq);

		constexpr auto pllp_satisfy_constraint = [](auto const& t_pllp_candidate) {
			auto const plln = t_pllp_candidate.first * plln_over_pllp;
			return (min_plln <= plln && plln <= max_plln);
		};

		constexpr auto pllp_iter =
			detail::find_if(PllP::AVAIL_DIVISION_FACTOR.begin(), PllP::AVAIL_DIVISION_FACTOR.end(), pllp_satisfy_constraint);
		constexpr auto pllp = pllp_iter->first;
		constexpr auto plln = static_cast<std::uint32_t>(plln_over_pllp * pllp);

		// if constexpr (need to calculate pllq and pllr) {
		//    ...
		// } else
		return std::tuple{PllM{DivisionFactor_v<pllm>}, PllN{DivisionFactor_v<plln>}, PllP{DivisionFactor_v<pllp>},
											PllQ{DivisionFactor_v<2U>}, PllR{DivisionFactor_v<2U>}};
	}

 public:
	static constexpr auto CPU_WAIT_STATE = flash::WaitTable::getWaitState<STM32_VDD>(AHB_CLK_FREQ_);

	static constexpr auto NEED_OVERDRIVE =
		(APB1_CLK_FREQ >= APB1_FREQ_MAX_NO_OVERDRIVE || APB2_CLK_FREQ >= APB2_FREQ_MAX_NO_OVERDRIVE ||
		 AHB_CLK_FREQ >= AHB_VOS_SCALE2_MAX_FREQ_OVERDRIVE);

	static constexpr auto VOLTAGE_SCALE = []() {
		if constexpr (NEED_OVERDRIVE || AHB_CLK_FREQ >= AHB_VOS_SCALE2_MAX_FREQ_NO_OVERDRIVE) {
			return pwr::VoltageScale::Scale1Mode;
		} else if constexpr (AHB_CLK_FREQ >= AHB_VOS_SCALE1_MAX_FREQ) {
			return pwr::VoltageScale::Scale2Mode;
		} else {
			return pwr::VoltageScale::Scale3Mode;
		}
	}();

	static constexpr auto SYS_CLK_SRC = []() {
		using rcc::SysClk;
		if constexpr (SYS_CLK_FREQ != HSE_CLK_FREQ && SYS_CLK_FREQ != HSI_CLK_FREQ) {
			// maybe its Pllr, @todo add this into consideration
			return SysClk::Pllp;
		} else if constexpr (SYS_CLK_FREQ == HSE_CLK_FREQ) {
			return SysClk::Hse;
		} else if constexpr (SYS_CLK_FREQ == HSI_CLK_FREQ) {
			return SysClk::Hsi;
		}
	}();

	template <rcc::SysClk SystemClock = SYS_CLK_SRC>
	static constexpr void init() noexcept {
		using rcc::ClkSrc;

		if constexpr (HSE_BYPASS_CLK_SRC) {
			rcc::bypass_clksrc<ClkSrc::Hse>();
		}
	}

	// @todo, this is a bit strange
	template <rcc::ClkSrc PllSrc, typename = std::enable_if_t<rcc::is_pll_clk_src<PllSrc>>>
	static constexpr void init() noexcept {
		using rcc::ClkSrc, rcc::PeriphClk;
		if constexpr (HSE_BYPASS_CLK_SRC) {
			rcc::bypass_clksrc<ClkSrc::Hse>();
		}

		// enable PLL Clock Src, i.e., HSE or HSI
		rcc::enable_clk<PllSrc>();
		rcc::wait_osc_rdy<PllSrc>();
		rcc::set_sysclk<rcc::SysClk{to_underlying(PllSrc)}>();

		{	 // operations that requires PLL off
			auto const& [m, n, p, q, r] = CALC_PLL_DIV_FACTOR<PllSrc>();

			rcc::disable_clk<ClkSrc::Pll>();
			rcc::set_pllsrc_and_div_factor<PllSrc>(m, n, p, q, r);
			rcc::enable_periph_clk<PeriphClk::Pwr>();
			pwr::set_voltage_scale(VOLTAGE_SCALE);
		}

		rcc::enable_clk<ClkSrc::Pll>();

		if constexpr (NEED_OVERDRIVE) {
			pwr::enable_overdrive();
			pwr::wait_overdrive_rdy();

			pwr::enable_overdrive_switch();
			pwr::wait_overdrive_switch_rdy();
		}

		constexpr auto wait_state = flash::Latency{flash::CpuWaitState_v<CPU_WAIT_STATE>};
		flash::config_access_ctl<flash::ARTAccel::InstructCache, flash::ARTAccel::DataCache>(wait_state);

		auto const& [ahb, apb1, apb2] = CALC_ADVANCE_BUS_DIV_FACTOR();
		rcc::config_adv_bus_division_factor(ahb, apb1, apb2);

		rcc::wait_osc_rdy<ClkSrc::Pll>();	 // wait for PLL lock

		// switch sysclk and wait ready
		rcc::set_sysclk<SYS_CLK_SRC>();
		rcc::wait_sysclk_rdy<SYS_CLK_SRC>();
	}
};

}	 // namespace cpp_stm32::sys
