#ifndef WNDHIJACK_CONTROL_MJ_HPP
#define WNDHIJACK_CONTROL_MJ_HPP

#include <modules/export_getter.hpp>
#include <defs/drv.hpp>
#include <defs/nt.hpp>

#define IOCTL_GET_WINDOW_THREAD_CTX CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SET_WINDOW_THREAD_CTX CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1338, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

namespace wnd_hjk
{
	struct generic_thread_ctx_t
	{
		uint64_t window_handle;
		uint64_t thread_pointer;
	};

	NTSTATUS control_handler( PDEVICE_OBJECT device_object, PIRP request_packet );
}

#endif