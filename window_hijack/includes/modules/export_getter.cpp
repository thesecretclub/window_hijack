#include "export_getter.hpp"

#include <modules/export_getter.hpp>

uint8_t* wnd_hjk::find_export( const uint64_t module_addr, const char* export_name )
{
	if ( !module_addr )
		return nullptr;

	// imo it's better to use this function than manually parse exports yourself
	return reinterpret_cast< uint8_t* >(
		RtlFindExportedRoutineByName( reinterpret_cast< void* >( module_addr ), export_name ) );
}

uint8_t* wnd_hjk::find_export( const char* module_name, const char* export_name )
{
	return find_export( find_module( module_name ).base_address, export_name );
}