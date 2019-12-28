#include <boost/algorithm/string/predicate.hpp>
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
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace logging = boost::log;
using namespace boost;
using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;

#define M 4
#define L 100

void Logging() {

	logging::register_simple_formatter_factory<
		logging::trivial::severity_level,
		char
	>("Severity");
	static const string format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%";

	auto sinkFile = logging::add_file_log(
		logging::keywords::file_name = "logs/log_%N.log",
		logging::keywords::rotation_size = 10 * 1024 * 1024,
		logging::keywords::format = format
	);

	auto sinkConsole = logging::add_console_log(
		cout,
		logging::keywords::format = format
	);
	sinkConsole->set_filter(
		logging::trivial::severity >= logging::trivial::info
	);

	logging::add_common_attributes();

}

void func(int thread_num) {
	srand(time(0) + thread_num);
	while (true)
	{
		vector<unsigned char> v(L);
		generate(v.begin(), v.end(), std::rand);

		string hash;
		picosha2::hash256_hex_string(v, hash);

		if (algorithm::ends_with(hash, "0000"))
		{
			cout << hash << endl;
		}
	}
}

int main(int argc, char** argv)
{
	unsigned int thread_count;

	thread_group pool;

	Logging();

	if (argc < 2)
	{
		thread_count = boost::thread::hardware_concurrency();
	}
	else {
		try {
			thread_count = lexical_cast<unsigned int>(argv[1]);
		}
		catch (const bad_lexical_cast&) {
			std::cerr << " Please enter the correct number of threads " << std::endl;
			return -1;
		}
	}

	for (int i = 0; i < thread_count; ++i)
		pool.create_thread(bind(func, i));
	pool.join_all();

	return 0;
}


