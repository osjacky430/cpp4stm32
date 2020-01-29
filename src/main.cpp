/**
 * @Date:   2019-11-19T14:00:12+08:00
 * @Email:  osjacky430@gmail.com
 * @Filename: main.cpp
 */

#include "sys_init.hxx"

#include "cpp_stm32/driver/digitalout.hxx"
#include "cpp_stm32/driver/usart_serial.hxx"

using namespace cpp_stm32;
using namespace driver;
using namespace gpio;
using namespace usart;

int main() {
	sys::Clock::init<rcc::ClkSrc::Hse>();

	Usart const debugOut{UsartTx_v<PinName::PA_2>, UsartRx_v<PinName::PA_3>, 115200_Baud};
	DigitalOut<PinName::PA_5> led;

	while (true) {
		constexpr auto SOME_PERIOD = 1000000;
		for (int i = 0; i < SOME_PERIOD; ++i) {
			__asm__("nop");
		}

		debugOut << "c\n\r";
		led.toggle();
	}

	return 0;
}
