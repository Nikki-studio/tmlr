#include "token.hpp"

unique_ptr<tml_token_struct> init_token(tml_token_type type,string value,unsigned int line,unsigned int col,unsigned int len, string file_path)
{
	auto token = make_unique<tml_token_struct>();
	if (!token) return nullptr;
	token -> type = type;
	token -> value = value;
	token -> line = line;
	token -> col = col;
	token -> len = len;
	token -> file_path = file_path;
	return token;
}

void token_view(unique_ptr<tml_token_struct>& token)
{
	if (!token) return;
		cout <<
			"\ntoken\ntype:  " << static_cast<int>(token -> type) <<
			"\nvalue:  " << token -> value <<
			"\nline:  " << token -> line <<
			"\ncolumn:  " << token -> col <<
			"\nfile:  " << token -> file_path << '\n';
}
