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

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static int check_argument(const int argc, char* argv[], int& index, const std::string& option)
{
	if (strcmp(argv[index], ("-" + option).c_str()) == 0) {
		if (++index >= argc) {
			std::cout << "Error: Missing parameter for \"-" << option << "\"\n";
			return -1;
		}
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> object_files;
	std::string              instructions_file = "";
	std::string              output_file = "";

	if (argc < 2) {
		std::cout << "Usage: make-psylink-file [object files] -c [instructions] -o [output]\n\n"
			"[object files]     - List of object files to link\n"
			" -c [instructions] - Linker instructions file\n"
			" -o [output]       - Output file\n";
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		int success;
		if ((success = check_argument(argc, argv, i, "c")) < 0) {
			return -1;
		} else if (success > 0) {
			if (!instructions_file.empty()) {
				std::cout << "Error: Instructions file already defined.\n";
				return -1;
			}
			instructions_file = argv[i];
			continue;
		}

		if ((success = check_argument(argc, argv, i, "o")) < 0) {
			return -1;
		} else if (success > 0) {
			if (!output_file.empty()) {
				std::cout << "Error: Output file already defined.\n";
				return -1;
			}
			output_file = argv[i];
			continue;
		}

		object_files.push_back(argv[i]);
	}

	if (output_file.empty()) {
		std::cout << "Error: Output symbol file not defined.\n";
		return -1;
	}

	std::ofstream output(output_file, std::ios::out);
	if (!output.is_open()) {
		std::cout << "Error: Cannot open \"" << output_file << "\" for writing.\n";
		return -1;
	}

	output << "\n";
	for (auto& object_file : object_files) {
		output << "\tinclude\t\"" << object_file << "\"\n";
	}

	if (!instructions_file.empty()) {
		std::ifstream instructions(instructions_file, std::ios::in);
		if (!instructions.is_open()) {
			std::cout << "Error: Cannot open \"" << instructions_file << "\" for reading.\n";
			return -1;
		}
		output << "\n" << instructions.rdbuf();
	}
}