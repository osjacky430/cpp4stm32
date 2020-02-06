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

#include "cpp_stm32/common/dma.hxx"
#include "cpp_stm32/target/stm32/f4/memory/dma_reg.hxx"

namespace cpp_stm32::dma {

/**
 * [clear_interrupt_flag description]
 */
template <Port DMA, Stream Str, InterruptFlag... Flags>
constexpr void clear_interrupt_flag() noexcept {
	if constexpr (to_underlying(Str) <= 3) {
		reg::LIFCR<DMA>.template setBit<InterruptFlag{to_underlying(Flags) * 4U + to_underlying(Str)}...>();
	} else {
		reg::HIFCR<DMA>.template setBit<InterruptFlag{to_underlying(Flags) * 4U + to_underlying(Str) - 4}...>();
	}
}

/**
 * [get_interrupt_flag description]
 * @return [description]
 */
template <Port DMA, Stream Str, InterruptFlag... Flags>
[[nodiscard]] constexpr bool get_interrupt_flag() noexcept {
	if constexpr (to_underlying(Str) <= 3) {
		return get<0>(
			reg::LISR<DMA>.template readBit<InterruptFlag{to_underlying(Flags) * 4U + to_underlying(Str)}...>(ValueOnly));
	} else {
		return get<0>(
			reg::HISR<DMA>.template readBit<InterruptFlag{to_underlying(Flags) * 4U + to_underlying(Str) - 4}...>(ValueOnly));
	}
}

/**
 * [is_enabled description]
 * @return [description]
 */
template <Port DMA, Stream Str>
[[nodiscard]] constexpr bool is_enabled() noexcept {
	return get<0>(reg::SxCR<DMA, Str>.template readBit<reg::SxCRField::EN>(ValueOnly));
}

/**
 * [set_tx_direction description]
 * @param t_dir [description]
 */
template <Port DMA, Stream Str>
constexpr void set_tx_mode(TransferDir const& t_dir) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::DIR>(t_dir);
	}
}

/**
 * [enable_tx_complete_irq description]
 */
template <Port DMA, Stream Str>
constexpr void set_priority(StreamPriority const& t_prior) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::PL>(t_prior);
	}
}

/**
 * [set_memory_size description]
 * @param size [description]
 */
template <Port DMA, Stream Str>
constexpr void set_memory_data_size(DataSize const& t_size) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::MSIZE>(t_size);
	}
}

/**
 * [set_periph_data_size description]
 * @param t_size [description]
 */
template <Port DMA, Stream Str>
constexpr void set_periph_data_size(DataSize const& t_size) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::PSIZE>(t_size);
	}
}

/**
 * [enable_tx_complete_irq description]
 */
template <Port DMA, Stream Str>
constexpr void enable_mem_increment() noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template setBit<reg::SxCRField::MINC>();
	}
}

/**
 * [disable_mem_increment description]
 */
template <Port DMA, Stream Str>
constexpr void disable_mem_increment() noexcept {
	reg::SxCR<DMA, Str>.template clearBit<reg::SxCRField::MINC>();
}

/**
 * [enable_periph_increment description]
 */
template <Port DMA, Stream Str>
constexpr void enable_periph_increment() noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template setBit<reg::SxCRField::PINC>();
	}
}

/**
 * [disable_periph_increment description]
 */
template <Port DMA, Stream Str>
constexpr void disable_periph_increment() noexcept {
	reg::SxCR<DMA, Str>.template clearBit<reg::SxCRField::PINC>();

}	 // namespace cpp_stm32::dma

/**
 * [enable_periph_fix_increment description]
 */
template <Port DMA, Stream Str>
constexpr void enable_periph_fix_increment() noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template setBit<reg::SxCRField::PINC, reg::SxCRField::PINCOS>();
	}
}

/**
 * [enable_circular_mode description]
 */
template <Port DMA, Stream Str>
constexpr void enable_circular_mode() noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template setBit<reg::SxCRField::CIRC>();
	}
}

/**
 * [channel_select description]
 * @param t_ch [description]
 */
template <Port DMA, Stream Str>
constexpr void channel_select(Channel const& t_ch) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::CHSEL>(t_ch);
	}
}

/**
 * [set_memory_burst description]
 * @param t_bs [description]
 */
template <Port DMA, Stream Str>
constexpr void set_memory_burst(BurstSize const& t_bs) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::MBURST>(t_bs);
	}
}

/**
 * [set_memory_burst description]
 * @param t_bs [description]
 */
