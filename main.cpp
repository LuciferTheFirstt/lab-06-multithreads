#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#define M 4

namespace logging = boost::log;
using namespace std;

int main(int argc, char **argv)
{
unsigned num_cpu;

if (argc<2)
{
num_cpu=thread::hardware_concurrency();
} else
{
 num_cpu=stoi(argv[1]);
 
}
vector<thread> treads(num_cpu);

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
