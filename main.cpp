#include "visitor.hpp"


void peek_in_file(const string& source_file_path)
{
	string line;
	if (fs::is_regular_file(fs::path(source_file_path)))
	{
		ifstream source_file(source_file_path);
		while (getline(source_file,line))
		{
			cout << line << '\n';
		}
		if (source_file.is_open()) source_file.close();
	
	}
	return;
}
void eat_file(const string& source_file_path)
{
	tml_lexer lexer(source_file_path);
	while (lexer.get_next_token() -> type != tml_token_type::eof)
	{
		auto token = lexer.get_next_token();
		token_view(token);
	}
	return;
}

void check_terminal_arguments(int argc,char** argv)
{
	if (argc < 2) cout << "you have not given me any file to use!\n";
	for (auto i{1}; i < argc; i++) // (auto str: span(argv,argc)) // i used this so that i can prevent my binaty from parsing itself by index being 1
	{
		string source_file_path(argv[i]);
		if (!fs::exists(fs::path(source_file_path))) 
		{
			cerr << "file:`" << source_file_path << "` does not exists!\nrecheck your file path.\n";
		}
		peek_in_file(source_file_path);
		eat_file(source_file_path);        
	}
}

int main(int argc, char** argv)
{
	check_terminal_arguments(argc,argv);
	return 0;
}
