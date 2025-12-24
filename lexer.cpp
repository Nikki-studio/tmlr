#include "lexer.hpp"


tml_lexer::tml_lexer(string current_filename)
	: current_states({}),
	  current_state(tml_lexer_state::INITIAL),
	  current_filename(current_filename),
	  buffer(""),
	  buffer_position(0),
	  current_line(1),
	  current_col(0),
	  total_lines(0),

	  error_log({}),
	  warning_log({})
{
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
		this -> report_error("failed to read file: " + this -> current_filename + "\nthings to recheck\n- your filepath\n- check file permission\n- if memory has run out");
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
	return this -> buffer[pos];
}

string tml_lexer::get_delimiter()
{
	this -> _advance(1);
	if (isspace(this -> get_current_char()))
		this -> skip_whitespace();
	if (this -> is_current_char_this('`'))
		return "``";
}

string tml_lexer::get_current_char_as_string()
{
	return string(1,this -> get_current_char());
}

string tml_lexer::get_identifier()
{
	string value;
	while (isalnum(this -> get_current_char() || this -> get_current_char() == '_'))
	{
		value.push_back(this -> get_current_char());
		this -> _advance();
	}
	return value;
}

string tml_lexer::get_string()
{
	char c = this -> get_current_char();
	string value;
	this -> _advance(1);
	while (c != this -> get_current_char())
	{
		value.push_back(this -> get_current_char());
		this -> _advance();
	}
	
	this -> _advance(1);
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
		else
		{
			current_col++;
		}
		buffer_position++;
	}
}

void tml_lexer::skip_whitespace()
{
	while (!this -> is_end_of_file() && isspace(this -> get_current_char()))
		this -> _advance();
}

void tml_lexer::skip_singleline_comments()
{
	this -> _advance(2);
	while (!this -> is_end_of_file()&& !this -> is_current_char_this('\n'))
	{
		if (this -> is_current_char_this('\\')) this -> skip_whitespace();
		else this -> _advance();
	}
	if (this -> is_current_char_this('\n')) this -> _advance();
}

void tml_lexer::skip_muliline_comments()
{
	this -> _advance(2);
	int depth = 1;
	while (depth > 0 && this -> buffer_position < buffer.size())
	{
		if (this -> is_current_char_this('/') && 
				this -> is_char_in_the_next_x_steps_this(1,'*'))
		{
			depth++;
			this -> _advance(2);
		}
		if (this -> is_current_char_this('*') && 
				this -> is_char_in_the_next_x_steps_this(1,'/'))
		{
			depth--;
			this -> _advance(2);
		}
		else
		{
			this -> _advance();
		}
	}
}

bool tml_lexer::is_current_char_this(char c)
{
	if (this -> get_current_char() == c) return true;
	return false;
}

bool tml_lexer::is_char_in_the_next_x_steps_this(unsigned int x_offset,char c, bool _skip_whitespace)
{
	if (this -> peek_ahead(x_offset,_skip_whitespace) == c) return true;
	return false;
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
	if (this -> buffer_position < this -> buffer.size()) return false;
	return true;
}

unique_ptr<tml_token_struct> tml_lexer::get_next_token()
{
	cout << this -> buffer;
	if (this -> is_end_of_file())
	{
		return this -> _advance_with_token(tml_token_type::eof,this -> get_current_char_as_string());
	}
	this -> _advance();
}

unique_ptr<tml_token_struct> tml_lexer::_advance_with_token(tml_token_type type, string value)
{
	return init_token(type,value, this -> current_line, this -> current_col, value.length(),this -> current_filename);
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

tml_lexer::~tml_lexer()
{}
