#include "settings.h"
#include "../parser/cmd_parser.h"

using std::nullopt;

settings settings::load_from_cmd(int argv_count, char* argv_begin[]) {
    settings settings;
    if(cmd_parser::is_option_present(argv_begin, argv_begin + argv_count, delimiter_flag)){
        char* delimiter_str = cmd_parser::parse_option(argv_begin, argv_begin + argv_count, delimiter_flag);
        if(delimiter_str != nullptr) {
            settings.delimiter = string()[0];
        }
    }

    if(cmd_parser::is_option_present(argv_begin, argv_begin + argv_count, use_headers_flag)){
        settings.use_headers = true;
    }

    if(cmd_parser::is_option_present(argv_begin, argv_begin + argv_count, verbose_flag)){
        settings.verbose = true;
    }

    if(cmd_parser::is_option_present(argv_begin, argv_begin + argv_count, redirect_output_flag)){
        settings.redirect_output = true;
    }

    if(cmd_parser::is_option_present(argv_begin, argv_begin + argv_count, redirect_output_path_flag)){
        char* path = cmd_parser::parse_option(argv_begin, argv_begin + argv_count, redirect_output_path_flag);
        if(path != nullptr) {
            settings.redirect_output_path = string(path);
        }
    }

    return settings;
}