/** lexer.cpp
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
		this -> report_error("Cannot open file: " + this -> current_filename + "\nthings to recheck\n- your filepath\n- check file permission\n- if memory has run out");
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
	if (this -> buffer_position >= this -> buffer.size() ) return '\0';
	return this -> buffer[this -> buffer_position];
}

char tml_lexer::peek_ahead(unsigned int offset, bool _skip_whitespace ) const 
{
	size_t pos = this -> buffer_position + offset;
	if (pos >= this -> buffer.size()) return '\0';
	if (_skip_whitespace)
		while (pos < this -> buffer.size() && isspace(this -> buffer[pos]))
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
	string value = "";
	if (this -> is_end_of_file() ) return value;
	while ( !this -> is_end_of_file() &&  // is not end of file
			!this -> is_current_char_this(border) && // and is not the border 
			!this -> is_current_char_this('\0') && // is not null 
			!(
				this -> is_current_char_this('/') &&
				(
					this -> is_char_in_the_next_x_steps_this(1,'/',false) || // is not single line comment
					this -> is_char_in_the_next_x_steps_this(1,'*',false)    // or is not multiline comments
				)
			)
		)
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

unique_ptr<tml_token_struct> tml_lexer::skip_whitespace()
{
	int line(this -> current_line),col(this -> current_col);
	while (this -> is_current_char_whitespace_except_end_of_file())
		this -> _advance();
	return init_token(tml_token_type::whitespace,"",line,col,1,this -> current_filename);
}

bool tml_lexer::is_current_char_whitespace_except_end_of_file()
{
	return (!this -> is_end_of_file()) && isspace(this -> get_current_char());
}

unique_ptr<tml_token_struct> tml_lexer::skip_singleline_comments()
{
	unsigned int line(this -> current_line),col(this -> current_col),len(0);
	this -> _advance(2);
	len += 2;
	while (!this -> is_end_of_file()&& !this -> is_current_char_this('\n'))
	{
		this -> _advance();
		len++;
	}
	if (this -> is_current_char_this('\n'))
	{
		this -> _advance();
		len++;
	}
	return init_token(tml_token_type::s_comment,"",line,col,len,this -> current_filename);
}

unique_ptr<tml_token_struct> tml_lexer::skip_multiline_comments()
{
	unsigned int line(this -> current_line),col(this -> current_col),len(0);
	this -> _advance(2);
	len += 2;
	int depth = 1;
	while (depth > 0 && this -> buffer_position < buffer.size())
	{
		if (this -> is_current_char_this('/') && 
				this -> is_char_in_the_next_x_steps_this(1,'*',false))
		{
			depth++;
			this -> _advance(2);
			len += 2;
      continue;
		}
		if (this -> is_current_char_this('*') && 
				this -> is_char_in_the_next_x_steps_this(1,'/',false))
		{
			this -> _advance(2);
			len += 2;
      depth--;
      continue;
    }
		this -> _advance(1);
		len ++;
	}
	return init_token(tml_token_type::m_comment,"",line,col,len,this -> current_filename);
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
	string err("!!lexer err!: {(lin, col),(" + to_string(this -> current_line) + ", " + to_string(this -> current_col ) + ")}\n\t" + message);
	this -> error_log.push_back(err);
}

void tml_lexer::report_warning(const string& message)
{
	string warning("??lexer warning?: {(lin, col),(" + to_string(this -> current_line) + ", " + to_string(this -> current_col ) + ")}\n\t" + message);
	this -> warning_log.push_back(warning);
}

bool tml_lexer::is_end_of_file()
{
	return  (this -> buffer_position >= this -> buffer.size());
}

unique_ptr<tml_token_struct> tml_lexer::HANDLE_INITIAL()
{
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	if (this -> is_current_char_this('/') && this -> is_char_in_the_next_x_steps_this(1,'/',false)) return this -> skip_singleline_comments();
	if (this -> is_current_char_this('/') && this -> is_char_in_the_next_x_steps_this(1,'*',false)) return this -> skip_multiline_comments();
	string value = "";
	unique_ptr<tml_token_struct> token = nullptr;
	if (this -> is_current_char_this('`'))
	{
		if (this -> is_char_in_the_next_x_steps_this(1,'`'))
		{
			value += this -> get_current_char_as_string();
			this -> _advance(1);
			value += this -> get_current_char_as_string();
			token = init_token(tml_token_type::delimiter,value,this -> current_line,this -> current_col,value.length(),this -> current_filename);
			this -> _advance(1);
			this -> change_state( tml_lexer_state::IN_TAG_HEAD);
			return token;
		}
		value += " > " + this -> get_current_char_as_string()+ " < ";
		token = init_token(tml_token_type::content,
				value,this -> current_line,this -> current_col,1,
				this -> current_filename);
		this -> report_error("unknown token:\n\t>" + this -> get_current_char_as_string() + "<\n\t ^\n\t |\n\tthis token is placed in the wrong place. or is not part of the syntax\n\tthis must be accompanied by a tag\n\tGENERAL");
		this -> report_warning("unknown token:\n\t>" + this -> get_current_char_as_string() + "<\n\t ^\n\t |\n\tthis token might not be allowed in the lexer\n\tthis must be accompanied by a tag\n\tGENERAL");
		this -> _advance(1);
		return token;
	}
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	value += this -> get_content('`');
	token = init_token(tml_token_type::content,
			value,this -> current_line,this -> current_col,1,
			this -> current_filename);
	return token;
}

unique_ptr<tml_token_struct> tml_lexer::HANDLE_IN_TAG_HEAD()
{
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	if (this -> is_current_char_this('/') && this -> is_char_in_the_next_x_steps_this(1,'/',false)) return this -> skip_singleline_comments();
	if (this -> is_current_char_this('/') && this -> is_char_in_the_next_x_steps_this(1,'*',false)) return this -> skip_multiline_comments();
	if (this -> is_current_char_whitespace_except_end_of_file()) return this -> skip_whitespace();

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
		this -> change_state (tml_lexer_state::IN_TAG_TAIL);
		return this -> _advance_with_token(tml_token_type::backtick,this -> get_current_char_as_string());
	}
	else if (this -> is_current_char_this('"')) return this -> _advance_with_token(tml_token_type::t_string           , this -> get_current_char_as_string());
	else if (this -> is_current_char_this('\'')) return this -> _advance_with_token(tml_token_type::c_string          , this -> get_current_char_as_string());
	else if (this -> is_current_char_this('=')) return this -> _advance_with_token(tml_token_type::equals_sign        , this -> get_current_char_as_string());
	else if (this -> is_current_char_this(';')) return this -> _advance_with_token(tml_token_type::semi_colon         , this -> get_current_char_as_string());
	else if (this -> is_current_char_this(',')) return this -> _advance_with_token(tml_token_type::comma              , this -> get_current_char_as_string());
	else if (this -> is_current_char_this('{')) return this -> _advance_with_token(tml_token_type::opening_curly_brace, this -> get_current_char_as_string());
	else if (this -> is_current_char_this('}')) return this -> _advance_with_token(tml_token_type::closing_curly_brace, this -> get_current_char_as_string());
	value += " > " + this -> get_current_char_as_string()+ " < ";
	token = init_token(tml_token_type::content,
			value,this -> current_line,this -> current_col,1,
			this -> current_filename);
	this -> report_error("unknown token:\n\t>" + this -> get_current_char_as_string() + "<\n\t ^\n\t |\n\tthis token is placed in the wrong place. or is not part of the syntax\n\tTAG HEAD");
	this -> report_warning("unknown token:\n\t>" + this -> get_current_char_as_string() + "<\n\t ^\n\t |\n\tthis token might not be allowed in the lexer\n\tTAG HEAD");
	this -> _advance(1);
	return token;
}

unique_ptr<tml_token_struct> tml_lexer::HANDLE_IN_TAG_TAIL()
{
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	if (this -> is_current_char_this('/') && this -> is_char_in_the_next_x_steps_this(1,'/',false)) return this -> skip_singleline_comments();
	if (this -> is_current_char_this('/') && this -> is_char_in_the_next_x_steps_this(1,'*',false)) return this -> skip_multiline_comments();
	string value = "";
	unique_ptr<tml_token_struct> token = nullptr;

	if (this -> is_current_char_this('`'))
	{
    	unsigned int line(this -> current_line), col(this -> current_col);
    	value += this -> get_current_char_as_string();
		this -> _advance();
    	if (this -> is_current_char_this('`'))
    	{
    	  value += this -> get_current_char_as_string();
    	  token = init_token(tml_token_type::delimiter,value,line,col,value.length(),this -> current_filename);
    	  this -> _advance();
    	  this -> current_states.push_back(this -> current_state);
    	  this -> change_state (tml_lexer_state::IN_TAG_HEAD);
    	  return token;
    }
    if (this -> current_states.empty()) this -> change_state(tml_lexer_state::INITIAL);
    else if (!this -> current_states.empty())
    {
      this -> change_state(this -> current_states.back());
      this -> current_states.pop_back();
    }
    return init_token(tml_token_type::backtick,value,line,col,value.length(),this -> current_filename);// token
	}
	if (this -> is_end_of_file()) return this -> return_end_of_file();
	unsigned int line(this -> current_line),col(this -> current_col);
	value += this -> get_content('`');
	if (value.empty() && this -> is_end_of_file()) return this -> return_end_of_file();
	token = init_token(tml_token_type::content,value,line,col,value.length(),this -> current_filename);
	return token;
}

unique_ptr<tml_token_struct> tml_lexer::get_next_token()
{
	if (this -> is_end_of_file()) return this -> return_end_of_file();

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
