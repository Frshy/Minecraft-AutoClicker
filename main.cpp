#include <iostream>
#include <windows.h>
#include <time.h>
#include <random>
#include <thread>
#include <cassert>

namespace settings {
	struct {
		int bind = 0;
		bool enabled = false;
		int min_cps = 10;
		int max_cps = 13;
	} left, right;
}

auto get_cps( int min, int max ) -> int {
	srand( time( NULL ) );
	return 1000 / ( min + rand( ) % ( (max + 1) - min) );
}

namespace modules {
	auto left_clicking( ) -> void {
		while ( true ) {
			if ( settings::left.enabled ) {
				mouse_event( MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
			}
			Sleep( get_cps( settings::left.min_cps, settings::left.max_cps ) );
		}
	}

	auto right_clicking( ) -> void {
		while ( true ) {
			if ( settings::right.enabled ) {
				mouse_event( MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0 );
			}
			Sleep( get_cps( settings::right.min_cps, settings::right.max_cps ) );
		}
	}
}

auto setup( ) -> void {
	settings::left.bind = 'Z';
	settings::right.bind = 'X';

	std::cout << "============  Left Button Configuration  ============\n";
	std::cout << "Enter minimum cps > ";
	std::cin >> settings::left.min_cps;
	std::cout << "Enter maximum cps > ";
	std::cin >> settings::left.max_cps;
	system( "cls" );

	std::cout << "============  Right Button Configuration  ============\n";
	std::cout << "Enter minimum cps > ";
	std::cin >> settings::right.min_cps;
	std::cout << "Enter maximum cps > ";
	std::cin >> settings::right.max_cps;
	system( "cls" );

	std::thread left_clicking_th( modules::left_clicking );
	left_clicking_th.detach( );
	assert( !left_clicking_th.joinable( ) );

	std::thread right_clicking_th( modules::right_clicking );
	right_clicking_th.detach( );
	assert( !right_clicking_th.joinable( ) );
}

auto print_settings( ) -> void {
	std::cout << "Left button: " << settings::left.min_cps << " - " << settings::left.max_cps << " | " << ( char )settings::left.bind << std::endl;
	std::cout << "Right button: " << settings::right.min_cps << " - " << settings::right.max_cps << " | " << ( char )settings::right.bind << std::endl;
	std::cout << std::endl;
}

auto main( ) -> int {
	setup( );
	print_settings( );
	std::cout << "Click END on keyboard to shutdown the program\n";

	while ( !GetAsyncKeyState( VK_END ) ) {
		if ( GetAsyncKeyState( settings::left.bind ) & 1 ) settings::left.enabled ^= 1;
		if ( GetAsyncKeyState( settings::right.bind ) & 1 ) settings::right.enabled ^= 1;
		Sleep( 5 );
	}

	getchar( );
	return 0;
}
