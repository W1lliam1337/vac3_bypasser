#include "hooks.h"

void hooks::init( ) {
	if ( MH_Initialize( ) != MH_OK ) {
		printf( "failed to init minhooks" );
		return;
	}

	static const auto some_status_checks = static_cast< void* >( utils::sig( ctx::m_steam_image, "55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 83 EC 6C" ) );
	HOOK( some_status_checks, hk_some_status_checks, og::m_some_status_checks );

	static const auto runprocess_log = static_cast< void* >( utils::sig( ctx::m_steam_image, "55 8B EC 81 EC 2C 03 00 00 8D" ) );
	HOOK( runprocess_log, hk_runprocces_log, og::m_runprocces_log );

	static const auto load_vac = static_cast< void* >( utils::sig( ctx::m_steam_image, "55 8B EC 83 EC 28 53 56 8B 75" ) );
	HOOK( load_vac, hk_load_vac, og::m_load_vac );

	static const auto crc32 = static_cast< void* >( utils::sig( ctx::m_steam_image, "55 8B EC 80 3D ?? ?? ?? ?? ?? 56 8B" ) );
	HOOK( crc32, hk_crc, og::m_crc );

	static const auto crc_service_module = static_cast< void* >( utils::sig( ctx::m_steam_image, "55 8B EC 83 EC 0C 53 56 57 8B 7D 08 8B" ) );
	HOOK( crc_service_module, hk_crc_service_module, og::m_crc_service_module );

	//static const auto ipc_client_detecter = static_cast< void* >( utils::sig( ctx::m_steam_image, "55 8B EC 83 EC 0C 53 56 8B 75 08 8B" ) );
	//HOOK( ipc_client_detecter, hk_ipc_client_detecter, og::m_ipc_client_detecter );

	//HOOK_API( L"kernel32.dll", "CreateFileMappingA", hk_create_file_mapping, og::m_create_file_mapping );
	HOOK( CreateFileMappingA, hk_create_file_mapping, og::m_create_file_mapping );

	printf( "[ hooks ] targets: %d\n", m_targets.size( ) );
	printf( "[ main ] hooks passed.\n" );
}

int __fastcall hooks::hk_ipc_client_detecter( int a1, int a2, int a3, int a4 ) {
	// @xref: "IPC client is in my process, could/should be using an in-process pipe."
	static const auto sig = static_cast< void* >( utils::sig( ctx::m_steam_image, "74 23 8B 75 0C" ) );
	if ( _ReturnAddress( ) == sig )
		return 1;

	return og::m_ipc_client_detecter( a1, a2, a3, a4 );
}

void __fastcall hooks::hk_runprocces_log( registers,
										  const char* a2, const char* a3, int a4 ) {
	// @xref: "\\logs\\runprocess_log.txt"		
	// @note: this function collects all data about the launch of any game by the client.
	// return nothing.
	return;
}

int __fastcall hooks::hk_some_status_checks( registers,
											 unsigned int crc_hash,
											 char mode,
											 int a4,
											 int a5,
											 int a6,
											 int a7,
											 int a8,
											 int a9,
											 void** a10,
											 std::size_t* size ) {
	int last_result = og::m_some_status_checks( ecx, edx, crc_hash, mode, a4, a5, a6, a7, a8, a9, a10, size );
	static const auto condition = static_cast< void* >( utils::sig( ctx::m_steam_image, "0F 84 ? ? ? ? B9 ? ? ? ? 8B 06" ) );

	// patch crc hash checks.
	if ( _ReturnAddress( ) == condition ) {
		return 1;
	}

	//mode &= ~3;
	if ( last_result != e_last_status::OK && last_result != 2 )
		last_result = e_last_status::OK;

	printf( "[ hook ] patched status: %d\n", last_result );
	return last_result;
}

bool __stdcall hooks::hk_load_vac( c_module* Src, char mode ) {
	// @ida: 55 8B EC 8B 55 0C 81
	auto is_valid_module = [ & ]( const c_module* module_struct ) {
		const auto module_data = module_struct->m_raw;
		const auto module_size = module_struct->m_module_size;

		if ( module_size < 0x200
			 || *reinterpret_cast< WORD* >( module_data ) != 23117
			 || *reinterpret_cast< DWORD* >( module_data + 60 ) < 0x40
			 || *reinterpret_cast< DWORD* >( module_data + 60 ) >= module_size - 248
			 || *reinterpret_cast< DWORD* >( ( *reinterpret_cast< DWORD* >( module_data + 60 ) ) + module_data ) != 17744 ) {
			return false;
		}

		if ( *reinterpret_cast< DWORD* >( module_data + 64 ) != 5655638
			 || *reinterpret_cast< DWORD* >( module_data + 68 ) != 1
			 || module_size < *reinterpret_cast< DWORD* >( module_data + 72 ) ) {
			return false;
		}

		const auto v6 = ( *reinterpret_cast< DWORD* >( module_data + 60 ) ) + module_data + 24;

		if ( *reinterpret_cast< WORD* >( v6 ) != 267
			 && *reinterpret_cast< WORD* >( v6 ) != 523 ) {
			return false;
		}

		return true;
	};

	// @ida: E8 ?? ?? ?? ?? 83 C4 08 85 C0 74 19 56
	// return success if module is invalid.
	if ( !is_valid_module( Src ) ) {
		Src->m_last_result = e_last_status::OK;
		return 1;
	}

	// unload vac module
	const auto raw = Src->m_raw;
	FreeLibrary( Src->m_module );
	Src->m_runfunc = nullptr;
	Src->m_raw = -1;
	Src->m_module_size = -1;
	Src->m_last_result = e_last_status::OK;

	printf( "[ hook ] VAC module was successfully unloaded\n" );;

	return 1;
}

int __cdecl hooks::hk_crc( int a1, int a2 ) {
	int og = og::m_crc( a1, a2 );
	printf( "[ crc hash ] crc32: %d\n", og );
	return og;
}

bool __fastcall hooks::hk_crc_service_module( registers, int unCRC32, void* Src, std::size_t Size ) {
	int og = og::m_crc_service_module( ecx, edx, unCRC32, Src, Size );
	printf( "[ crc hash ] unCRC32: %d\n", unCRC32 );
	printf( "[ crc hash ] return service module: %d\n", og );
	return og;
}

HANDLE __stdcall hooks::hk_create_file_mapping( HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName ) {
	if ( lpName == "Steam_{E9FD3C51-9B58-4DA0-962C-734882B19273}_Pid:%000008X" ) {
		lpName = "";
		printf( "[file mapping] found shit and successfully patched\n" );
	}

	return og::m_create_file_mapping( hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName );
}

// unused.
DWORD __stdcall hooks::hk_get_module_file_name_ex( HANDLE  hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize ) {
	return NULL;
}

void hooks::destroy( ) {
	MH_DisableHook( MH_ALL_HOOKS );
	MH_Uninitialize( );
}