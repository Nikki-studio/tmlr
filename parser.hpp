/** parser.hpp
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


#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include "lexer.hpp"
#include "ast.hpp"


class tml_parser 
{
    unique_ptr<tml_token_struct> previous_token;
    unique_ptr<tml_token_struct> current_token;
    tml_lexer lexer;
    tml_lexer_state tml_parser_state;
    unsigned int tag_depth;
    tml_color tag_color;
    unordered_map<int,vector<tml_property>> inherited_property;
    public:
    tml_parser(tml_lexer lexer);
    void change_state(tml_lexer_state tml_parser_state);
    bool current_token_type_is_this(tml_token_type type);
    bool previous_token_type_is_this(tml_token_type type);
    void eat();
    void set_color(string color_value);
    void add_inherited_property();
    unique_ptr<tml_ast_struct> parse();
    ~ tml_parser();
};



#endif // PARSER_HPP_INCLUDED
