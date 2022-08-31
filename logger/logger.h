#ifndef CPP_CSV_LOGGER_H
#define CPP_CSV_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>

using std::string;
using std::ofstream;

class logger {
public:
    logger(bool redirect_logs, bool verbose, string path);
    ~logger();
    void close();
    void log_string(string message);
    void log_info_string(string message);
    void log_error_string(string message);
private:
    bool redirect_output;
    bool verbose;
    ofstream log_file_stream;
    void log(string log, std::ostream& stream);
};


#endif //CPP_CSV_LOGGER_H
