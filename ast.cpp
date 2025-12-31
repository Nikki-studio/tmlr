/** ast.cpp
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


#include "ast.hpp"


unique_ptr<tml_ast_struct> init_ast (tml_color tag_color,vector<tml_property> tml_properties, string value)
{
    auto ast = make_unique<tml_ast_struct>();
    if (!ast) return nullptr;
    ast -> tag_color = tag_color;
    ast -> tml_properties = tml_properties;
    ast -> value = value;
    return ast;
}

void view_ast(unique_ptr<tml_ast_struct> ast)
{
    if (!ast) return;

    cout << "ast\n\ttag color: " << ast -> tag_color <<"\n\tproperties\n\t\t";
    for_each(ast -> tml_properties.begin(),ast -> tml_properties.end(),[&](auto& tml_property_x){
        cout << tml_property_x  << "\n\t\t";
    });
}
