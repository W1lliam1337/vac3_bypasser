#pragma once
#include "memory/memory.h"

#define GET_VFUNC( type, function_name, index, ...) \
	[[nodiscard]] __forceinline auto function_name { \
		return utils::call_vfunc< type >( this, index )( this, __VA_ARGS__ ); \
	}

namespace utils {
	inline FILE* m_file{ };
	void alloc_console( );
	void destroy_console( );
}