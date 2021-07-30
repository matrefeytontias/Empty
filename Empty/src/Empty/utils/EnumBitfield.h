#pragma once

#include <type_traits>

namespace utils
{
#define CONDITION std::enable_if_t<std::is_integral_v<std::underlying_type_t<E>>, int> = 0
	/**
	 * Store and manipulate enum values as bits in a bitfield.
	 * Enum values have to have an integral underlying type and
	 * actual bit values (values like 1, 2, 4, 8 as opposed to 
	 * 1, 2, 3, 4).
	 */
	template <typename E, CONDITION>
	struct EnumBitfield
	{
		using type = std::underlying_type_t<E>;

		EnumBitfield(E e) : _value(utils::value(e)) {}

		EnumBitfield<E> operator|(E e) { return _value | utils::value(e); }
		EnumBitfield<E> operator&(E e) { return _value & utils::value(e); }

		operator bool() { return _value; }
		bool operator!() { return !_value; }
		operator type() { return _value; }

	private:
		EnumBitfield(type v) : _value(v) {}
		type _value;
	};

	/**
	 * Combine two enum values into an EnumBitfield. Bring into scope
	 * with `using utils::operator|`.
	 */
	template <typename E, CONDITION>
	EnumBitfield<E> operator|(E a, E b)
	{
		return EnumBitfield(a) | b;
	}

	/**
	 * Combine two enum values into an EnumBitfield. Bring into scope
	 * with `using utils::operator&`.
	 */
	template <typename E, CONDITION>
	EnumBitfield<E> operator&(E a, E b)
	{
		return EnumBitfield(a) & b;
	}

#undef CONDITION
}
