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
    inherited_properties({})
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
    if (!this -> current_token)
        cerr <<"Unexpected end of input!\n";
}
bool tml_parser::current_token_type_is_this(tml_token_type type)
{
    return this -> current_token -> type == type;
}
    
bool tml_parser::previous_token_type_is_this(tml_token_type type)
{
    return this -> previous_token -> type == type;
}

void tml_parser::update_inheritable()
{
    this -> inheritable_properties.clear();
    while (this -> tag_depth < this -> inherited_properties.size())
        this -> inherited_properties.pop_back();

    for (int i = this -> tag_depth; i >= 0; --i)
    {
        for (const auto& inherited_property:this -> inherited_properties)
            for (const auto& value_:inherited_property.second)
                if ((value_ == tml_property::_horizontal && !this -> inheritable_properties_contains(tml_property::_vertical  )) || 
                    (value_ == tml_property::_vertical   && !this -> inheritable_properties_contains(tml_property::_horizontal)) ||
                    (value_ == tml_property::_top        && !this -> inheritable_properties_contains(tml_property::_low       )) ||
                    (value_ == tml_property::_low        && !this -> inheritable_properties_contains(tml_property::_top       )) ||
                    (value_ == tml_property::_left       && !this -> inheritable_properties_contains(tml_property::_right     )) ||
                    (value_ == tml_property::_right      && !this -> inheritable_properties_contains(tml_property::_left      )))
                        this -> inheritable_properties.push_back(value_);
    }
}

void tml_parser::set_color(string color_value)
{
    auto it = __colors.find(color_value);
    if (it != __colors.end())
        this -> tag_color = it -> second;
}

void tml_parser::add_inherited_properties(vector<string> properties_value)
{
    vector<tml_property> vector_to_add;
    for (const auto&property_value:properties_value)
    {
        auto it = __properties.find(property_value);
        if (it != __properties.end())
            vector_to_add.push_back(it -> second);
    };
    this -> inherited_properties.push_back(pair(this -> tag_depth,vector_to_add));
}

bool tml_parser::inheritable_properties_contains(tml_property property)
{
    for (const auto& property_subject:this -> inheritable_properties)
        if (property_subject == property) return true;
    return false;
}

unique_ptr<tml_ast_struct> tml_parser::parse()
{
    if (this -> current_token_type_is_this(tml_token_type::eof))
       return nullptr;
    if (this -> current_token_type_is_this(tml_token_type::error)      ||
        this -> current_token_type_is_this(tml_token_type::whitespace) ||
        this -> current_token_type_is_this(tml_token_type::warning))
    {
        this -> eat();
    }
    if (this -> current_token_type_is_this(tml_token_type::delimiter))
    {
        this -> tag_depth++;
        this -> eat();
        string value;
        bool is_in_the_tail = false;
        bool tag_name_satisfied = false;
        // ... document ... link ... paragraph ... line ... word
        if (this -> current_token_type_is_this(tml_token_type::eof)&&this -> current_token_type_is_this(tml_token_type::backtick))
        {}
    }
    else if (this -> current_token_type_is_this(tml_token_type::content))
    {
        auto ast = init_ast(tml_color::black,{},this -> current_token -> value);
        this -> eat();
        return ast;
    }
    cerr << "!!parser err: misplaced token\n\t>" << this -> current_token ->value << "<\n\t ^\n\t |\n\t this token does not follow the set rules.\n";
    exit(1);
}


tml_parser::~tml_parser()
{}





/*

tml_token_type::backtick
tml_token_type::equals_sign
tml_token_type::identifier
tml_token_type::m_comment
tml_token_type::s_comment
tml_token_type::content
tml_token_type::t_string
tml_token_type::c_string
tml_token_type::opening_curly_brace
tml_token_type::closing_curly_brace
tml_token_type::semi_colon
tml_token_type::comma

*/