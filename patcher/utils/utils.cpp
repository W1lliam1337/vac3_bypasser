#include "utils.h"

void utils::alloc_console( ) {
	AllocConsole( );
	AttachConsole( ATTACH_PARENT_PROCESS );
	freopen_s( &m_file, "CONOUT$", "w", stdout );
}

void utils::destroy_console( ) {
	fclose( m_file );
	FreeConsole( );

	// get console window
	if ( const auto console_wnd = GetConsoleWindow( ); console_wnd ) {
		// close console window
		PostMessageW( console_wnd, WM_CLOSE, 0U, 0L );
	}
}