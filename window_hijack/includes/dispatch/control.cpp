#include "control.hpp"

NTSTATUS wnd_hjk::control_handler( PDEVICE_OBJECT, PIRP request_packet )
{
	const auto curr_stack = request_packet->Tail.Overlay.CurrentStackLocation;

	if ( !curr_stack )
		return STATUS_INVALID_PARAMETER;

	static const auto ValidateHwnd = reinterpret_cast< nt::tag_wnd*( * )( uint64_t ) >(
		find_export( "win32kbase.sys", "ValidateHwnd" )
	);

	if ( !ValidateHwnd )
	{
		DBG( "[!] Can't find ValidateHwnd export, catastrophic error\n" );
		return STATUS_UNSUCCESSFUL;
	}

	SIZE_T bytes_operated = 0;
	NTSTATUS operation_status = STATUS_SUCCESS;

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