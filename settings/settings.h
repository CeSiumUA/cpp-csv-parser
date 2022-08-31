#ifndef CPP_CSV_SETTINGS_H
#define CPP_CSV_SETTINGS_H

#include <string>
#include <vector>
#include <optional>

using std::string;
using std::vector;
using std::optional;

const string delimiter_flag = "-d";
const string use_headers_flag = "-e";
const string verbose_flag = "-v";
const string redirect_output_flag = "-r";
const string redirect_output_path_flag = "-rp";

class settings {
public:
    char delimiter = ';';
    bool use_headers = true;
    bool verbose = true;
    bool redirect_output = true;
    string redirect_output_path;
    static settings load_from_cmd(int argv_count, char* argv_begin[]);
};


#endif //CPP_CSV_SETTINGS_H
