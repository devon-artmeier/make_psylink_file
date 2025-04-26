/*
	Copyright(c) 2024 Devon Artmeier

	Permission to use, copy, modify, and /or distribute this software
	for any purpose with or without fee is hereby granted.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
	WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIE
	WARRANTIES OF MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL THE
	AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
	DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
	PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
	TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
	PERFORMANCE OF THIS SOFTWARE.
*/

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static std::string StringToLower(const std::string& str)
{
	std::string lower_str = str;
	std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), [](unsigned char c) { return std::tolower(c); });
	return lower_str;
}

static bool CheckArgument(const int argc, char* argv[], int& index, const std::string& option, bool ignore_case = false)
{
	std::string option_copy = option;
	if (ignore_case) {
		option_copy = StringToLower(option);
	}

	if (strcmp(argv[index], ("-" + option_copy).c_str()) == 0) {
		if (++index >= argc) {
			throw std::runtime_error(("Missing parameter for \"-" + option + "\"").c_str());
		}
		return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> object_files;
	std::string              instructions_file = "";
	std::string              output_file       = "";

	if (argc < 2) {
		std::cout << "Usage: make_psylink_file -c [instructions] -o [output] [object files]" << std::endl << std::endl <<
		             "    -c [instructions] - Linker instructions file" << std::endl <<
		             "    -o [output]       - Output file" << std::endl <<
		             "    [object files]    - List of object files to link" << std::endl << std::endl;
		return -1;
	}

	try {
		for (int i = 1; i < argc; i++) {
			if (CheckArgument(argc, argv, i, "c")) {
				if (!instructions_file.empty()) {
					throw std::runtime_error("Instructions file already defined.");
				}
				instructions_file = argv[i];
				continue;
			}

			if (CheckArgument(argc, argv, i, "o")) {
				if (!output_file.empty()) {
					throw std::runtime_error("Output file already defined.");
				}
				output_file = argv[i];
				continue;
			}

			object_files.push_back(argv[i]);
		}

		if (output_file.empty()) {
			throw std::runtime_error("Output symbol file not defined.");
		}

		std::ofstream output(output_file, std::ios::out);
		if (!output.is_open()) {
			throw std::runtime_error(("Cannot open \"" + output_file + "\" for reading.").c_str());
		}

		output << std::endl;
		for (auto& object_file : object_files) {
			output << "\tinclude\t\"" << object_file << "\"" << std::endl;
		}

		if (!instructions_file.empty()) {
			std::ifstream instructions(instructions_file, std::ios::in);
			if (!instructions.is_open()) {
				throw std::runtime_error(("Cannot open \"" + instructions_file + "\" for reading.").c_str());
			}
			output << std::endl << instructions.rdbuf();
		}
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}
	
	return 0;
}