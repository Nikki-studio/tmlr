/** token.cpp
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


#include "token.hpp"

unique_ptr<tml_token_struct> init_token(tml_token_type type,string value,unsigned int line,unsigned int col,unsigned int len, string file_path)
{
	auto token = make_unique<tml_token_struct>();
	if (!token) 
	{
		cerr << "Failed to create new token!\n";
		exit(1);
		return nullptr;
	}
	token -> type = type;
	token -> value = value;
	token -> line = line;
	token -> col = col;
	token -> len = len;
	token -> file_path = file_path;
	return token;
}

void token_view(unique_ptr<tml_token_struct>& token)
{
	if (!token) return;
		cout <<
			"\ntoken\ntype:  " << static_cast<int>(token -> type) <<
			"\nvalue:  " << token -> value <<
			"\nline:  " << token -> line <<
			"\ncolumn:  " << token -> col <<
			"\nfile:  " << token -> file_path << '\n';
}
