#ifndef WNDHIJACK_DRIVER_INTERFACE_HPP
#define WNDHIJACK_DRIVER_INTERFACE_HPP

#include <windows.h>
#include <stdint.h>

#include <memory>

#include "driver_defs.hpp"

namespace drv_interface
{
	constexpr const wchar_t* driver_name = L"\\\\.\\secret_club";
	inline std::unique_ptr<void, decltype( &CloseHandle )> driver_handle = { nullptr, nullptr };

	bool initialize( );

	void get_thread( HWND window_handle, uint64_t* thread_context );
	void set_thread( HWND window_handle, uint64_t thread_context );
}

#endif