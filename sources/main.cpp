#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include <picosha2.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>

using namespace picosha2;
using namespace boost::log;
using namespace boost;
using namespace std;



void init() {

	register_simple_formatter_factory<
		trivial::severity_level,
		char
	>("Severity");
	static const string format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%";

	auto sinkFile = add_file_log(
		keywords::file_name = "logs/log_%N.log",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::format = format
	);

	auto sinkConsole = add_console_log(
		cout,
		keywords::format = format
	);
	sinkConsole->set_filter(
		trivial::severity >= trivial::info
	);

	add_common_attributes();

}

void create_threads(int thread_num) {
	srand(time(0) + thread_num);

	const unsigned int data_size = 100;

	while (true)
	{
		vector<unsigned char> v(data_size);
		generate(v.begin(), v.end(), std::rand);

		string hash;
		hash256_hex_string(v, hash);

		if (algorithm::ends_with(hash, "0000"))
		{
			cout << hash << endl;
		}
	}
}

int main(int argc, char** argv)
{
	unsigned int thread_count=0;

	thread_group pool;

	init();

	if (argc < 2)
	{
		thread_count = thread::hardware_concurrency();
	}
	else {
		try {
			thread_count = lexical_cast<unsigned int>(argv[1]);
		}
		catch (const bad_lexical_cast&) {
			cerr << " Please enter the correct number of threads " << endl;
			return -1;
		}
	}

	for (int i = 0; i < thread_count; ++i)
		pool.create_thread(bind(create_threads, i));
	pool.join_all();

}


