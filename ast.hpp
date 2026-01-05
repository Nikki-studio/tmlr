/** ast.hpp
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


#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include <unordered_map>


using namespace std;
typedef enum tml_property
{
    _attributes,
    _normal,
    _standout,
    _underline, 
    _reverse,
    _blink,
    _dim,
    _bold,
    _altcharset,
    _invisible,
    _protect,
    _horizontal,
    _left,
    _low,
    _right,
    _top,
    _vertical
} tml_property;

typedef enum tml_color
{
    black = 1,
    red,
    green,
    yellow,
    blue,
    cyan,
    magenta, //purple
    white,
    // ... 
    grey,
    pink,
    maroon,
    light_blue,
    light,
    light_green,
    gold,
    brown
}tml_color;

static const unordered_map<string,tml_property> __properties = 
{
    {"attributes",_attributes},
    {"normal",_normal},
    {"standout",_standout},
    {"underline",_underline}, 
    {"reverse",_reverse},
    {"blink",_blink},
    {"dim",_dim},
    {"bold",_bold},
    {"altcharset",_altcharset},
    {"invisible",_invisible},
    {"protect",_protect},
    {"horizontal",_horizontal},
    {"left",_left},
    {"low",_low},
    {"right",_right},
    {"top",_top},
    {"vertical",_vertical}
};

static const unordered_map<string,tml_color> __colors= 
{
    {"black",black},
    {"red",red},
    {"green",green},
    {"yellow",yellow},
    {"blue",blue},
    {"cyan",cyan},
    {"magenta",magenta},
    {"white",white},
    {"grey",grey},
    {"pink",pink},
    {"maroon",maroon},
    {"light_blue",light_blue},
    {"light",light},
    {"light_green",light_green},
    {"gold",gold},
    {"brown",brown}
};

typedef struct tml_ast_struct 
{
    tml_color tag_color;
    vector<tml_property> tml_properties;
    string value;
}tml_ast_struct;

unique_ptr<tml_ast_struct> init_ast (tml_color tag_color,vector<tml_property> tml_properties, string value);
void view_ast(unique_ptr<tml_ast_struct> ast);

#endif // AST_HPP_INCLUDED
