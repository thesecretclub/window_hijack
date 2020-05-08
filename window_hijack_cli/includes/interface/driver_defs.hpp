#ifndef WNDHIJACK_DRIVER_DEFS_HPP
#define WNDHIJACK_DRIVER_DEFS_HPP

#include <windows.h>
#include <stdint.h>

#define IOCTL_GET_WINDOW_THREAD_CTX CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SET_WINDOW_THREAD_CTX CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1338, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

namespace drv_interface
{
	struct generic_thread_ctx_t
	{
		uint64_t window_handle;
		uint64_t thread_pointer;
		uint64_t thread_alternative;
	};
}

#endif