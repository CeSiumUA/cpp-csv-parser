#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <iterator>

using std::string;
using std::cout;
using std::endl;
using std::fstream;
using std::vector;
using std::map;

class TimeEntry
{
public:
    string name;
    string email;
    string department;
    string position;
    string project;
    string task;
    string date;
    int hours;
    TimeEntry(
            string name,
            string email,
            string department,
            string position,
            string project,
            string task,
            string date,
            int hours) : name(std::move(name)), email(std::move(email)), department(std::move(department)), position(std::move(position)), project(std::move(project)), task(std::move(task)), date(std::move(date)), hours(hours) {}
};

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

        TimeEntry timeEntry = TimeEntry(row[0], row[1], row[2], row[3], row[4], row[5], months[stoi(dates[1])-1] + " " + dates[0], stoi(row[7]));

        auto name_entry = time_entries_map.find(timeEntry.name);

        if(name_entry != time_entries_map.end()){
            auto date_entry = name_entry->second.find(timeEntry.date);
            if(date_entry != name_entry->second.end()){
                name_entry->second[timeEntry.date] += timeEntry.hours;
            }
            else{
                map<string, int> date_map {{timeEntry.date, timeEntry.hours}};
                name_entry->second.insert(std::pair<string, int>(timeEntry.date, timeEntry.hours));
            }
        }
        else{
            map<string, int> date_map {{timeEntry.date, timeEntry.hours}};
            time_entries_map.insert(std::pair<string, map<string, int>>(timeEntry.name, date_map));
        }
    }

    save_csv(time_entries_map);

    filein.close();
    fileout.close();
}

void save_csv(const map<string, map<string, int>>& time_entries_map){
    for(auto name_iterator = time_entries_map.begin(); name_iterator != time_entries_map.end(); ++name_iterator){
        auto dates_map = name_iterator->second;
        for(auto date_iterator = dates_map.begin(); date_iterator != dates_map.end(); ++date_iterator){
            fileout << name_iterator->first << ";" << date_iterator->first << ";" << date_iterator->second << endl;
        }
    }
}