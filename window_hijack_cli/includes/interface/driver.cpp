#include "driver.hpp"

bool drv_interface::initialize( )
{
	driver_handle = {
		CreateFileW( driver_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, INVALID_HANDLE_VALUE ),
		&CloseHandle
	};

	if ( driver_handle.get( ) == INVALID_HANDLE_VALUE )
	{
		printf( "[-] driver isn't loaded\n" );
		return false;
	}

	printf( "[+] opened driver handle : 0x%p\n", driver_handle.get( ) );
	return true;
}

void drv_interface::get_thread( HWND window_handle, uint64_t* thread_context )
{
	generic_thread_ctx_t request{};

	request.window_handle = reinterpret_cast< uint64_t >( window_handle );
	request.thread_pointer = 0;

	if ( !DeviceIoControl(
		driver_handle.get( ),
		IOCTL_GET_WINDOW_THREAD_CTX,
		&request,
		sizeof( request ),
		&request,
		sizeof( request ),
		nullptr,
		nullptr ) )
		return;

	*thread_context = request.thread_pointer;
}

void drv_interface::set_thread( HWND window_handle, uint64_t thread_context )
{
	generic_thread_ctx_t request{};

	request.window_handle = reinterpret_cast< uint64_t >( window_handle );
	request.thread_pointer = thread_context;
	request.thread_alternative = thread_context;

	if ( !DeviceIoControl(
		driver_handle.get( ),
		IOCTL_SET_WINDOW_THREAD_CTX,
		&request,
		sizeof( request ),
		&request,
		sizeof( request ),
		nullptr,
		nullptr ) )
		return;
}