template <Port DMA, Stream Str>
constexpr void set_periph_burst(BurstSize const& t_bs) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::PBURST>(t_bs);
	}
}

/**
 * [set_current_target description]
 */
template <Port DMA, Stream Str>
constexpr void double_buffer_set_target(std::uint8_t const& t_mem) noexcept {
	if (!is_enabled<DMA, Str>()) {
		reg::SxCR<DMA, Str>.template writeBit<reg::SxCRField::CT>(t_mem);
	}
}

/**
 * [set_mem_addr description]
 * @param t_mem_addr [description]
 */
template <Port DMA, Stream Str>
constexpr void set_address(MemoryAddress_t const& t_mem_addr) noexcept {
	reg::SxM0AR<DMA, Str>.template writeBit<reg::SxM0ARField::M0A>(t_mem_addr.get());
}

/**
 * [set_mem_addr description]
 * @param t_mem_addr [description]
 */
template <Port DMA, Stream Str>
constexpr void set_address(MemoryAddress_t const& t_mem0, MemoryAddress_t const& t_mem1) noexcept {
	reg::SxM0AR<DMA, Str>.template writeBit<reg::SxM0ARField::M0A>(t_mem0.get());
	reg::SxM1AR<DMA, Str>.template writeBit<reg::SxM1ARField::M1A>(t_mem1.get());
}

/**
 * [set_address description]
 * @param t_periph_addr [description]
 */
template <Port DMA, Stream Str>
constexpr void set_address(PeriphAddress_t const& t_periph_addr) noexcept {
	reg::SxPAR<DMA, Str>.template writeBit<reg::SxPARField::PA>(t_periph_addr.get());
}

/**
 * [set_tx_data_num description]
 * @param t_num [description]
 */
template <Port DMA, Stream Str>
constexpr void set_tx_data_num(std::uint16_t const& t_num) noexcept {
	reg::SxNDTR<DMA, Str>.template writeBit<reg::SxNDTRField::NDT>(t_num);
}

/**
 * [get_tx_data_num description]
 * @return [description]
 */
template <Port DMA, Stream Str>
constexpr auto get_tx_data_num() noexcept {
	return reg::SxNDTR<DMA, Str>.template readBit<reg::SxNDTRField::NDT>(ValueOnly);
}

/**
 * @brief   This function clears transfer complete interrupt flag
 * @tparam  Port    ::Port
 * @tparam  Stream  ::Stream
 */
template <Port DMA, Stream Str>
constexpr auto clear_tx_complete_flag = clear_interrupt_flag<DMA, Str, InterruptFlag::TCI>;

/**
 * @brief   This function get transfer complete interrupt flag
 * @tparam  Port    ::Port
 * @tparam  Stream  ::Stream
 */
template <Port DMA, Stream Str>
constexpr auto get_tx_complete_flag = get_interrupt_flag<DMA, Str, InterruptFlag::TCI>;

/**
 * @brief   This function clears half transfer interrupt flag
 * @tparam  Port    ::Port
 * @tparam  Stream  ::Stream
 */
template <Port DMA, Stream Str>
constexpr auto clear_half_tx_flag = clear_interrupt_flag<DMA, Str, InterruptFlag::HTI>;

/**
 * @brief   This function get half transfer interrupt flag
 * @tparam  Port    ::Port
 * @tparam  Stream  ::Stream
 */
template <Port DMA, Stream Str>
constexpr auto get_half_tx_flag = get_interrupt_flag<DMA, Str, InterruptFlag::HTI>;

/**
 * [enable description]
 */
template <Port DMA, Stream Str>
constexpr void disable() noexcept {
	reg::SxCR<DMA, Str>.template clearBit<reg::SxCRField::EN>();
}

/**
 * [enable description]
 */
template <Port DMA, Stream Str>
constexpr void enable() noexcept {
	reg::SxCR<DMA, Str>.template setBit<reg::SxCRField::EN>();
}

/**
 * [reset description]
 */
template <Port DMA, Stream Str>
constexpr void reset() noexcept {
	reg::SxCR<DMA, Str>.template clearBit<reg::SxCRField::EN>();

	while (is_enabled<DMA, Str>()) {
	}

	reg::SxCR<DMA, Str>.reset();
	reg::SxNDTR<DMA, Str>.reset();
	reg::SxPAR<DMA, Str>.reset();
	reg::SxM0AR<DMA, Str>.reset();
	reg::SxM1AR<DMA, Str>.reset();
	reg::SxFCR<DMA, Str>.reset();

	clear_interrupt_flag<DMA, Str, InterruptFlag::FEI, InterruptFlag::DMEI, InterruptFlag::TEI, InterruptFlag::HTI,
											 InterruptFlag::TCI>();
}

