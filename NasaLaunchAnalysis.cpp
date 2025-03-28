#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TimeCode.h"

using namespace std;
int i;

// change return type to bool, if invalid object encountered, return false
bool parse_line(string line, TimeCode &tc){
    string minutes;
    string hours;
    stringstream ss(line);
    string word;
    vector<string> words;

    while (ss >> word) {  // Split by space
        words.push_back(word);
    }
    if (words.size() == 3) {  
        stringstream ss(words[1]);
        //https://www.geeksforgeeks.org/getline-string-c/
        // getline() can be used to extract string by delimiter
        getline(ss, hours, ':');//first extracts value up to the hour
        getline(ss, minutes, ':');//second extracts value up to the minute
        // the first value is discarded after extraction, so the only
        // thing left in string stream is the second object
        tc = TimeCode(stoi(hours), stoi(minutes), 0);
        return true;
    }else{
        tc = TimeCode(0,0,0); 
        return false;
    }
    
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
    TimeCode tc;

    inFile.open("Space_Corrected.csv");
    if (!inFile.is_open()) {
        cout << "Error: Could not open file." << endl;
        return 1;
    }

    //since content in line is replaced everytime, nothing is done to the content in
    //the first iteration skipping the first line.
    getline(inFile, line);
    while (getline(inFile, line)) {//getline reads next line in file until ending
        launchTimes = split(line, ',');//splits the line by ","
        time = launchTimes[4]; // gets the fourth string of the line which represents time
        if (parse_line(time,tc)) {// if parsed time does not return false, increment datapoint
            sum = sum + tc;
            datapoint++;
        }
    }
    cout << "Datapoints: " << datapoint << endl;
    cout << "average: " << (sum / datapoint).ToString() << endl;
}