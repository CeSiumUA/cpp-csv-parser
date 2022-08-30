#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>

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
    struct tm date;
    int hours;
    TimeEntry(
            string name,
            string email,
            string department,
            string position,
            string project,
            string task,
            struct tm date,
            int hours) : name(std::move(name)), email(std::move(email)), department(std::move(department)), position(std::move(position)), project(std::move(project)), task(std::move(task)), date(date), hours(hours) {}
};

void update_record(const TimeEntry& timeEntry);
fstream filein, fileout;

int main(int argc, char* argv[]){
    cout << "arguments count: " << argc << endl;
    for(int x = 1; x < argc; x++){
        cout << "argument #" << x << ": " << argv[x] << endl;
    }

    filein.open(argv[1], std::ios::in);

    fileout.open(argv[2]);

    cout << "file contents" << endl;
    cout << endl;

    vector<string> row;
    string line, value;

    getline(filein, line);

    while (!filein.eof()){
        row.clear();

        getline(filein, line);
        std::stringstream str(line);
        while(getline(str, value, ';')){
            row.push_back(value);
        }
        cout << line << endl;

        struct tm tm{};

        if(!strptime(row[6].c_str(), "%Y-%m-%d", &tm)){
            cout << "date is invalid" << endl;
        }

        TimeEntry timeEntry = TimeEntry(row[0], row[1], row[2], row[3], row[4], row[5], tm, stoi(row[7]));

       update_record(timeEntry);
    }

    filein.close();
    fileout.close();
}

void update_record(const TimeEntry& timeEntry){
    string line, value;
    vector<string> row;
    while(!fileout.eof()){
        getline(fileout, line);
        if(line.empty()){
            continue;
        }
        std::stringstream str(line);
        while(getline(str, value, ';')){
            row.push_back(value);
        }
        if(row[0] == timeEntry.name){
            return;
        }
    }
    fileout.clear();
    fileout.seekg(-1);
    fileout << timeEntry.name << ";" << timeEntry.date.tm_year << "-" << timeEntry.date.tm_mon << ";" << timeEntry.hours << endl;
    fileout.clear();
    fileout.seekg(0, std::ios_base::beg);
}