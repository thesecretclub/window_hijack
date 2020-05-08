#include <interface/driver.hpp>
#include <renderer/dx_renderer.hpp>

#include <thread>
#include <chrono>

int wmain( int argc, wchar_t** argv )
{
	if ( argc < 2 )
	{
		printf( "[!] invalid usage, proper: window_hijack_cli.exe [window_class_name]\n" );
		return 0;
	}

	if ( !drv_interface::initialize( ) )
		return 0;

	const auto exterior_window_handle = FindWindowW( argv[ 1 ], nullptr );

	if ( !exterior_window_handle )
	{
		printf( "[!] can't get exterior window's handle\n" );
		return 0;
	}

	printf( "[+] found window : 0x%p\n", exterior_window_handle );

	d2d_window_t window{ };
	d2d_renderer_t renderer{ window._handle, exterior_window_handle };

	namespace thread = std::this_thread;
	using ms = std::chrono::milliseconds;

	while ( !GetAsyncKeyState( VK_END ) )
	{
		static const auto center = wnd_hjk::vec2_t{ wnd_hjk::screen_resolution.first * 0.5f, wnd_hjk::screen_resolution.second * 0.5f };

		renderer.begin_scene( );
		renderer.draw_rectangle( center, { 150.f, 150.f } );
		renderer.end_scene( );

		thread::sleep_for( ms( 1 ) );
	}

	return 1;
}