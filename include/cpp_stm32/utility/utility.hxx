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

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

/**
 *
 */
template <std::size_t idx>
using ConstIndexType = std::integral_constant<std::size_t, idx>;

/**
 *
 */
template <std::size_t start, std::size_t seq, std::size_t... end>
struct IdxIntervalImpl {
	using IdxIntervalType = typename IdxIntervalImpl<start, seq - 1, seq, end...>::IdxIntervalType;
};

template <std::size_t start, std::size_t... seq>
struct IdxIntervalImpl<start, start, seq...> {
	using IdxIntervalType = std::index_sequence<start, seq...>;
};

template <std::size_t start, std::size_t end>
struct IdxInterval {
	using IdxIntervalType = typename IdxIntervalImpl<start, end - 1, end>::IdxIntervalType;
};

template <std::size_t start, std::size_t end>
using Interval = typename IdxInterval<start, end>::IdxIntervalType;

/**
 *
 */
template <std::size_t N, typename... Args>
using NthType = std::tuple_element_t<N, std::tuple<Args...>>;

template <std::size_t N, typename T, typename... Args>
static constexpr auto IsNthTypeSame = std::is_same_v<NthType<N, Args...>, T>;

/**
 *
 */
template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
constexpr auto to_underlying(Enum t_e) noexcept {
	return static_cast<std::underlying_type_t<Enum>>(t_e);
}