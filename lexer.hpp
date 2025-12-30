/** lexer.hpp
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


#ifndef LEXER_HPP_INCLUDED
#define LEXER_HPP_INCLUDED

#include "token.hpp"
enum class tml_lexer_state
{
	INITIAL,
	IN_TAG_HEAD,
	IN_TAG_TAIL,
};

class tml_lexer {
	vector<tml_lexer_state> current_states;
	tml_lexer_state current_state;
	string current_filename;
	string buffer;
	unsigned int current_line;
	unsigned int current_col;
	unsigned int buffer_position;

	vector<string> error_log;
	vector<string> warning_log;
public:
	tml_lexer(string current_filename);
	void load_file();
	char get_current_char() const;
	char peek_ahead(unsigned int offset = 1, bool _skip_whitespace = true) const;
	string get_current_char_as_string();
	string get_identifier();
	string get_content(char border = '`');
	unique_ptr<tml_token_struct> HANDLE_INITIAL();
	unique_ptr<tml_token_struct> HANDLE_IN_TAG_HEAD();
	unique_ptr<tml_token_struct> HANDLE_IN_TAG_TAIL();
	unique_ptr<tml_token_struct> get_next_token();
	unique_ptr<tml_token_struct> _advance_with_token(tml_token_type type, string value);
	unique_ptr<tml_token_struct> return_end_of_file();
	unique_ptr<tml_token_struct> skip_singleline_comments();
	unique_ptr<tml_token_struct> skip_multiline_comments();
	unique_ptr<tml_token_struct> skip_whitespace();
	bool is_current_char_whitespace_except_end_of_file();
	void _advance(int count = 1);
	void report_error(const string& message);
	void report_warning(const string& message);
	void clear_error();
	void clear_warning();
	void change_state(tml_lexer_state state);
	vector<string> get_errors();
	vector<string> get_warnings();
	bool is_current_char_this(char c);
	bool is_char_in_the_next_x_steps_this(unsigned int x_offset,char c,bool _skip_whitespace = true);
	bool is_end_of_file();
	~tml_lexer();
};

// I have chosen to exclude the method get string because I am gon use quote chars as markers for my visitors 😘🫣😘🫣😘


#endif // LEXER_HPP_INCLUDED
