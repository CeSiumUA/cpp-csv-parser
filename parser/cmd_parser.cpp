#include "cmd_parser.h"

using std::find;

bool cmd_parser::is_option_present(char **begin, char **end, const string &option) {
    return find(begin, end, option) != end;
}

char *cmd_parser::parse_option(char **begin, char **end, const string &option) {
    char** iterator = find(begin, end, option);
    if(iterator != end && ++iterator != end){
        return *iterator;
    }
    return nullptr;
}
