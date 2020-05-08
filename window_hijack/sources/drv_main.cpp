#include <utils/drv_utils.hpp>

#include <dispatch/filler.hpp>
#include <dispatch/control.hpp>

void DriverUnload( PDRIVER_OBJECT driver_object )
{
	auto dos_device_ustr = USTR( wnd_hjk::dos_device_name );

	IoDeleteSymbolicLink( &dos_device_ustr );
	IoDeleteDevice( driver_object->DeviceObject );
}

NTSTATUS DriverEntry( PDRIVER_OBJECT driver_object, PUNICODE_STRING )
{
	DBG( "[+] entry point called\n" );

	auto dos_device_ustr = USTR( wnd_hjk::dos_device_name );
	auto device_ustr = USTR( wnd_hjk::device_name );

	if ( !NT_SUCCESS( IoCreateDevice( driver_object,
		 0,
		 &device_ustr,
		 FILE_DEVICE_UNKNOWN,
		 FILE_DEVICE_SECURE_OPEN,
		 FALSE,
		 &wnd_hjk::device_object ) ) )
	{
		DBG( "[!] failed to create device object\n" );
		return STATUS_UNSUCCESSFUL;
	}

	if ( !NT_SUCCESS( IoCreateSymbolicLink( &dos_device_ustr,
		 &device_ustr ) ) )
	{
		DBG( "[!] failed to create symbolic link\n" );
		return STATUS_UNSUCCESSFUL;
	}

	for ( auto i = 0u; i < IRP_MJ_MAXIMUM_FUNCTION; i++ )
		driver_object->MajorFunction[ i ] = wnd_hjk::filler_handler;

	driver_object->MajorFunction[ IRP_MJ_DEVICE_CONTROL ] = wnd_hjk::control_handler;
	driver_object->DriverUnload = DriverUnload;

	wnd_hjk::device_object->Flags |= DO_DIRECT_IO;
	wnd_hjk::device_object->Flags &= ~DO_DEVICE_INITIALIZING;

	DBG( "[!] initialized driver!\n" );

	return STATUS_SUCCESS;
}