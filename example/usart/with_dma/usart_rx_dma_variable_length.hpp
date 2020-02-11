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

#include <algorithm>
#include <array>

#include "sys_init.hxx"

#include "cpp_stm32/driver/digitalout.hxx"
#include "cpp_stm32/driver/usart_serial.hxx"

#include "cpp_stm32/target/stm32/f4/dma.hxx"

namespace Driver = cpp_stm32::driver;
namespace Gpio	 = cpp_stm32::gpio;
namespace Usart	 = cpp_stm32::usart;
namespace Rcc		 = cpp_stm32::rcc;
namespace Dma		 = cpp_stm32::dma;
namespace Nvic	 = cpp_stm32::nvic;
namespace Sys		 = cpp_stm32::sys;

using Usart::operator"" _Baud;

/**/
constexpr auto PACKET_LEN_IDX = 3;

constexpr auto PACKET_HEADER_NUM = 2;
constexpr auto PACKET_ID_NUM		 = 1;
constexpr auto PACKET_LEN_NUM		 = 1;

/**/
enum class ReceiverState { TransferComplete, HalfTransfer, Idle };

/* Global DMA define */
constexpr auto DMA = Dma::Port::DMA1;
constexpr auto Str = Dma::Stream::Stream5;

/* Global Storage for DMA destination */
static constexpr std::size_t MAX_BUFFER_SIZE = 20;
std::array<char, MAX_BUFFER_SIZE> buffer;

/* Complete message sent from PC */
static constexpr std::size_t MAX_MESSAGE_SIZE = 10;
char processed_data[MAX_MESSAGE_SIZE];

/* Input from PC */
Driver::Usart const pc{Driver::UsartTx_v<Gpio::PinName::PA_2>, Driver::UsartRx_v<Gpio::PinName::PA_3>, 115200_Baud};

/* For indication, whether interrupt is entered or not */
Driver::DigitalOut<Gpio::PinName::PA_5> led;

constexpr void setup_dma() noexcept {
	constexpr auto p_addr = Usart::reg::DR<Usart::Port::Usart2>.memoryAddr();
	constexpr auto m_addr = &buffer[0];

	Rcc::enable_periph_clk<Rcc::PeriphClk::Dma1>();
	Nvic::enable_irq<cpp_stm32::IrqNum::Dma1Stream5Global>();

	Dma::reset<DMA, Str>();
	Dma::set_tx_mode<DMA, Str>(Dma::TransferDir::PeriphToMem);
	Dma::set_tx_data_num<DMA, Str>(5);
	Dma::channel_select<DMA, Str>(Dma::Channel::Channel4);
	Dma::set_memory_data_size<DMA, Str>(Dma::DataSize::Byte);
	Dma::set_periph_data_size<DMA, Str>(Dma::DataSize::Byte);
	Dma::enable_mem_increment<DMA, Str>();
	Dma::enable_circular_mode<DMA, Str>();
	Dma::set_address<DMA, Str>(Dma::MemoryAddress_t{(std::uintptr_t)m_addr});
	Dma::set_address<DMA, Str>(Dma::PeriphAddress_t{p_addr});
	Dma::set_priority<DMA, Str>(Dma::StreamPriority::Low);
	Dma::enable_irq<DMA, Str, Dma::InterruptFlag::TCI, Dma::InterruptFlag::HTI>();
	Dma::enable<DMA, Str>();

	Usart::enable_idle_irq<Usart::Port::Usart2>();
	Usart::enable_rx_dma<Usart::Port::Usart2>();
}

int main() {
	Sys::Clock::init<Rcc::ClkSrc::Hse>();

	setup_dma();

	while (true) {
		constexpr auto SOME_INTERVAL = 10000000;
		for (int i = 0; i < SOME_INTERVAL; ++i) {
			__asm("nop");
		}

		pc << processed_data << "\n\r";
	}

	return 0;
}

static std::size_t old_pos{0}, last_processed_idx{0};

template <auto State>
constexpr void process_buffer() noexcept {
	auto const current_pos = MAX_BUFFER_SIZE - Dma::get_tx_data_num<DMA, Str>();
	auto const pos_diff		 = current_pos - old_pos;

	if (current_pos != old_pos) {
		if (pos_diff > 0) {
			std::copy(&buffer[old_pos], &buffer[current_pos], &processed_data[last_processed_idx]);

		} else {
			std::copy(&buffer[old_pos], &buffer[MAX_BUFFER_SIZE - old_pos], processed_data);

			if (current_pos > 0) {
				std::copy(&buffer[0], &buffer[current_pos], processed_data);
			}
		}
	}

	if constexpr (State == ReceiverState::TransferComplete) {
		if (pos_diff > PACKET_LEN_IDX) {
			int const total_packet		= processed_data[PACKET_LEN_IDX] + 2;
			int const packet_received = pos_diff - PACKET_HEADER_NUM - PACKET_ID_NUM - PACKET_LEN_NUM;

			if (packet_received < total_packet) {
				// packet transfer isn't finish, but reach to the end of the buffer
				last_processed_idx = pos_diff;
			} else {
				// packet transfer finished, and also reach to the end of the buffer
				last_processed_idx = 0;
			}
		} else {
			// the length of the packet is still unknown, but we have already reach to the end of the buffer
			last_processed_idx = pos_diff;
		}

		old_pos = 0;
	} else if constexpr (State == ReceiverState::Idle) {
		// transfer maybe aborted, or transfer finished, reset processed_idx
		last_processed_idx = 0;
		old_pos						 = current_pos;
	} else if constexpr (State == ReceiverState::HalfTransfer) {
		last_processed_idx = pos_diff;
		old_pos						 = current_pos;
	}
}

void cpp_stm32::interrupt::dma1_stream5() noexcept {
	if (Dma::get_tx_complete_flag<DMA, Str>()) {
		Dma::clear_tx_complete_flag<DMA, Str>();

		process_buffer<ReceiverState::TransferComplete>();
	}

	if (Dma::get_half_tx_flag<DMA, Str>()) {
		Dma::clear_half_tx_flag<DMA, Str>();

		process_buffer<ReceiverState::HalfTransfer>();
	}
}

void cpp_stm32::interrupt::usart2() noexcept {
	if (Usart::get_interrupt_flag<Usart::Port::Usart2, Usart::InterruptFlag::IDLE>()) {
		[[gnu::unused]] auto const clear_idle = Usart::receive<Usart::Port::Usart2>();
		process_buffer<ReceiverState::Idle>();
	}
}
