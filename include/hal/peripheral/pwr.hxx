/**
 * @Date:   2019-12-23T01:52:18+08:00
 * @Email:  osjacky430@gmail.com
 * @Filename: pwr.hxx
 */

#pragma once

#include "include/hal/peripheral/memory/pwr_reg.hxx"

enum class VoltageScale { Scale1Mode = 0b11, Scale2Mode = 0b10, Scale3Mode = 0b01 };

constexpr auto pwr_is_vos_rdy() noexcept { return get<0>(PWR_CSR.readBit<PwrCsrBit::VosRdy>(ValueOnly)); }

constexpr void pwr_enable_overdrive() noexcept { PWR_CR.setBit<PwrCrBit::Oden>(); }

constexpr auto pwr_is_overdrive_rdy() noexcept { return get<0>(PWR_CSR.readBit<PwrCsrBit::OdrRdy>(ValueOnly)); }

constexpr auto pwr_is_overdrive_switch_rdy() noexcept {
	return get<0>(PWR_CSR.readBit<PwrCsrBit::OdrSwRdy>(ValueOnly));
}

constexpr void pwr_wait_vos_rdy() noexcept {
	while (pwr_is_vos_rdy() != 1) {
	}
}

constexpr void pwr_wait_overdrive_rdy() noexcept {
	while (pwr_is_overdrive_rdy() != 1) {
	}
}

constexpr void pwr_wait_overdrive_switch_rdy() noexcept {
	while (pwr_is_overdrive_switch_rdy() != 1) {
	}
}

constexpr void pwr_set_voltage_scale(VoltageScale const& t_val) noexcept {
	pwr_wait_vos_rdy();
	PWR_CR.setBit<PwrCrBit::Vos>(t_val);
}
