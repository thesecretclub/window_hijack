#ifndef WNDHIJACK_DRV_UTILS_HPP
#define WNDHIJACK_DRV_UTILS_HPP

#include <defs/drv.hpp>

namespace wnd_hjk
{
	UNICODE_STRING make_ustr( const wchar_t* str );
}

#define USTR(s) ::wnd_hjk::make_ustr(s)

#endif