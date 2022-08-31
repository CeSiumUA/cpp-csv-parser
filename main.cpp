#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include <chrono>
#include "parser/cmd_parser.h"
#include "settings/settings.h"
#include "logger/logger.h"

using std::string;
using std::endl;
using std::fstream;
using std::vector;
using std::map;
using std::cout;

const string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void save_csv(const map<string, map<string, int>>& time_entries_map, bool write_headers);
void print_help();
fstream filein, fileout;

int main(int argc, char* argv[]){

    auto time_start = std::chrono::high_resolution_clock::now();

    if(cmd_parser::is_option_present(argv, argv+argc, "-h")){
        print_help();
        return 0;
    }

    cout << "run with -h to view all available arguments" << endl;

    settings settings = settings::load_from_cmd(argc, argv);

    logger logger (settings.redirect_output, settings.verbose, settings.redirect_output_path);

    filein.open(argv[1], std::ios::in);
    if(!filein){
        logger.log_error_string("input file not found or could not be opened");
        return 1;
    }

    fileout.open(argv[2], std::ios::out);
    if(!fileout){
        logger.log_error_string("output file could not be written");
        return 1;
    }

    vector<string> row;
    string line, value;

    map<string, map<string, int>> time_entries_map;

    if(settings.use_headers) {
        getline(filein, line);
        std::stringstream str(line);
        string message = "Input headers are: ";
        while(getline(str, value, settings.delimiter)){
            message +=(value + ", ");
        }
        message = message.substr(0, message.length() - 2);
        logger.log_string(message);
    }

    while (!filein.eof()){
        row.clear();

        getline(filein, line);
        logger.log_string("Read line from csv: " + line);
        std::stringstream str(line);
        while(getline(str, value, settings.delimiter)){
            row.push_back(value);
        }

        std::stringstream date_str(row[6]);

        vector<string> dates;
        while(getline(date_str, value, '-')){
            dates.push_back(value);
        }

        string time_entry_date = months[stoi(dates[1])-1] + " " + dates[0];
        int time_entry_hours = stoi(row[7]);

        auto name_entry = time_entries_map.find(row[0]);

        if(name_entry != time_entries_map.end()){
            auto date_entry = name_entry->second.find(time_entry_date);
            if(date_entry != name_entry->second.end()){
                name_entry->second[time_entry_date] += time_entry_hours;
            }
            else{
                name_entry->second.insert(std::pair<string, int>(time_entry_date, time_entry_hours));
            }
        }
        else{
            map<string, int> date_map {{time_entry_date, time_entry_hours}};
            time_entries_map.insert(std::pair<string, map<string, int>>(row[0], date_map));
        }
    }

    logger.log_string("Writing CSV file...");
    save_csv(time_entries_map, settings.use_headers);
    logger.log_string("Done writing CSV!");

    logger.log_string("Closing I/O files");
    filein.close();
    fileout.close();

    auto time_stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_stop - time_start).count();

    logger.log_info_string("Done generating report. Time consumed: " + std::to_string(duration) + " milliseconds");

    logger.log_info_string("Done writing file!");

    logger.close();
}

void save_csv(const map<string, map<string, int>>& time_entries_map, bool write_headers){
    if(write_headers) {
        fileout << "Name;Month;Total hours" << endl;
    }
    for(const auto & name_iterator : time_entries_map){
        auto dates_map = name_iterator.second;
        for(auto & date_iterator : dates_map){
            fileout << name_iterator.first << ";" << date_iterator.first << ";" << date_iterator.second << endl;
        }
    }
}

void print_help(){
    cout << "First argument: input csv file, Second argument: output csv file" << endl;
    cout << "Optional flags:" << endl;
    cout << "\t-d set delimiter. Example: -d ;" << endl;
    cout << "\t-e set to use headers (skip first row of input file)" << endl;
    cout << "\t-v set to print optional logs to output" << endl;
    cout << "\t-r set to redirect output to specified in -rp file instead console. Similar to >> in cli" << endl;
    cout << "\t-rp set output path. Example: -rp /dev/null" << endl;
}