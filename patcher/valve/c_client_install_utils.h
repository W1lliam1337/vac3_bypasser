#pragma once
#include "../utils/utils.h"

namespace valve {
	class c_client_install_utils {
	public:
		// @xref: "%s\\Microsoft\\Windows\\GameExplorer\\%s\\%s\\%d"
		GET_VFUNC( bool( __thiscall* )( void*, int, LPCCH, BSTR ),
				   launch_tasks( int a2, LPCCH lpMultiByteStr, BSTR bstrString ), 3,
				   a2, lpMultiByteStr, bstrString );
		
		// @xref: "\"%s\\%s\" steam://uninstall/%d"
		GET_VFUNC( bool( __thiscall* )( void*, HKEY, int, BYTE*, BYTE*, BYTE*, int, BYTE* ),
				   unistall( HKEY phkResult, int a3, BYTE* lpData, BYTE* a5, BYTE* a6, int a7, BYTE* a8 ), 16,
				   phkResult, a3, lpData, a5, a6, a7, a8 );


		// @ida: 55 8B EC B8 28
		/*__forceinline bool unistall( HKEY phkResult, int a3, BYTE* lpData, BYTE* a5, BYTE* a6, int a7, BYTE* a8 ) {
			sub_100B4F50( SubKey, 260, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App %d", phkResult );
			is_wow_64_process = c_utils::is_wow_64_process( );
			phkResult = 0;
			if ( RegCreateKeyExA( HKEY_LOCAL_MACHINE, SubKey, 0, 0, 0, ( is_wow_64_process ? 0x100 : 0 ) | 0xF003F, 0, &phkResult, 0 ) )
				return 0;
			sub_100B4F50( v24, 292, "\"%s\\%s\" steam://uninstall/%d", ( this + 4 ), *( this + 264 ), v9 );
			a3 = 1;
			sub_100B85B0( v10, Data, 2048, 4 );
			sub_100B85B0( a7, v21, 2048, 4 );
			v12 = RegSetValueExA( phkResult, "DisplayIcon", 0, 1u, lpData, strlen( lpData ) + 1 );
			v13 = RegSetValueExW( phkResult, L"DisplayName", 0, 1u, Data, 2 * wcslen( Data ) + 2 ) | v12;
			v14 = RegSetValueExA( phkResult, "HelpLink", 0, 1u, a5, strlen( a5 ) + 1 ) | v13;
			v15 = RegSetValueExA( phkResult, "InstallLocation", 0, 1u, a6, strlen( a6 ) + 1 ) | v14;
			v16 = RegSetValueExW( phkResult, L"Publisher", 0, 1u, v21, 2 * wcslen( v21 ) + 2 ) | v15;
			v17 = RegSetValueExA( phkResult, "UninstallString", 0, 1u, v24, strlen( v24 ) + 1 ) | v16;
			v18 = RegSetValueExA( phkResult, "URLInfoAbout", 0, 1u, a8, strlen( a8 ) + 1 ) | v17;
			LODWORD( v19 ) = RegSetValueExA( phkResult, "NoRepair", 0, 4u, &a3, 4u ) | v18;
			HIDWORD( v19 ) = RegSetValueExA( phkResult, "NoModify", 0, 4u, &a3, 4u );
			RegCloseKey( phkResult );
			return v19 == 0;
		}*/
	};
}