#ifndef LEXER_HPP_INCLUDED
#define LEXER_HPP_INCLUDED

#include "token.hpp"
enum class tml_lexer_state
{
	INITIAL,
	IN_TAG_HEAD,
	IN_TAG_PROPERTY_COLLECTION,
	IN_TAG_PROPERTY_KEY_COLLECTION,
	IN_TAG_TAIL,
	IN_S_COMMENTS,
	IN_M_COMMENTS,
};

class tml_lexer {
	vector<tml_lexer_state> current_states;
	tml_lexer_state current_state;
	string current_filename;
	string buffer;
	unsigned int current_line;
	unsigned int current_col;
	unsigned int buffer_position;
	unsigned int total_lines;

	vector<string> error_log;
	vector<string> warning_log;
public:
	tml_lexer(string current_filename);
	void load_file();
	char get_current_char() const;
	char peek_ahead(unsigned int offset = 1, bool _skip_whitespace = true) const;
	string get_delimiter();
	string get_current_char_as_string();
	string get_identifier();
	string get_string();
	unique_ptr<tml_token_struct> get_next_token();
	unique_ptr<tml_token_struct> _advance_with_token(tml_token_type type, string value);
	void _advance(int count = 1);
	void skip_whitespace();
	void skip_singleline_comments();
	void skip_muliline_comments();
	void report_error(const string& message);
	void report_warning(const string& message);
	void change_state(tml_lexer_state state);
	vector<string> get_errors();
	vector<string> get_warnings();
	bool is_current_char_this(char c);
	bool is_char_in_the_next_x_steps_this(unsigned int x_offset,char c,bool _skip_whitespace = true);
	bool is_end_of_file();
	~tml_lexer();
};




#endif // LEXER_HPP_INCLUDED
