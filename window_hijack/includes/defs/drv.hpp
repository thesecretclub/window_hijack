#ifndef WNDHIJACK_DRIVER_DEFS_HPP
#define WNDHIJACK_DRIVER_DEFS_HPP

#include <ntifs.h>
#include <stdint.h>

namespace wnd_hjk
{
	struct image_data_t
	{
		uint64_t base_address;
		uint32_t image_size;
	};

	constexpr uint32_t pool_tag = static_cast< uint32_t >( 'Secr' );

	constexpr const wchar_t* device_name = L"\\Device\\secret_club";
	constexpr const wchar_t* dos_device_name = L"\\DosDevices\\secret_club";

	inline PDRIVER_OBJECT driver_object = nullptr;
	inline PDEVICE_OBJECT device_object = nullptr;
}

#define DBG(s, ...) DbgPrintEx(77,0,s,##__VA_ARGS__)

#endif