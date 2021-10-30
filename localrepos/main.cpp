#include <iostream>
#include <filesystem>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

// Rough and very simple config file parser
void parse_config_file(string file_name) {
	cout << "  - Parsing Git Config: " << file_name << endl;

	ifstream config_file(file_name += "\\config");
	if (config_file.is_open()) {

		string line;
		while (getline(config_file, line)) {

			// removes all whitespaces
			std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
			line.erase(end_pos, line.end());

			// commented out lines & sections
			if (line[0] == '#' || line[0] == '[' || line.empty())
				continue;

			auto delimiter_pos = line.find("=");
			auto name = line.substr(0, delimiter_pos);
			auto value = line.substr(delimiter_pos + 1);

			if (name.find("url") != std::string::npos) {
				cout << "    - Found Git Url: " << value << endl;
			}
		}
	}
}

int main() {
	string search_path;
	
	cout << "Enter your path: ";
	getline(cin, search_path);

	cout << "[info] Chosen Path: " << search_path << endl;

	auto found_git_paths = 0;
	for (const auto& p : fs::recursive_directory_iterator(search_path)) {
		if (p.is_directory()) {
			auto directory_name = p.path().filename().string();

			if (directory_name == ".git") {
				parse_config_file(p.path().string());
				found_git_paths++;
			}
		}
	}

	if(found_git_paths == 0)
		cout << "[err] No git paths have been found." << endl;
}