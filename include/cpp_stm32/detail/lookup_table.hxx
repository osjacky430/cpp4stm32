#pragma once

#include <cstdint>
#include <tuple>
#include <utility>

#include "cpp_stm32/detail/algorithm.hxx"
#include "cpp_stm32/utility/integral_constant.hxx"

// @todo this is the best solution I can think of so far...
#define PAIR(x, y) (x), (y)

namespace cpp_stm32::detail {

template <typename Key, typename Val>
struct Pair {
	Key key;
	Val val;

	explicit constexpr Pair(Key&& t_key, Val&& t_val) noexcept : key(t_key), val(t_val) {}
};

/**
 * [Table description]
 * @param  t_val [description]
 * @return       [description]
 *
 * @todo remove this in the future, this is just bad...
 */
struct Table {
 protected:
	std::uint32_t const m_value;

 public:
	explicit constexpr Table(std::uint32_t const t_val) noexcept : m_value(t_val) {}

	[[nodiscard]] constexpr auto operator()() const noexcept { return m_value; }
	[[nodiscard]] constexpr auto get() const noexcept { return m_value; }
};

/**
 * [std::tuple_cat description]
 * @param Key    [description]
 * @param [name] [description]
 * @param KVP    [description]
 */
template <typename Tag, auto... KVPs>
struct KeyValTable : public Table {
 private:
	template <auto Key, auto Val, auto... Rest>
	struct NTTPPackDecomp {
		static constexpr std::tuple KVP{std::tuple_cat(std::tuple{Pair{Key, Val}}, NTTPPackDecomp<Rest...>::KVP)};
	};

	template <auto Key, auto Val>
	struct NTTPPackDecomp<Key, Val> {
		static constexpr std::tuple KVP{Pair{Key, Val}};
	};

	template <auto Key, std::size_t... Idx>
	static constexpr auto HAVE_KEY_IMPL(std::index_sequence<Idx...> const) noexcept {
		return ((std::get<Idx>(KEY_VAL_MAP).key == Key) || ...);
	}

 public:
	static constexpr auto KEY_VAL_MAP = NTTPPackDecomp<KVPs...>::KVP;
	static constexpr auto KEY_VAL_NUM = sizeof...(KVPs) / 2;

	template <auto Val>
	static constexpr auto HAVE_KEY() noexcept {
		return HAVE_KEY_IMPL<Val>(std::make_index_sequence<KEY_VAL_NUM>{});
	}

	template <auto Key>
	static constexpr auto UPPER_BOUND() noexcept {
		constexpr auto idx =
			detail::upper_bound(KEY_VAL_MAP, Key, [](auto const& t_key, auto const& t_elem) { return t_key < t_elem.key; });
		return std::get<idx>(KEY_VAL_MAP);
	}

	template <auto Val>
	explicit constexpr KeyValTable(std::integral_constant<decltype(Val), Val> const /*unused*/) noexcept : Table{Val} {
		static_assert(HAVE_KEY<Val>());
	}
};

/**
 * [KeyValTable description]
 * @param  const [description]
 * @return       [description]
 */
template <typename Tag, auto Lower, auto Upper>
struct KeyValTable<Tag, Lower, Upper> : public Table {
	static constexpr auto MAX = Upper;
	static constexpr auto MIN = Lower;

	template <auto Val>
	static constexpr auto HAVE_KEY() noexcept {
		return MIN <= Val && Val <= MAX;
	}

	template <auto Val>
	explicit constexpr KeyValTable(std::integral_constant<decltype(Val), Val> const /*unused*/) noexcept : Table{Val} {
		static_assert(HAVE_KEY<Val>());
	}
};

}	// namespace cpp_stm32::detail
