#pragma once
#include "../../includes.h"

namespace utils {
	template <typename T>
	[[nodiscard]] static __forceinline T call_vfunc( void* instance, const std::size_t index ) {
		return ( *static_cast< T** >( instance ) )[ index ];
	}

	std::uint8_t* sig( const HMODULE module, const std::string& byte_array );
};