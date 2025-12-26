/** lexer.cpp
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


#include "lexer.hpp"


tml_lexer::tml_lexer(string current_filename)
	: current_states({}),
	  current_state(tml_lexer_state::INITIAL),
	  current_filename(current_filename),
	  buffer(""),
	  buffer_position(0),
	  current_line(1),
	  current_col(0),

	  error_log({}),
	  warning_log({})
{
	if (this -> current_filename.empty())
	{
		this -> report_error("no file name provided!");
		return;
	}
	this -> load_file();
}

void tml_lexer::load_file()
{
	ifstream file(this -> current_filename, ios::binary | ios::ate);
	if (!file.is_open())
	{
		report_error("Cannot open file: " + this -> current_filename + "\nthings to recheck\n- your filepath\n- check file permission\n- if memory has run out");
		return;
	}

	streamsize size = file.tellg();
	file.seekg(0,ios::beg);

	this -> buffer.resize(size);

	if (!file.read(&this -> buffer[0],size))
	{
		this -> report_error("failed to read file: " + this -> current_filename + "\ngo and reckeck\n\t- your filepath\n\t- check the file permission\n\t- if memory has run out");
		this -> buffer.clear();
	}

	file.close();
}

char tml_lexer::get_current_char () const 
{
	if (this -> buffer_position >= this -> buffer.size()) return '\0';
	return this -> buffer[this -> buffer_position];
}

char tml_lexer::peek_ahead(unsigned int offset, bool _skip_whitespace ) const 
{
	size_t pos = this -> buffer_position + offset;
	if (pos >= this -> buffer.size()) return '\0';
	if (_skip_whitespace)
		while (isspace(this -> buffer[pos]))
			pos++;
	if (pos >= this -> buffer.size()) return '\0';
	return this -> buffer[pos];
}

string tml_lexer::get_current_char_as_string()
{
	if (this -> is_end_of_file()) return "";
	char current = this -> get_current_char();
	return string(&current,1);
}

string tml_lexer::get_identifier()
{
	if (this -> is_end_of_file()) return "";
	string value = "";
	while (!this -> is_end_of_file() && (isalnum(this -> get_current_char()) || this -> get_current_char() == '_'))
	{
		value.push_back(this -> get_current_char());
		this -> _advance();
	}
	return value;
}

string tml_lexer::get_content(char border)
{
	if (this -> is_end_of_file() ) return "";
	string value = "";
	while (!this -> is_end_of_file() && !(this -> is_current_char_this(border)||this -> is_current_char_this('\0')))
	{
		value += this -> get_current_char_as_string();
		this -> _advance(1);
	}
	return value;
}

void tml_lexer::_advance(int count )
{
	for (int i(0); i < count; i++)
	{
		if (this -> is_end_of_file()) return;
		if (this -> is_current_char_this('\n'))
		{
			this -> current_line++;
			this -> current_col = 0;
		}
		else this -> current_col++;
		this ->buffer_position++;
	}
}

void tml_lexer::skip_whitespace()
{
	while (!this -> is_end_of_file() && isspace(this -> get_current_char()))
		this -> _advance();
}

void tml_lexer::skip_singleline_comments()
{
	if (!(this -> is_current_char_this('/')&&this -> is_char_in_the_next_x_steps_this(1,'/',false))) return;
	else this -> _advance(2);
	while (!this -> is_end_of_file()&& !this -> is_current_char_this('\n'))
	{
		this -> _advance();
	}
	if (this -> is_current_char_this('\n')) this -> _advance();
}

void tml_lexer::skip_muliline_comments()
{
	if (!(this -> is_current_char_this('/')&&this -> is_char_in_the_next_x_steps_this(1,'/',false))) return;
	else this -> _advance(2);
	int depth = 1;
	while (depth > 0 && this -> buffer_position < buffer.size())
	{
		if (this -> is_current_char_this('/') && 
				this -> is_char_in_the_next_x_steps_this(1,'*',false))
		{
			depth++;
			this -> _advance(2);
		}
		if (this -> is_current_char_this('*') && 
				this -> is_char_in_the_next_x_steps_this(1,'/',false))
		{
			depth--;
			this -> _advance(2);
		}
		this -> _advance(1);
	}
}

bool tml_lexer::is_current_char_this(char c)
{
	if (this -> get_current_char() == c) return true;
	return false;
}

bool tml_lexer::is_char_in_the_next_x_steps_this(unsigned int x_offset,char c, bool _skip_whitespace)
{
	if (x_offset == 0) return this -> get_current_char() == c;
	return this -> peek_ahead(x_offset, _skip_whitespace) == c;
}

void tml_lexer::report_error(const string& message)
{
	string err("!!err!: {(lin, col),(" + to_string(this -> current_line) + ", " + to_string(this -> current_col ) + "}\n\t" + message);
	this -> error_log.push_back(err);
}

void tml_lexer::report_warning(const string& message)
{
	string warning("??warning?: {(lin, col),(" + to_string(this -> current_line) + ", " + to_string(this -> current_col ) + "}\n\t" + message);
	this -> warning_log.push_back(warning);
}

bool tml_lexer::is_end_of_file()
{
	return  (this -> buffer.size() < this -> buffer_position);
}

unique_ptr<tml_token_struct> tml_lexer::HANDLE_INITIAL()
{
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	string value = "";
	unique_ptr<tml_token_struct> token = nullptr;
	if (this -> is_current_char_this('`'))
	{
		cout << "here\n";
		if (this -> is_char_in_the_next_x_steps_this(1,'`'))
		{
			value += this -> get_current_char_as_string();
			this -> _advance(1);
			this -> skip_whitespace();
			value += this -> get_current_char_as_string();
			token = init_token(tml_token_type::delimiter,value,this -> current_line,this -> current_col,value.length(),this -> current_filename);
			this -> _advance(1);
			this -> current_state = tml_lexer_state::IN_TAG_HEAD;
			return token;
		}
		else /* return _advance_with_token(tml_token_type::backtick,this -> get_current_char_as_string()); */
		{
			value += " > " + this -> get_current_char_as_string()+ " < ";
			token = init_token(tml_token_type::content,
					value,this -> current_line,this -> current_col,1,
					this -> current_filename);
			this -> report_error("unknown token:\n\t>" + this -> get_current_char_as_string() + "<\n\t ^\n\t |\n\tthis token is placed in the wrong place. or is not part of the syntax\n\tthis must be accompanied by a tag\n\tGENERAL");
			this -> _advance(1);
			return token;
		}
	}
	else
	{
		if (this -> is_end_of_file()) return this -> return_end_of_file();
		value += this -> get_content('`');
		token = init_token(tml_token_type::content,
				value,this -> current_line,this -> current_col,1,
				this -> current_filename);
		return token;
	}
	return nullptr;
}

