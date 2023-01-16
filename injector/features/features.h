#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

namespace features {
	void error_log( const char* message );
	void log( const char* message );
	std::string random_string( size_t length );
	bool does_file_exist( const char* name );
	DWORD get_process_id( const char* process_name );
	uintptr_t get_module_base_address( DWORD pid, const char* mod_name );
	bool inject( DWORD process_id, const char* dll );
}
