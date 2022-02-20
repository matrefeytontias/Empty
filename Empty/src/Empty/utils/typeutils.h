#pragma once

#include <type_traits>

namespace Empty::utils
{
	
	template <typename T> struct unwrapped_ref
	{
		using type = T;
	};
	template <typename T> struct unwrapped_ref<std::reference_wrapper<T>>
	{
		using type = T;
	};
	template <typename T> struct unwrapped_ref<T&>
	{
		using type = T;
	};
	
	template <typename T> using unwrapped_ref_t = typename unwrapped_ref<T>::type;
	
}