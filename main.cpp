/** main.cpp
 *  Copyright (C) 2024  Joseph Wangai Mwaniki joewamwaniki@gmail.com
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


void init_colors()
{
    // First 8 standard colors
    init_pair(BLACK_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MAGENTA_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(WHITE_PAIR, COLOR_BLACK,COLOR_WHITE);
    
    // For extended colors, we need to check terminal support
    if (can_change_color() && COLORS >= 16) {
        // Define custom colors (RGB values 0-1000)
        init_color(8, 500, 500, 500);     // Grey
        init_color(9, 1000, 500, 500);    // Pink
        init_color(10, 500, 0, 0);        // Maroon
        init_color(11, 200, 600, 1000);   // Light Blue
        init_color(12, 800, 800, 800);    // Light
        init_color(13, 400, 1000, 400);   // Light Green
        init_color(14, 1000, 800, 0);     // Gold
        init_color(15, 600, 400, 200);    // Brown
        
        // Create pairs for custom colors
        init_pair(GREY_PAIR, 8, COLOR_BLACK);
        init_pair(PINK_PAIR, 9, COLOR_BLACK);
        init_pair(MAROON_PAIR, 10, COLOR_BLACK);
        init_pair(LIGHT_BLUE_PAIR, 11, COLOR_BLACK);
        init_pair(LIGHT_PAIR, 12, COLOR_BLACK);
        init_pair(LIGHT_GREEN_PAIR, 13, COLOR_BLACK);
        init_pair(GOLD_PAIR, 14, COLOR_BLACK);
        init_pair(BROWN_PAIR, 15, COLOR_BLACK);
    } else {
        // Fallback to standard colors if terminal doesn't support custom colors
        init_pair(GREY_PAIR, COLOR_WHITE, COLOR_BLACK);      // Grey -> White
        init_pair(PINK_PAIR, COLOR_MAGENTA, COLOR_BLACK);    // Pink -> Magenta
        init_pair(MAROON_PAIR, COLOR_RED, COLOR_BLACK);      // Maroon -> Red
        init_pair(LIGHT_BLUE_PAIR, COLOR_CYAN, COLOR_BLACK); // Light Blue -> Cyan
        init_pair(LIGHT_PAIR, COLOR_WHITE, COLOR_BLACK);     // Light -> White
        init_pair(LIGHT_GREEN_PAIR, COLOR_GREEN, COLOR_BLACK); // Light Green -> Green
        init_pair(GOLD_PAIR, COLOR_YELLOW, COLOR_BLACK);     // Gold -> Yellow
        init_pair(BROWN_PAIR, COLOR_YELLOW, COLOR_BLACK);    // Brown -> Yellow
    }
}


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
	tml_parser parser(lexer);
	string err_buffer = "\n🤢🤢\n---\n";
	bool error_added = false;
	string warning_buffer = "\n😡😡\n---\n";
	bool warning_added = false;
	while (!parser.current_token_type_is_this(tml_token_type::eof))
	{
		parser.eat();
		parser.show_current_token();
		if (!parser.get_lexer().get_errors().empty()) error_added = true;
		if (!parser.get_lexer().get_warnings().empty()) warning_added = true;
	}
	
	if (error_added)
	{
		cout << err_buffer;
		for_each(parser.get_lexer().get_errors().begin(),parser.get_lexer().get_errors().end(),[&](auto& e_str){cout << e_str << "\n\t---\n\n";});
	}
	if (warning_added)
	{
		cout << warning_buffer;
		for_each(parser.get_lexer().get_warnings().begin(),parser.get_lexer().get_warnings().end(),[&](auto& w_str){cout << w_str << "\n\t---\n\n";});
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
