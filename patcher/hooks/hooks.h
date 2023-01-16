#pragma once
#include "../utils/utils.h"
#include <MinHook.h>

#define registers void* ecx, void* edx

#define HOOK( target, detour, og ) \
	if (MH_CreateHook( target, detour, reinterpret_cast<void**>(&( og ))) == MH_OK \
		&& MH_EnableHook( target ) == MH_OK ) { \
		printf("[ hooks ] created target: %s\n", #target); hooks::m_targets.emplace_back(target);\
	} \
	else { std::string str = "failed init "; str += #target; throw std::runtime_error( str ); }

#define HOOK_API( module_name, func_name, detour, og ) \
	if (MH_CreateHookApi( module_name, func_name, &detour, reinterpret_cast<void**>(&( og ))) == MH_OK ) { \
		printf("[ hooks ] created target: %s\n", #func_name); \
	} \
	else { std::string str = "failed init "; str += #func_name; throw std::runtime_error( str ); }

class c_module {
	typedef int( __stdcall* runfunc_t )( int, unsigned char*, int, unsigned char*, int* );
	char gap0[ 4 ];
public:
	HMODULE m_module{ };
	HMODULE m_sec_module{ };
	runfunc_t m_runfunc{ };
	int m_last_result{ };
	std::size_t m_module_size{ };
	int m_raw{ };
private:
	char gap1C[ 16 ];
};

enum e_last_status : int {
	OK = 1,
};

namespace hooks {
	void init( );
	void destroy( );
	void __fastcall hk_runprocces_log( registers,
									   const char* a2, const char* a3, int a4 );
	int __fastcall hk_some_status_checks( registers,
										  unsigned int crc_hash,
										  char mode,
										  int a4,
										  int a5,
										  int a6,
										  int a7,
										  int a8,
										  int a9,
										  void** a10,
										  std::size_t* size );
	bool __stdcall hk_load_vac( c_module* Src, char mode );
	int __cdecl hk_crc( int a1, int a2 );
	bool __fastcall hk_crc_service_module( registers, int unCRC32, void* Src, std::size_t Size );
	HANDLE __stdcall hk_create_file_mapping( HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName );
	DWORD __stdcall hk_get_module_file_name_ex( HANDLE  hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
	int __fastcall hk_ipc_client_detecter( int a1, int a2, int a3, int a4 );

	namespace og {
		inline decltype( &hk_ipc_client_detecter ) m_ipc_client_detecter{ };
		inline decltype( &hk_get_module_file_name_ex ) m_get_module_file_name_ex{ };
		inline decltype( &hk_create_file_mapping ) m_create_file_mapping{ };
		inline decltype( &hk_crc_service_module ) m_crc_service_module{ };
		inline decltype( &hk_crc ) m_crc{ };
		inline decltype( &hk_runprocces_log ) m_runprocces_log{ };
		inline decltype( &hk_some_status_checks ) m_some_status_checks{ };
		inline decltype( &hk_load_vac ) m_load_vac{ };
	}

	inline std::vector < LPVOID > m_targets{ };
}