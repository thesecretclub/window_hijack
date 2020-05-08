#ifndef WNDHIJACK_EXPORT_GETTER_HPP
#define WNDHIJACK_EXPORT_GETTER_HPP

#include <defs/nt.hpp>
#include <defs/smart.hpp>

#include "data_getter.hpp"

namespace wnd_hjk
{
	extern "C" NTSYSAPI PVOID NTAPI RtlFindExportedRoutineByName( PVOID, PCCH );

	uint8_t* find_export( const char* module_name, const char* export_name );
	uint8_t* find_export( const uint64_t module_address, const char* export_name );
}

#endif