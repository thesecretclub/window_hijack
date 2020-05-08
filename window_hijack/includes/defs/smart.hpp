#ifndef WNDHIJACK_SMART_DEFS_HPP
#define WNDHIJACK_SMART_DEFS_HPP

#include "drv.hpp"
#include <memory>

namespace smart
{
	struct object_deleter
	{
		void operator( )( void* arg ) const
		{
			if ( arg )
				ObfDereferenceObject( arg );
		}
	};

	template <typename T>
	using object = std::unique_ptr<std::remove_pointer_t<T>, object_deleter>;

	struct alloc_deleter
	{
		void operator( )( void* arg ) const
		{
			if ( arg )
				ExFreePoolWithTag( arg, 0 );
		}
	};

	using alloc = std::unique_ptr<void, alloc_deleter>;
}

#endif