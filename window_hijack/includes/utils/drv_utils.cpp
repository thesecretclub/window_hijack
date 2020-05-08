#include "drv_utils.hpp"

UNICODE_STRING wnd_hjk::make_ustr( const wchar_t* str )
{
	UNICODE_STRING string{};

	string.Buffer = const_cast< wchar_t* >( str );

	auto str_len = wcslen( str ) * 2;

	if ( str_len >= 0xfffe )
		str_len = 0xfffc;

	string.Length = static_cast< USHORT >( str_len );
	string.MaximumLength = static_cast< USHORT >( str_len + 2 );

	return string;
}