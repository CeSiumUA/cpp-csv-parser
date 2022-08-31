#include <iostream>
#include "logger.h"

using std::cout;
using std::cerr;
using std::endl;

const std::string getCurrentDateTime();

logger::logger(bool redirect_logs, bool verbose, string path) {
    this->redirect_output = redirect_logs;
    this->verbose = verbose;
    this->log_file_stream.open(path);
}

void logger::log_info_string(string message) {
    string log = "INF " + getCurrentDateTime() + ": " + message;

    this->log(log, cout);
}

void logger::log_string(string message) {
    string log = "LOG " + getCurrentDateTime() + ": " + message;

    if(this->verbose){
        this->log(log, cout);
    }
}

void logger::log_error_string(string message) {
    string log = "ERR " + getCurrentDateTime() + ": " + message;

    this->log(log, cerr);
}

const std::string getCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tm;
    char       buf[80];
    tm = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tm);

    return buf;
}

void logger::log(string log, std::ostream& stream) {
    if(!this->redirect_output){
        stream << log << endl;
    }
    if(this->log_file_stream){
        this->log_file_stream << log << endl;
    }
}

void logger::close() {
    this->log_file_stream.close();
}

logger::~logger() {
    close();
}