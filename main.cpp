/** main.cpp
 *  Copyright (C) 2024  Your Name <your.email@example.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "visitor.hpp"

enum arguments{
	peek_inside_of_file, // --peek -P
	run // --run -R
};

void peek_in_file(const string& source_file_path)
{
	string line;
	if (fs::is_regular_file(fs::path(source_file_path)))
	{
		ifstream source_file(source_file_path);
		while (getline(source_file,line))
		{
			cout << line << '\n';
		}
		if (source_file.is_open()) source_file.close();
	
	}
	return;
}
void eat_file(const string& source_file_path)
{
	tml_lexer lexer(source_file_path);
	while (auto token = lexer.get_next_token())
	{
		token_view(token);
		if ( token -> type != tml_token_type::eof) break;
	}
	return;
}

void check_terminal_arguments(int argc,char** argv)
{
	
	if (argc < 2) cout << "you have not given me any file to use!\n";
	for (auto i{1}; i < argc; i++) // (auto str: span(argv,argc)) // i used this so that i can prevent my binary from parsing itself by index being 1
	{
		string source_file_path(argv[i]);
		if (!fs::exists(fs::path(source_file_path))) 
		{
			cerr << "file:`" << source_file_path << "` does not exists!\nrecheck your file path.\n";
		}
		//peek_in_file(source_file_path);
		eat_file(source_file_path);        
	}
}

int main(int argc, char** argv)
{
	check_terminal_arguments(argc,argv);
	return 0;
}
