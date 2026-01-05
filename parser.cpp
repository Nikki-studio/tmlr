/** parser.cpp
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


#include "parser.hpp"

tml_parser::tml_parser(tml_lexer lexer):
    lexer(lexer),
    tml_parser_state(tml_lexer_state::INITIAL),
    current_token(this -> lexer.get_next_token()),
    previous_token(nullptr),
    tag_depth(0),
    inherited_property({})
{
}

void tml_parser::change_state(tml_lexer_state tml_parser_state)
{
    this -> tml_parser_state = tml_parser_state;
}

void tml_parser::eat()
{
    this -> previous_token = move(this -> current_token);
    this -> current_token = this -> lexer.get_next_token();
}
bool tml_parser::advance_if_current_token_type_is_this(tml_token_type type)
{
    return this -> current_token -> type == type;
}
    
bool tml_parser::advance_if_previous_token_type_is_this(tml_token_type type)
{
    return this -> previous_token -> type == type;
}

unique_ptr<tml_ast_struct> tml_parser::parse()
{
    if (advance_if_current_token_type_is_this(tml_token_type::eof))
    return nullptr;
}



tml_parser::~tml_parser()
{}





/*
tml_token_type::delimiter
tml_token_type::backtick
tml_token_type::equals_sign
tml_token_type::identifier
tml_token_type::m_comment
tml_token_type::s_comment
tml_token_type::whitespace
tml_token_type::content
tml_token_type::t_string
tml_token_type::c_string
tml_token_type::opening_curly_brace
tml_token_type::closing_curly_brace
tml_token_type::semi_colon
tml_token_type::comma
tml_token_type::error
tml_token_type::warning

*/