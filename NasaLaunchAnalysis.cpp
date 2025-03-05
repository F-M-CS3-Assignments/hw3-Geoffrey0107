#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TimeCode.h"

using namespace std;
int i;

TimeCode parse_line(string line){
    string minutes;
    string hours;
    vector<string> timeList;
    stringstream ss(line);
    string word;
    vector<string> words;

    while (ss >> word) {  // Split by space
        words.push_back(word);
    }
    if (words.size() == 3) {  
        timeList.push_back(words[1]);
        stringstream ss(words[1]);
        getline(ss, hours, ':');
        getline(ss, minutes, ':');
        return TimeCode(stoi(hours),stoi(minutes), 0);
    }
    return TimeCode(0,0,0);
}
 

vector<string> split(string line , char delim){
    //How to seperate a string based on a deliminary character:
    //https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
    vector <string>result;
    string segment;
    std::stringstream ss(line);

    while (getline(ss, segment, delim)) {
        result.push_back(segment);
        //cout << segment << endl;
    }

    return result;

}

int main() {
    ifstream inFile;
    string line;
    vector<string> launchTimes;
    string time;
    TimeCode sum (0,0,0);
    int datapoint = 0;

    inFile.open("Space_Corrected.csv");
    if (!inFile.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return 1;
    }

    //since content in line is replaced everytime, nothing is done to the content in
    //the first iteration skipping the first line.
    getline(inFile, line);
    while (getline(inFile, line)) {//getline reads next line in file until ending
        launchTimes = split(line, ',');//splits the line by ","
        time = launchTimes[4]; // gets the fourth string of the line which represents time
        if (!(parse_line(time).ToString() == "0:0:0")) {// if parsed time is not empty
            sum = sum + parse_line(time);
            datapoint++;
        }
    }
    cout << "Datapoints: " << datapoint << endl;
    cout << "average: " << (sum / datapoint).ToString() << endl;
}