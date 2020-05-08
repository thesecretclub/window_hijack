#ifndef WNDHIJACK_MODULE_GETTER_HPP
#define WNDHIJACK_MODULE_GETTER_HPP

#include <defs/nt.hpp>
#include <defs/smart.hpp>

namespace wnd_hjk
{
	extern "C" NTSYSAPI NTSTATUS NTAPI ZwQuerySystemInformation( ULONG, PVOID, ULONG, PULONG );

	image_data_t find_module( const char* module_name );
}

#endif