/**
 * [DmaBuilder description]
 *
 * @note     Stream configuration procedure
 *           1. disable stream if is enabled, wait until the stream is disabled
 *           2. set peripheral port register address
 *           3. set memory address
 *           4. configure the total number of data items to be transfered
 *           5. select the DMA channel
 *           6. configure flow control if needed
 *           7. configure stream priority
 *           8. configure fifo usage
 *           9. configure data transfer direction, perihperal and memory increment/fix mode,
 *              single or burst transactions, peripheral and memory data widths, circular mode,
 *              double-buffer mode, and interrupts
 *          10. active the stream
 */
template <Port DMA, Stream Str>
class DmaBuilder {
 private:
	Channel m_channelSelect{Channel::Channel0};
	TransferDir m_transferDirection{TransferDir::PeriphToMem};
	StreamPriority m_streamPriority{StreamPriority::Low};

	std::uint8_t m_periphIncrementMode{false};
	std::uint8_t m_periphFixIncr{false};
	DataSize m_peripheralDataSize{DataSize::Byte};
	BurstSize m_peripheralBurstSize{BurstSize::Single};

	std::uint8_t m_memoryIncrementMode{false};
	DataSize m_memoryDataSize{DataSize::Byte};
	BurstSize m_memoryBurstSize{BurstSize::Single};

	std::uint8_t m_flowControl{0};
	std::uint8_t m_circularMode{false};
	std::uint8_t m_doubleBuffer{false};
	std::uint8_t m_currentTarget{0};

	std::uint8_t m_directModeDisabled{false};
	std::uint8_t m_fifoErrorIrq{false};
	FifoThreshold m_fifoThreshold{FifoThreshold::Half};

	std::uint8_t m_directModeDisabledErrorIrq{false};
	std::uint8_t m_transferErrorIrq{false};
	std::uint8_t m_halfTransferIrq{false};
	std::uint8_t m_transferCompleteIrq{false};

	/**
	 * [resetDMA description]
	 * @return [description]
	 */
	static constexpr void resetDMA() noexcept {
		reg::SxCR<DMA, Str>.template clearBit<reg::SxCRField::EN>();

		while (is_enabled<DMA, Str>()) {
		}

		clear_interrupt_flag<DMA, Str, InterruptFlag::FEI, InterruptFlag::DMEI, InterruptFlag::TEI, InterruptFlag::HTI,
												 InterruptFlag::TCI>();
	}

 public:
	constexpr DmaBuilder() noexcept { resetDMA(); }

	[[nodiscard]] constexpr auto transferDir(PeriphAddress_t const& t_from, MemoryAddress_t const& t_to) noexcept {
		m_transferDirection = TransferDir::PeriphToMem;

		reg::SxPAR<DMA, Str>.template writeBit<reg::SxPARField::PA>(t_from.get());
		reg::SxM0AR<DMA, Str>.template writeBit<reg::SxM0ARField::M0A>(t_to.get());

		return *this;
	}

	[[nodiscard]] constexpr auto transferDir(MemoryAddress_t const& t_from, PeriphAddress_t const& t_to) noexcept {
		m_transferDirection = TransferDir::MemToPeriph;

		reg::SxPAR<DMA, Str>.template writeBit<reg::SxPARField::PA>(t_to.get());
		reg::SxM0AR<DMA, Str>.template writeBit<reg::SxM0ARField::M0A>(t_from.get());

		return *this;
	}

	[[nodiscard]] constexpr auto transferDir(MemoryAddress_t const& t_from, MemoryAddress_t const& t_to) noexcept {
		m_transferDirection = TransferDir::MemToMem;

		reg::SxPAR<DMA, Str>.template writeBit<reg::SxPARField::PA>(t_from.get());
		reg::SxM0AR<DMA, Str>.template writeBit<reg::SxM0ARField::M0A>(t_to.get());

		return *this;
	}

	[[nodiscard]] constexpr auto txDataNum(std::uint16_t const& t_ndt) noexcept {
		reg::SxNDTR<DMA, Str>.template writeBit<reg::SxNDTRField::NDT>(t_ndt);
		return *this;
	}

	[[nodiscard]] constexpr auto selectChannel(Channel const& t_ch) noexcept {
		m_channelSelect = t_ch;
		return *this;
	}

	[[nodiscard]] constexpr auto streamPriority(StreamPriority const& t_str) noexcept {
		m_streamPriority = t_str;
		return *this;
	}

