/**
 * @Date:   2019-11-29T20:17:17+08:00
 * @Email:  osjacky430@gmail.com
 * @Filename: digitalout.hpp
 * @Last modified time: 2019-12-06T13:38:32+08:00
 */

#ifndef DIGITAL_OUT_HPP_
#define DIGITAL_OUT_HPP_

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include "include/driver/gpio_base.hpp"
#include "include/driver/utility.hpp"

#include "include/hal/gpio.hpp"
#include "include/hal/rcc.hpp"

template <PinName... PinNames>
class DigitalOut : public GpioBase<PinNames...> {
 private:
	static constexpr auto gpioGroupList = GpioBase<PinNames...>::makeGpioGroupList();

	using PinGrouper = PinGroupingHelper<PinNames...>;
	using GpioGroupIterator = std::make_index_sequence<std::tuple_size<decltype(gpioGroupList)>::value>;

	template <typename Tuple, std::size_t... Idx>
	constexpr void modeSetupImp(const Tuple t_tup, std::index_sequence<Idx...>) const noexcept {
		constexpr auto gpio_mode_setup_for_gpio_group = [](const auto t_tup, const auto num) constexpr noexcept {
			auto param_list = std::tuple_cat(std::make_tuple(PinGrouper::getPort(num), GpioMode::Output, GpioPupd::None),
																			 std::get<num()>(t_tup));

			std::apply([](auto&&... args) { gpio_mode_setup(std::forward<decltype(args)>(args)...); }, param_list);
		};

		(gpio_mode_setup_for_gpio_group(t_tup, ConstIndexType<Idx>{}), ...);
	}

	template <typename Tuple, std::size_t... Idx>
	constexpr void toggleImp(const Tuple tup, std::index_sequence<Idx...>) const noexcept {
		constexpr auto gpio_toggle_for_gpio_group = [](const auto t_tup, const auto num) constexpr noexcept {
			auto param_list = std::tuple_cat(std::make_tuple(PinGrouper::getPort(num)), std::get<num()>(t_tup));

			std::apply([](auto&&... args) { gpio_toggle(std::forward<decltype(args)>(args)...); }, param_list);
		};

		(gpio_toggle_for_gpio_group(tup, ConstIndexType<Idx>{}), ...);
	}

 public:
	constexpr DigitalOut() {
		// enable rcc
		GpioBase<PinNames...>::enableAllGpioClk();

		// setup mode
		this->modeSetup();
	}

	constexpr void modeSetup() const noexcept { modeSetupImp(gpioGroupList, GpioGroupIterator{}); }

	constexpr void toggle() const noexcept { toggleImp(gpioGroupList, GpioGroupIterator{}); }
};

#endif	// DIGITAL_OUT_HPP_