/** token.hpp
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


#ifndef TOKEN_HPP_INCLUDED
#define TOKEN_HPP_INCLUDED

#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <span>
#include <memory>

using namespace std;
namespace fs = filesystem;

/**
 *  ``tag metadata` contents ... `
 */
enum class tml_token_type
{
	eof, /* end of file */
	delimiter, /* `` */
	backtick, /* ` */
	equals_sign, /* = */
	identifier, /* variable_name */
	m_comment, //
	s_comment, /* */
  whitespace, //  \n\t\r
	content, /* ... */
	t_string, /* "..." */
	c_string, /* '...' */
	opening_curly_brace, /* { */
	closing_curly_brace, /* } */
	semi_colon, /* ; */
	comma, /* , */
	error, /* ! */
	warning, /* ? */
};

typedef struct tml_token_struct
{
	tml_token_type type;
	string value;
	unsigned int line;
	unsigned int  col;
	unsigned int len;
	string file_path;
} tml_token_struct;

unique_ptr<tml_token_struct> init_token(tml_token_type type,string value,unsigned int line,unsigned int col,unsigned int len, string file_path);

void token_view(unique_ptr<tml_token_struct>& token);

#endif // TOKEN_HPP_INCLUDED

