#pragma once
#include <Windows.h>
#include <thread>
#include <array>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <stdio.h>
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

namespace ctx {
	inline HMODULE m_steam_image{ };
}