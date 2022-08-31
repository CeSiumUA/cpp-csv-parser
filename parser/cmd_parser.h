#ifndef CPP_CSV_CMD_PARSER_H
#define CPP_CSV_CMD_PARSER_H

#include <string>

using std::string;

class cmd_parser {
public:
    static bool is_option_present(char** begin, char** end, const string& option);
    static char* parse_option(char** begin, char** end, const string& option);
};


#endif //CPP_CSV_CMD_PARSER_H
