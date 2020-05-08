#include "data_getter.hpp"

#include <modules/data_getter.hpp>

wnd_hjk::image_data_t wnd_hjk::find_module( const char* module_name )
{
	// allocate an initial buffer with 0x2000 bytes, this probably won't be enough for ZwQuerySystemInformation
	uint32_t buffer_bytes_sz = 0x2000;
	smart::alloc buffer_bytes{ ExAllocatePoolWithTag( PagedPool, buffer_bytes_sz, pool_tag ) };

	if ( !buffer_bytes )
		return {};

	// 11 is the enum for SystemModuleInformation
	auto last_status = ZwQuerySystemInformation( 11, buffer_bytes.get( ), buffer_bytes_sz, reinterpret_cast< PULONG >( &buffer_bytes_sz ) );

	// if the status returned indicates that the buffer was too small, keep reallocating until we have a buffer big enough to store the data
	while ( last_status == STATUS_INFO_LENGTH_MISMATCH )
	{
		buffer_bytes.reset( ExAllocatePoolWithTag( PagedPool, buffer_bytes_sz, pool_tag ) );

		if ( !buffer_bytes )
			return {};

		last_status = ZwQuerySystemInformation( 11, buffer_bytes.get( ), buffer_bytes_sz, reinterpret_cast< PULONG >( &buffer_bytes_sz ) );
	}

	if ( !NT_SUCCESS( last_status ) )
	{
		DBG( "[!] ZwQuerySystemInformation failed at line %lu in %s with status 0x%x\n", __LINE__, __FILE__, last_status );
		return {};
	}

	// now iterate through the data
	const auto module_list = reinterpret_cast< nt::rtl_modules* >( buffer_bytes.get( ) );

	for ( auto i = 0u; i < module_list->count; i++ )
	{
		const auto curr_module = &module_list->modules[ i ];

		// get the file name from the full file path, we could also just avoid this and do strstr instead of strcmp
		const auto curr_module_name = reinterpret_cast< char* >( curr_module->full_path ) + curr_module->file_name_offset;

		// return value of strcmp is 0 incase there's full collision, otherwise it's the first character that mismatches
		if ( strcmp( curr_module_name, module_name ) != 0 )
			continue;

		return { curr_module->image_base, curr_module->image_size };
	}

	return {};
}