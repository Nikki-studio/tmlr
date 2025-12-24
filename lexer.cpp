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

string tml_lexer::get_eof()
{
	return string(1,'\0');
}

string tml_lexer::get_delimiter()
{}

string tml_lexer::get_backtick()
{}

string tml_lexer::get_equals_sign()
{}

string tml_lexer::get_identifier()
{}

string tml_lexer::get_t_string()
{}

string tml_lexer::get_c_string()
{}

string tml_lexer::get_opening_curly_brace()
{}

string tml_lexer::get_closing_curly_brace()
{}

string tml_lexer::get_semi_colon()
{}

void tml_lexer::_advance(int count = 1)
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

bool tml_lexer::is_char_in_the_next_x_steps_this(unsigned int x_offset,char c)
{
	if (this -> peek_ahead(x_offset) == c) return true;
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
	cout << buffer << '\n';
	return this -> _advance_with_token(tml_token_type::eof,this -> get_eof());
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
