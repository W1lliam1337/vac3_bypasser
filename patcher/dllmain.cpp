#include "includes.h"
#include "hooks/hooks.h"
#include <sddl.h>;

extern "C" BOOL WINAPI _CRT_INIT( HMODULE moduleHandle, DWORD reason, LPVOID reserved );

void main( HMODULE hModule ) {
	utils::alloc_console( );
	while ( !GetModuleHandleA( "tier0_s.dll" ) || !GetModuleHandleA( "steamservice.dll" ) )
		std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
	
	printf( "hello friend! credits to vac3 patcher:\n @tg: https://t.me/kernel_mode2\n @ds: william_coder#8276\n @github: https://github.com/W1lliam1337\n\n" );
	printf( "[ main ] steam image: 0x%p \n", ctx::m_steam_image = GetModuleHandleA( "steamservice.dll" ) );

	hooks::init( );

	while ( !GetAsyncKeyState( 0x79 ) )
		Sleep( 500 );

	utils::destroy_console( );
	hooks::destroy( );

	_CRT_INIT( hModule, DLL_PROCESS_DETACH, nullptr );
	FreeLibraryAndExitThread( hModule, 0 );

	return ExitThread( EXIT_SUCCESS );
}

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
) {
	if ( ul_reason_for_call != DLL_PROCESS_ATTACH )
		return FALSE;

	const auto thread = CreateThread( 0, 0, 
									  reinterpret_cast<LPTHREAD_START_ROUTINE>( main ), 
									  hModule, 0, 0 );
	if ( thread )
		CloseHandle( thread );

	return TRUE;
}

