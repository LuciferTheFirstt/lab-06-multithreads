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

namespace logging = boost::log;
using namespace boost;
using namespace std;

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
	unsigned num_cpu;

	thread_group g;

	Logging();

	if (argc < 2)
	{
		cout << num_cpu << endl;
	}
	else
	{
		const regex prov("[0-9]"); 
		if (regex_match(argv[1], prov)) {
			num_cpu = lexical_cast<int>(argv[1]);
		}
		else {
			cout << " Please enter the correct number of threads " << endl;
			exit(0);
		}
	}

	for (int i = 0; i < num_cpu; ++i)
		g.create_thread(bind(func, i));
	g.join_all();

	return 0;
}



