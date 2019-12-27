#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <picosha2.h>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <boost/log/sinks.hpp>
#include <thread>
#define M 4
#define L 100

namespace logging = boost::log;
using namespace std;

void Logging() {

	logging::register_simple_formatter_factory<
		logging::trivial::severity_level,
		char
	>("Severity");
	static const std::string format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%";

	auto sinkFile = logging::add_file_log(
		boost::log::keywords::file_name = "logs/log_%N.log",
		boost::log::keywords::rotation_size = 128 * 1024 * 1024,
		boost::log::keywords::format = format
	);
	sinkFile->set_filter(
		logging::trivial::severity >= logging::trivial::trace
	);

	auto sinkConsole = boost::log::add_console_log(
		std::cout,
		boost::log::keywords::format = format
	);
	sinkConsole->set_filter(
		logging::trivial::severity >= logging::trivial::info
	);

	boost::log::add_common_attributes();

}

void func(int thread_num) {
	srand(time(0) + thread_num);
	while (true)
	{
		std::vector<unsigned char> v(L);
		std::generate(v.begin(), v.end(), std::rand);

		std::string hash;
		picosha2::hash256_hex_string(v, hash);

		// BOOST_LOG_TRIVIAL( trace ) << thread_num << v << hash;

		if (hash[64 - 1] == '0' &&
			hash[64 - 2] == '0' &&
			hash[64 - 3] == '0' &&
			hash[64 - 4] == '0') {
			std::cout << hash << std::endl;
			//BOOST_LOG_TRIVIAL( info ) << hash;
		}
	}
}

using namespace std;

int main(int argc, char** argv)
{
	unsigned num_cpu;

	Logging();

	if (argc < 2)
	{
		num_cpu = thread::hardware_concurrency();
	}
	else
	{
		num_cpu = stoi(argv[1]);

	}
	vector<thread> threads;

	for (int i = 0; i < num_cpu; ++i)
		threads.emplace_back(func, i);

	for (int i = 0; i < num_cpu; ++i)
		threads[i].join();

	return 0;
}