	[[nodiscard]] constexpr auto memoryIncrementMode(bool const& t_incr) noexcept {
		m_memoryIncrementMode = t_incr;
		return *this;
	}

	[[nodiscard]] constexpr auto memoryBurstTransfer(BurstSize const& t_bs) noexcept {
		m_memoryBurstSize = t_bs;
		return *this;
	}

	[[nodiscard]] constexpr auto memoryDataWidth(DataSize const& t_ds) noexcept {
		m_memoryDataSize = t_ds;
		return *this;
	}

	[[nodiscard]] constexpr auto peripheralFlowControl(FlowControl const& t_fc) noexcept {
		m_flowControl = to_underlying(t_fc);	// @todo: change underlying type of the register
		return *this;
	}

	[[nodiscard]] constexpr auto peripheralIncrementMode(bool const& t_incr) noexcept {
		m_periphIncrementMode = t_incr;
		return *this;
	}

	[[nodiscard]] constexpr auto perihperalFixIncrement(bool const& t_fix_incr) noexcept {
		m_periphFixIncr = t_fix_incr;
		return *this;
	}

	[[nodiscard]] constexpr auto peripheralBurstTransfer(BurstSize const& t_bs) noexcept {
		m_peripheralBurstSize = t_bs;
		return *this;
	}

	[[nodiscard]] constexpr auto perihperalDataWidth(DataSize const& t_ds) noexcept {
		m_peripheralDataSize = t_ds;
		return *this;
	}

	[[nodiscard]] constexpr auto circularMode(bool const& t_c) noexcept {
		m_circularMode = t_c;
		return *this;
	}

	[[nodiscard]] constexpr auto doubleBufferTarget(std::uint8_t const& t_target) noexcept {
		m_doubleBuffer	= true;
		m_currentTarget = t_target;
		return *this;
	}

	[[nodiscard]] constexpr auto configFIFO(FifoThreshold const& t_fts) noexcept {
		m_directModeDisabled = true;
		m_fifoThreshold			 = t_fts;
		return *this;
	}

	template <InterruptFlag... Flags>
	[[nodiscard]] constexpr auto enableInterrupt() noexcept {
		// @todo find a better way to do this
		m_fifoErrorIrq							 = ((Flags == InterruptFlag::FEI) || ...);
		m_directModeDisabledErrorIrq = ((Flags == InterruptFlag::DMEI) || ...);
		m_transferErrorIrq					 = ((Flags == InterruptFlag::TEI) || ...);
		m_halfTransferIrq						 = ((Flags == InterruptFlag::HTI) || ...);
		m_transferCompleteIrq				 = ((Flags == InterruptFlag::TCI) || ...);

		return *this;
	}

	constexpr void build() noexcept {
		using namespace reg;

		SxFCR<DMA, Str>.template writeBit<SxFCRField::FTH, SxFCRField::DMDIS, SxFCRField::FEIE>(
			m_fifoThreshold, m_directModeDisabled, m_fifoErrorIrq);

		SxCR<DMA, Str>.template writeBit<SxCRField::EN, SxCRField::DMEIE, SxCRField::TEIE, SxCRField::HTIE, SxCRField::TCIE,   //
                                     SxCRField::PFCTRL, SxCRField::DIR, SxCRField::CIRC, SxCRField::PINC, SxCRField::MINC,
                                     SxCRField::PSIZE, SxCRField::MSIZE, SxCRField::PINCOS, SxCRField::PL, SxCRField::DBM,
                                     SxCRField::CT, SxCRField::PBURST, SxCRField::MBURST, SxCRField::CHSEL>(
			std::uint8_t{0}, m_directModeDisabledErrorIrq, m_transferErrorIrq, m_halfTransferIrq, m_transferCompleteIrq,
      m_flowControl, m_transferDirection, m_circularMode, m_periphIncrementMode, m_memoryIncrementMode,
      m_peripheralDataSize, m_memoryDataSize, m_periphFixIncr, m_streamPriority, m_doubleBuffer,
      m_currentTarget, m_peripheralBurstSize, m_memoryBurstSize, m_channelSelect);

		enable<DMA, Str>();
	}
};

template <Port DMA, Stream Str, InterruptFlag Flag>
class DmaStateManager {
 public:
	[[nodiscard]] constexpr DmaStateManager() noexcept {
		while (!get_interrupt_flag<DMA, Str, Flag>()) {
		}
	}

	~DmaStateManager() noexcept { clear_interrupt_flag<DMA, Str, Flag>(); }
};

}	 // namespace cpp_stm32::dma