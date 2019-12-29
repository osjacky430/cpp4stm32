/**
 * @Date:   2019-12-28T15:07:22+08:00
 * @Email:  osjacky430@gmail.com
 * @Filename: utility.h
 */

#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

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
