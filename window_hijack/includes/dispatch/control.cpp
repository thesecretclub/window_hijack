#include "control.hpp"

NTSTATUS wnd_hjk::control_handler( PDEVICE_OBJECT, PIRP request_packet )
{
	const auto curr_stack = request_packet->Tail.Overlay.CurrentStackLocation;

	if ( !curr_stack )
		return STATUS_INVALID_PARAMETER;

	// works because of the control handler being attached to the current process that called
	// the current process will have win32kbase mapped in physical memory
	// the System process will not, its aids but its how Windows works...
	static const auto ValidateHwnd = reinterpret_cast< nt::tag_wnd*( * )( uint64_t ) >(
		find_export( "win32kbase.sys", "ValidateHwnd" )
	);

	if ( !ValidateHwnd ) // should not happen unless call comes from outside of user-mode process
	{
		DBG( "[!] Can't find ValidateHwnd export, catastrophic error\n" );
		return STATUS_UNSUCCESSFUL;
	}

	SIZE_T bytes_operated = 0;
	NTSTATUS operation_status = STATUS_SUCCESS;
	
	// in order to call ValidateHwnd you must have a win32 version of your current thread
	// PsSetThreadWin32Thread & PsGetThreadWin32Thread can handle this, example:
	
	/*
	void* get_win32() {
		return PsGetThreadWin32Thread(ethr);
	}
	
	void set_win32(void* new_, void* buffer) {
		void* current = get_win32();
		PsSetThreadWin32Thread(ethr, NULL, current); // reset win32
		PsSetThreadWin32Thread(ethr, new_, NULL); // modify win32

		if (buffer && current)
			*reinterpret_cast<void**>(buffer) = current;
	}
	
	void* o_win32 = NULL;
	set_win32(process_thread.get_win32(), &o_win32);
	*/
	
	// a hard notice though, you must have a current system thread, or thread hijack one, or hook KeGetCurrentThread, for this to work
	// you can verify you get a system thread (or have one) by verifying if KeGetCurrentThread is NULL
	
	// so now to explain why this call works here, but no where else in this driver or other drivers
	// since it is in process context, and deviceiocontrol does a shit ton of thread movement and copying
	// it gives this the win32 thread of the process, where system threads dont have a win32 equivalent, process threads do
	// and this gets a process threads win32, now that you know that you can see why the example is necessary

	// btw the function actually returns a _WND*, the structure has changed a lot over the years so some self analysis is heavily required
	// i might post a more up to date version of the structure here but that is not a guarantee
	
	switch ( curr_stack->Parameters.DeviceIoControl.IoControlCode )
	{
	case IOCTL_GET_WINDOW_THREAD_CTX:
	{
		const auto curr_request = reinterpret_cast< generic_thread_ctx_t* >( request_packet->AssociatedIrp.SystemBuffer );

		if ( !curr_request )
		{
			DBG( "[!] Corrupt request sent to the dispatcher\n" );
			break;
		}

		const auto window_instance = ValidateHwnd( curr_request->window_handle );

		if ( !window_instance
			 || !window_instance->thread_info )
		{
			DBG( "[!] ValidateHwnd call failed\n" );
			break;
		}

		curr_request->thread_pointer = reinterpret_cast< uint64_t >( window_instance->thread_info->owning_thread );

		bytes_operated = sizeof( generic_thread_ctx_t );
		break;
	}
	case IOCTL_SET_WINDOW_THREAD_CTX:
	{
		const auto curr_request = reinterpret_cast< generic_thread_ctx_t* >( request_packet->AssociatedIrp.SystemBuffer );

		if ( !curr_request )
		{
			DBG( "[!] Corrupt request sent to the dispatcher\n" );
			break;
		}

		const auto window_instance = ValidateHwnd( curr_request->window_handle );

		if ( !window_instance 
			 || !window_instance->thread_info )
		{
			DBG( "[!] ValidateHwnd call failed\n" );
			break;
		}

		window_instance->thread_info->owning_thread = reinterpret_cast< PETHREAD >( curr_request->thread_pointer );

		bytes_operated = sizeof( generic_thread_ctx_t );
		break;
	}
	default:break;
	}

	request_packet->IoStatus.Information = static_cast< uint32_t >( bytes_operated );
	request_packet->IoStatus.Status = operation_status;

	IoCompleteRequest( request_packet, FALSE );

	return operation_status;
}
