#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>

using std::string;
using std::cout;
using std::endl;
using std::fstream;
using std::vector;
using std::map;

const string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void save_csv(const map<string, map<string, int>>& time_entries_map);
fstream filein, fileout;

int main(int argc, char* argv[]){
    cout << "arguments count: " << argc << endl;
    for(int x = 1; x < argc; x++){
        cout << "argument #" << x << ": " << argv[x] << endl;
    }

    filein.open(argv[1], std::ios::in);

    fileout.open(argv[2], std::ios::out);

    cout << "file contents" << endl;
    cout << endl;

    vector<string> row;
    string line, value;

    map<string, map<string, int>> time_entries_map;

    getline(filein, line);

    while (!filein.eof()){
        row.clear();

        getline(filein, line);
        std::stringstream str(line);
        while(getline(str, value, ';')){
            row.push_back(value);
        }
        cout << line << endl;

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

    save_csv(time_entries_map);

    filein.close();
    fileout.close();
}

void save_csv(const map<string, map<string, int>>& time_entries_map){
    fileout << "Name;Month;Total hours" << endl;
    for(const auto & name_iterator : time_entries_map){
        auto dates_map = name_iterator.second;
        for(auto & date_iterator : dates_map){
            fileout << name_iterator.first << ";" << date_iterator.first << ";" << date_iterator.second << endl;
        }
    }
}