#include <picosha2.h>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include <thread>
#define M 4
#define L 100

namespace logging = boost::log;
using namespace std;

void func( int thread_num ) {
	srand( time( 0 ) + thread_num );
	while( true )
	{
		std::vector<unsigned char> v( L );
		std::generate( v.begin(), v.end(), std::rand );

		std::string hash;
		picosha2::hash256_hex_string( v, hash );

		// BOOST_LOG_TRIVIAL( trace ) << thread_num << v << hash;

		if( hash[64 - 1] == '0' &&
			hash[64 - 2] == '0' &&
			hash[64 - 3] == '0' &&
			hash[64 - 4] == '0' ) {
			std::cout << hash << std::endl;
			//BOOST_LOG_TRIVIAL( info ) << hash;
		}
	}
}

using namespace std;

int main( int argc, char** argv )
{
	unsigned num_cpu;

	if( argc < 2 )
	{
		num_cpu = thread::hardware_concurrency();
	}
	else
	{
		num_cpu = stoi( argv[1] );

	}
	vector<thread> threads;
		
	for( int i = 0; i < num_cpu; ++i )
		threads.emplace_back( func, i );

	for( int i = 0; i < num_cpu; ++i )
		threads[i].join();
	
	logging::add_file_log // расширенная настройка
	(
	    logging::keywords::file_name = "log_%N.log",
	    logging::keywords::rotation_size = 10 * 1024 * 1024, 
	    logging::keywords::time_based_rotation = logging::sinks::file::rotation_at_time_point{0, 0, 0},
	    logging::keywords::format = "[%TimeStamp%]: %Message%"
	);

	logging::core::get()->set_filter
	(
	    logging::trivial::severity >= logging::trivial::info
	);



	return 0;
}


