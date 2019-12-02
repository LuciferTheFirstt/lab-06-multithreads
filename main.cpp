#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
using namespace boost::filesystem;
using namespace std;
int main(int argc, char *argv[])
{

path* p;

if (argc <2)
{
p = new path (current_path());
}
else p = new path (argv[1]);

vector<string> files;
vector<string> folders;

for(const auto& dirEntry :recursive_directory_iterator(*p))
{

if(is_directory(dirEntry))
{
continue;
}

if (is_regular_file(dirEntry))
{
        const boost::regex file_filter("balance_[0-9]{8}_[0-9]{8}.txt");

        if (boost::regex_match(dirEntry.path().filename().string(), file_filter, boost::regex_constants::match_default))
        {
	  files.push_back(dirEntry.path().filename().string());
	  folders.push_back(dirEntry.path().parent_path().filename().string());
	  //std::string brocker = dirEntry.path().parent_path().filename().string();
          //std::string raw = change_extension(dirEntry.path().filename().string(), "").string();
          //raw.erase(0, 8);
          //std::string account(raw.substr(0, 8));
          //std::string date(raw.substr(9, 8));

	  //std::vector<std::string> temp_vector;
 	  //std::map<std::string, std::vector<std::string>> temp_map;
   	  //temp_vector.push_back(date);
    	  //temp_map[account] = account;
	}

}
}

for(size_t i=0; i<files.size();++i)
{
cout<<folders[i]<<" "<<files[i]<<endl;
}
delete p;
return 0;
}

//auto Brocker_find(std::string brocker, std::string account, std::string data)
//{



//}
