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

#include "cpp_stm32/common/usart_common.hxx"

#include "cpp_stm32/target/stm32/l4/memory/memory_map.hxx"
#include "cpp_stm32/hal/bit.hxx"
#include "cpp_stm32/hal/register.hxx"

namespace cpp_stm32::stm32::l4 {

enum class UsartNum { Usart1, Usart2 };

static constexpr auto USART_BASE(UsartNum const& t_usart_num) {
	switch (t_usart_num) {
		case UsartNum::Usart1:
			return memory_at(PeriphAddr::Apb2Base, 0x3800U);
		case UsartNum::Usart2:
			return memory_at(PeriphAddr::Apb1Base, 0x4400U);
	}
}

/**
 * @degroup USART_CR1_GROUP		USART Control Register 1 Group
 * @{
 */

SETUP_REGISTER_INFO(UsartCr1Info,																 /************ Description ************/
										Binary<>{BitPos_t{0}},											 // USART Enable
										Binary<>{BitPos_t{1}},											 // USART Enable in stop mode
										Binary<>{BitPos_t{2}},											 // Receiver Enable
										Binary<>{BitPos_t{3}},											 // Transmitter Enable
										Binary<>{BitPos_t{4}},											 // Idle Interrupt Enable
										Binary<>{BitPos_t{5}},											 // Receiver Not Empty Interrupt Enable
										Binary<>{BitPos_t{6}},											 // Transmission Complete Interrupt Enable
										Binary<>{BitPos_t{7}},											 // Transmitter Empty Interrupt Enable
										Binary<>{BitPos_t{8}},											 // Packet Error Interrupt Enable
										Bit<1, usart::Parity>{BitPos_t{9}},		 // Parity Selection
										Binary<>{BitPos_t{10}},											 // Parity Control Enable
										Binary<>{BitPos_t{11}},											 // Receiver Wakeup Mode
										Binary<>{BitPos_t{12}},											 // Word Length (bit 0)
										Binary<>{BitPos_t{13}},											 // Mute Mode Enable
										Binary<>{BitPos_t{14}},											 // Character match interrupt enable
										Bit<1, usart::OverSampling>{BitPos_t{15}},	 // OverSampling
										Bit<5>{BitPos_t{16}},												 // Driver Enable De-assertion time
										Bit<5>{BitPos_t{21}},												 // Driver Enable assertion time
										Binary<>{BitPos_t{26}},											 // Receiver Timeout Interrupt Enable
										Binary<>{BitPos_t{27}},											 // End of Block Interrupt Enable
										Binary<>{BitPos_t{28}}											 // Word Length (bit 1)
)

enum class UsartCr1Bit {
	UE,
	UESM,
	RE,
	TE,
	IdleIE,
	RxNEIE,
	TCIE,
	TxEIE,
	PEIE,
	PS,
	PCE,
	Wake,
	M0,
	MME,
	CMIE,
	Over8,
	DEDT,
	DEAT,
	RToIE,
	EoBIE,
	M1
};

template <UsartNum Port>
static constexpr Register<UsartCr1Info, UsartCr1Bit> USART_CR1{USART_BASE(Port), 0x00U};

/**@}*/

/**
 * @defgroup USART_CR2_GROUP		USART Control Register 2 Group
 * @{
 */

SETUP_REGISTER_INFO(UsartCr2Info,																 /************ Description ************/
										Binary<>{BitPos_t{4}},											 // Address Detectio/4-bit Address Detection
										Binary<>{BitPos_t{5}},											 // Line Break Detection Length
										Binary<>{BitPos_t{6}},											 // Line Break Detection Interrupt Enable
										Binary<>{BitPos_t{8}},											 // Last Bit Clock Pause
										Binary<>{BitPos_t{9}},											 // Clock Phase
										Binary<>{BitPos_t{10}},											 // Clock Polarity
										Binary<>{BitPos_t{11}},											 // Clock Enable
										Bit<2, usart::Stopbit>{BitPos_t{12}},	 // Stop Bit
										Binary<>{BitPos_t{14}},											 // LIN Mode Enable
										Binary<>{BitPos_t{15}},											 // Swap TX/RX Pin
										Binary<>{BitPos_t{16}},											 // RX pin active level Inversion
										Binary<>{BitPos_t{17}},											 // TX pin active level Inversion
										Binary<>{BitPos_t{18}},											 // Binary Data Inversion
										Binary<>{BitPos_t{19}},											 // Most Significant Bit first
										Binary<>{BitPos_t{20}},											 // Auto Baudrate Enable
										Bit<2>{BitPos_t{21}},												 // Auto Baudrate Mode
										Binary<>{BitPos_t{23}},											 // Receiver Timeout Enable
										Bit<8>{BitPos_t{24}}												 // Address of USART node
)

enum class UsartCr2Bit {
	ADDM7,
	LBDL,
	LBDIE,
	LbCl,
	CPha,
	CPol,
	ClkEn,
	Stop,
	LINEn,
	Swap,
	RxInv,
	TxInv,
	DataInv,
	MsbFirst,
	ABREn,
	RTOEn,
	ADD
};

template <UsartNum Port>
static constexpr Register<UsartCr2Info, UsartCr2Bit> USART_CR2{USART_BASE(Port), 0x04U};

/**@}*/

/**
 * @defgroup USART_CR3_GROUP		USART Control Register 3 Group
 * @{
 */

SETUP_REGISTER_INFO(UsartCr3Info,						 /************ Description ************/
										Binary<>{BitPos_t{0}},	 // Error Interrupt Enable
										Binary<>{BitPos_t{1}},	 // IrDA mode Enable
										Binary<>{BitPos_t{2}},	 // IrDA Low Power
										Binary<>{BitPos_t{3}},	 // Half Duplex Selection
										Binary<>{BitPos_t{4}},	 // Smartcard NACK enable
										Binary<>{BitPos_t{5}},	 // Smarcard mode Enable
										Binary<>{BitPos_t{6}},	 // DMA Enable Receiver
										Binary<>{BitPos_t{7}},	 // DMA Enable Transmitter
										Binary<>{BitPos_t{8}},	 // RTS Enable
										Binary<>{BitPos_t{9}},	 // CTS Enable
										Binary<>{BitPos_t{10}},	 // CTS Interrupt Enable
										Binary<>{BitPos_t{11}},	 // One sample bit method enable
										Binary<>{BitPos_t{12}},	 // Overrun Disable
										Binary<>{BitPos_t{13}},	 // DMA Disable on Reception Error
										Binary<>{BitPos_t{14}},	 // Driver Enable Mode
										Binary<>{BitPos_t{15}},	 // Driver Enable Polarity selection
										Bit<3>{BitPos_t{17}},		 // Smartcard auto-retry count
										Bit<2>{BitPos_t{20}},		 // Wakeup from Stop mode interrupt flag selection
										Binary<>{BitPos_t{22}},	 // Wakeup from Stop mode interrupt Enable
										Binary<>{BitPos_t{23}},	 // USART Clock Enable in Stop mode
										Binary<>{BitPos_t{24}}	 // Transmission Complete Before Guard Time Interrupt Enable
)

enum class UsartCr3Bit {
	EIE,
	IrEn,
	IrLP,
	HDSel,
	NAck,
	SCEN,
	DMAR,
	DMAT,
	RTSE,
	CTSE,
	CTSIE,
	OneBit,
	OvrDis,
	DDRE,
	DEM,
	DEP,
	SCARCnt,
	WuS,
	WuSIE,
	UCESm,
	TCBGTIE
};

template <UsartNum Port>
static constexpr Register<UsartCr3Info, UsartCr3Bit> USART_CR3{USART_BASE(Port), 0x08U};

/**@}*/

/**
 * @degroup USART_BRR_GROUP		Usart Baud Rate Register Group
 * @{
 */

SETUP_REGISTER_INFO(UsartBrrInfo,												 /************ Description ************/
										Bit<16, std::uint16_t>{BitPos_t{0}}	 // Baud Rate Register
)

enum class UsartBrrBit { Brr };

template <UsartNum Port>
static constexpr Register<UsartBrrInfo, UsartBrrBit> USART_BRR{USART_BASE(Port), 0x0CU};
/**@}*/

/**
 * @defgroup USART_ISR_GROUP		USART Interrupt and Status Register Group
 * @{
 */

SETUP_REGISTER_INFO(UsartISrInfo,								/************ Description ************/
										StatusBit<1>{BitPos_t{0}},	// Parity Error
										StatusBit<1>{BitPos_t{1}},	// Framing Error
										StatusBit<1>{BitPos_t{2}},	// start bit Noise detection Flag
										StatusBit<1>{BitPos_t{3}},	// Overrun Error
										StatusBit<1>{BitPos_t{4}},	// Idle line detected
										Binary<>{BitPos_t{5}},			// Read data register not empty
										Binary<>{BitPos_t{6}},			// Transmission complete
										StatusBit<1>{BitPos_t{7}},	// Transmit Data Register empty
										Binary<>{BitPos_t{8}},			// LIN Break detection flag
										Binary<>{BitPos_t{9}},			// CTS Interrupt flag
										Binary<>{BitPos_t{10}},			// CTS flag
										Binary<>{BitPos_t{11}},			// Receiver Timeout Flag
										Binary<>{BitPos_t{12}},			// End Of Block Flag
										Binary<>{BitPos_t{14}},			// Auto Baud Rate Error
										Binary<>{BitPos_t{15}},			// Auto Baud Rate Flag
										Binary<>{BitPos_t{16}},			// Busy flag
										Binary<>{BitPos_t{17}},			// Character Match Flag
										Binary<>{BitPos_t{18}},			// Send Break Flag
										Binary<>{BitPos_t{19}},			// Receiver WakeUp from mute mode
										Binary<>{BitPos_t{20}},			// WakeUp from stop mode Flag
										Binary<>{BitPos_t{21}},			// Transmit Enable ACKnowledge flag
										Binary<>{BitPos_t{22}},			// Receive Enable ACKnowledge flag
										Binary<>{BitPos_t{25}}			// Transmission Complete Before Guard Time completion
)

enum class UsartISrBit {
	PE,
	FE,
	NF,
	OrE,
	Idle,
	RxNE,
	TC,
	TxE,
	LBD,
	CTSIF,
	CTS,
	RTOF,
	EoBF,
	ABRE,
	ABRF,
	Busy,
	CMF,
	SBKF,
	RWu,
	WuF,
	TEAck,
	REAck,
	TCBGT
};

template <UsartNum Port>
static constexpr Register<UsartISrInfo, UsartISrBit> USART_ISR{USART_BASE(Port), 0x1CU};
/**@}*/

/**
 * @defgroup USART_ICR_GROUP		USART Interrupt Clear Register Group
 * @{
 */

SETUP_REGISTER_INFO(UsartIcrInfo,														 /************ Description ************/
										Binary<BitMod::RdClrWr1>{BitPos_t{0}},	 // Parity Error Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{1}},	 // Framing Error Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{2}},	 // Noise detected Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{3}},	 // OverRun Error Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{4}},	 // IDLE line detected Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{6}},	 // Transmission Complete Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{7}},	 // Transmission Complete Before Guard Time Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{8}},	 // LIN Break Detection Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{9}},	 // CTS Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{11}},	 // Receiver TimeOut Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{12}},	 // End Of Block Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{17}},	 // Character Match Clear Flag
										Binary<BitMod::RdClrWr1>{BitPos_t{20}}	 // WakeUp from stop mode Clear Flag
)

enum class UsartIcrBit { PECF, FECF, NCF, ORECF, IdleCF, TCCF, TCBGTCF, LBDCF, CTSCF, RTOCF, EOBCF, CMCF, WUCF };

template <UsartNum Port>
static constexpr Register<UsartIcrInfo, UsartIcrBit> USART_ICR{USART_BASE(Port), 0x20U};
/**@}*/

/**
 * @defgroup USART_RDR_GROUP		USART Data Register Group
 * @{
 */
SETUP_REGISTER_INFO(UsartRDrInfo,																			 /************ Description ************/
										Bit<8, std::uint8_t, BitMod::RdOnly>{BitPos_t{0}}	 // Receive data value
)

enum class UsartRDrBit { RDr };

template <UsartNum Port>
static constexpr Register<UsartRDrInfo, UsartRDrBit> USART_RDR{USART_BASE(Port), 0x24U};

/**@}*/

/**
 * @defgroup USART_TDR_GROUP		USART Data Register Group
 * @{
 */
SETUP_REGISTER_INFO(UsartTDrInfo,				 /************ Description ************/
										Bit<8>{BitPos_t{0}}	 // Transmit data value
)

enum class UsartTDrBit { TDr };

template <UsartNum Port>
static constexpr Register<UsartTDrInfo, UsartTDrBit> USART_TDR{USART_BASE(Port), 0x28U};

/**@}*/

}	 // namespace cpp_stm32::stm32::l4