unique_ptr<tml_token_struct> tml_lexer::HANDLE_IN_TAG_HEAD()
{
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	this -> skip_whitespace();

	string value ="";
	unique_ptr<tml_token_struct> token = nullptr;
	if (isalpha(this -> get_current_char())||this -> is_current_char_this('_'))
	{
		unsigned int line(this -> current_line),col(this -> current_col);
		value += this -> get_identifier();
		token = init_token(tml_token_type::identifier,value,line,col,value.length(), this -> current_filename);
		return token;
	}
	else if (this -> is_current_char_this('`'))
	{
		this -> current_state = tml_lexer_state::IN_TAG_TAIL;
		return this -> _advance_with_token(tml_token_type::backtick,this -> get_current_char_as_string());
	}
	else if (this -> is_current_char_this('"')) return this -> _advance_with_token(tml_token_type::t_string           ,this -> get_current_char_as_string());
	else if (this -> is_current_char_this('\'')) return this -> _advance_with_token(tml_token_type::c_string          ,this -> get_current_char_as_string());
	else if (this -> is_current_char_this('=')) return this -> _advance_with_token(tml_token_type::equals_sign        ,this -> get_current_char_as_string());
	else if (this -> is_current_char_this(';')) return this -> _advance_with_token(tml_token_type::semi_colon         ,this -> get_current_char_as_string());
	else if (this -> is_current_char_this(',')) return this -> _advance_with_token(tml_token_type::comma              ,this -> get_current_char_as_string());
	else if (this -> is_current_char_this('{')) return this -> _advance_with_token(tml_token_type::opening_curly_brace,this -> get_current_char_as_string());
	else if (this -> is_current_char_this('}')) return this -> _advance_with_token(tml_token_type::closing_curly_brace,this -> get_current_char_as_string());
	value += " > " + this -> get_current_char_as_string()+ " < ";
	token = init_token(tml_token_type::content,
			value,this -> current_line,this -> current_col,1,
			this -> current_filename);
	this -> report_error("unknown token:\n\t>" + this -> get_current_char_as_string() + "<\n\t ^\n\t |\n\tthis token is placed in the wrong place. or is not part of the syntax\n\tTAG HEAD");
	this -> _advance(1);
}

unique_ptr<tml_token_struct> tml_lexer::HANDLE_IN_TAG_TAIL()
{
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	string value = "";
	unique_ptr<tml_token_struct> token = nullptr;

	if (this -> is_current_char_this('`'))
	{
		this -> current_state = tml_lexer_state::INITIAL;
		return this -> _advance_with_token(tml_token_type::backtick, this -> get_current_char_as_string());
	}

	unsigned int line(this -> current_line),col(this -> current_col);
	value += get_content('`');
	token = init_token(tml_token_type::content,value,line,col,value.length(),this -> current_filename);
	return token;
}

unique_ptr<tml_token_struct> tml_lexer::get_next_token()
{
	cout << (int)this -> current_state << "\n";
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	this -> skip_singleline_comments();
	this -> skip_muliline_comments();
	
	switch (this -> current_state)
	{
		case tml_lexer_state::IN_TAG_HEAD:
		{
			return this -> HANDLE_IN_TAG_HEAD();
		}
			break;
		case tml_lexer_state::IN_TAG_TAIL:
		{
			return this -> HANDLE_IN_TAG_TAIL();
		}
			break;
		default:
		{
			return this -> HANDLE_INITIAL();
		}
			break;
	}
	//this -> report_error("unknown token:\n\t" + this -> get_current_char_as_string() + "\n\t^\n\t|\n\tthis token is placed in the wrong place. or is not part of the syntax");
	return nullptr;
}

unique_ptr<tml_token_struct> tml_lexer::_advance_with_token(tml_token_type type, string value)
{
	unsigned int line(this -> current_line),col(this -> current_col);
	this -> _advance();
	return init_token(type,value, line, col, value.length(),this -> current_filename);
}

unique_ptr<tml_token_struct> tml_lexer::return_end_of_file()
{
	return init_token(tml_token_type::eof,"",this -> current_line,this -> current_col,1,this -> current_filename);
}

void tml_lexer::change_state(tml_lexer_state state)
{
	this -> current_state = state;
}

vector<string> tml_lexer::get_errors()
{
	return this -> error_log;
}

vector<string> tml_lexer::get_warnings()
{
	return this -> warning_log;
}
void tml_lexer::clear_error()
{
	this -> error_log.clear();
}
void tml_lexer::clear_warning()
{
	this -> warning_log.clear();
}
tml_lexer::~tml_lexer()
{}
