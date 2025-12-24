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
	eof, 
	delimiter, /* `` */
	tagname, 
	backtick, /* ` */
	equals_sign, /* = */
	identifier, /* variable_name */
	t_string, /* "..." */
	c_string, /* '...' */
	content, /* ... */
	opening_curly_brace, /* { */
	closing_curly_brace, /* } */
	semi_colon, /* ; */
	whitespace, /* \t\n */
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

