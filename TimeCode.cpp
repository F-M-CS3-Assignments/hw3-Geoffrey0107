#include <iostream>
#include <assert.h>
using namespace std;

#include "TimeCode.h"

TimeCode::TimeCode(const TimeCode& other) {
    this->t = other.t;
}

TimeCode:: TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec){
   t = 3600 * hr + 60* min + sec;
   //cout << "Constructor called: hr=" << hr << ", min=" << min << ", sec=" << sec << ", t=" << t << endl;
}

// This function sets the total seconds by multiplying hours by 3600.(3600 seconds in an hour)
void TimeCode::SetHours(unsigned int hours){
    t = ComponentsToSeconds(hours, GetMinutes(), GetSeconds());
}

// This function sets the total seconds by multiplying minutes by 60.(60 seconds in a minute)
void TimeCode::SetMinutes(unsigned int minutes){
    if (minutes >= 60) throw invalid_argument("Minutes must be less than 60");
    t = ComponentsToSeconds(GetHours(), minutes, GetSeconds());
}

// This function sets the total seconds by just adding the seconds
void TimeCode::SetSeconds(unsigned int seconds){
    if (seconds >= 60) throw invalid_argument("Seconds must be less than 60");
    t = ComponentsToSeconds(GetHours(), GetMinutes(), seconds);
}

// This function resets the time
void TimeCode::reset(){
    t = 0;
}
// hours = t/3600 because t represents the total seconds, each hour is 3600 seconds, 
// we do not need to worry about the remainder because they are less than an hour, so dividing 
// finds us the total number of hours.
unsigned int TimeCode::GetHours() const{
    return t / 3600;
}

// t mod 3600 finds the remainder after dividing by 3600, in other words, how many seconds 
// left after subtracting the number of hours. we do not worry about the seconds, because 
// integer division only returns an integer, rouned down. 
unsigned int TimeCode::GetMinutes() const{
    return (t % 3600)/60;
}
// Since the number of seconds is the lowest rank in total t, any seconds larger than 60 is
// either in minutes or hours, so t mod 60 finds us the remaining seconds, which is the seconds 
// for a time. 
unsigned int TimeCode::GetSeconds() const{
    return t % 60;
}

void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const{
    // if(min > 59 || sec > 59){
    //     throw invalid_argument("Invalid minutes or seconds");
    // }
    hr = GetHours();
    min = GetMinutes();
    sec = GetSeconds();
    //cout << "GetComponents called: hr=" << hr << ", min=" << min << ", sec=" << sec << endl;
}

// This function turns the instance variables hr, min and sec to a total seconds variable result.
long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec){
    long long unsigned int result;
    result = hr * 3600 + min * 60 + sec;
    return result;
}

// This function changes int variable hour, minutes and seconds to a string and concatonates them together.
string TimeCode::ToString() const {
    string result;
    result = to_string(GetHours()) + ":" + to_string(GetMinutes()) + ":" + to_string(GetSeconds());
    return result;
}

// This function overloads the '+' operator to add two TimeCode objects.
// It uses the hours, minutes, and seconds from both objects with the GetComponents function,
// converts them into total seconds with the ComponentsToSeconds function, and adds them up.
// The result is then used to construct a new TimeCode object representing the total time, only
// the seconds are passed because the Set functions will convert total seconds to hour, minutes and seconds.
TimeCode TimeCode::operator +(const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    unsigned long long int total;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);
    total = ComponentsToSeconds(h,m,s) + ComponentsToSeconds(newh,newm,news);
    return TimeCode(0,0,total);
}

// This function overloads the '-' operator to subtract two TimeCode objects.
// It uses the hours, minutes, and seconds from both objects with the GetComponents function,
// converts them into total seconds with the ComponentsToSeconds function, and subtracts them.
// The result is then used to construct a new TimeCode object representing the total time, only
// the seconds are passed because the Set functions will convert total seconds to hour, minutes and seconds.
// This function catches an exception when the resulting time is negative.
TimeCode TimeCode::operator -(const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    long long int total;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);
    total = ComponentsToSeconds(h,m,s) - ComponentsToSeconds(newh,newm,news);
    if(total < 0){
        throw invalid_argument("Subtraction results a negative number.");
    }
    
    return TimeCode(0,0,total);
}

// This function overloads the '*' operator to multiply a TimeCode object and a double variable a.
// It uses the hours, minutes, and seconds from a TimeCode object with the GetComponents function,
// converts them into total seconds with the ComponentsToSeconds function, and Multiply it with the double 
// variable. The result is then used to construct a new TimeCode object representing the total time, only
// the seconds are passed because the Set functions will convert total seconds to hour, minutes and seconds.
// This function also catches a invalid result when the multiplication results in a negative number.
TimeCode TimeCode::operator *(double a) const{
    unsigned int h, m, s;
    long long int result;
    GetComponents(h,m,s);
    result = ComponentsToSeconds(h,m,s) * a;
    if(result < 0){
        throw invalid_argument("Multiplication results a negative number.");
    }

    return TimeCode(0,0,result);
}   

// This function overloads the '/' operator to divide a TimeCode object and a double variable a.
// It uses the hours, minutes, and seconds from a TimeCode object with the GetComponents function,
// converts them into total seconds with the ComponentsToSeconds function, and divide it with the double 
// variable. The result is then used to construct a new TimeCode object representing the total time, only
// the seconds are passed because the Set functions will convert total seconds to hour, minutes and seconds.
// This function catches an exception when the denominator is zero.
TimeCode TimeCode::operator /(double a) const{
    if(a == 0 || a < 0){
        throw invalid_argument("Division by zero is not allowed.");
    }
    unsigned int h, m, s;
    long long int result  = 0;
    GetComponents(h,m,s);
    result = (h * 3600 + m * 60 + s)/a;

    return TimeCode(0,0,result);
}

// This function overloads the '==' operator to compare two TimeCode objects. It uses the hours, minutes, 
// and seconds from a TimeCode object with the GetComponents function, converts them into total seconds with 
// the ComponentsToSeconds function, compare the total seconds of the two objects. 
bool TimeCode::operator == (const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);

    if(ComponentsToSeconds(h,m,s) == ComponentsToSeconds(newh,newm,news)){
        return true;
    }
    else{
        return false;
    }
}

// This function overloads the '!=' operator to compare if two TimeCode objects are different. It uses the hours, minutes, 
// and seconds from a TimeCode object with the GetComponents function, converts them into total seconds with 
// the ComponentsToSeconds function, compare the total seconds of the two objects. 
bool TimeCode::operator != (const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);

    if(ComponentsToSeconds(h,m,s) != ComponentsToSeconds(newh,newm,news)){
        return true;
    }
    else{
        return false;
    }
}

// This function overloads the '<' operator to compare if two TimeCode objects are different. It uses the hours, minutes, 
// and seconds from a TimeCode object with the GetComponents function, converts them into total seconds with 
// the ComponentsToSeconds function, compare the total seconds of the two objects. 
bool TimeCode::operator < (const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);
    if(ComponentsToSeconds(h,m,s) < ComponentsToSeconds(newh,newm,news)){
        return true;
    }
    else{
        return false;
    }
}

// This function overloads the '<=' operator to compare if two TimeCode objects are different or equal. It uses the hours, minutes, 
// and seconds from a TimeCode object with the GetComponents function, converts them into total seconds with 
// the ComponentsToSeconds function, compare the total seconds of the two objects. 
bool TimeCode::operator <= (const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);
    if(ComponentsToSeconds(h,m,s) < ComponentsToSeconds(newh,newm,news) || ComponentsToSeconds(h,m,s) == ComponentsToSeconds(newh,newm,news)){
        return true;
    }
    else{
        return false;
    }
}

// This function overloads the '!=' operator to compare if two TimeCode objects are different. It uses the hours, minutes, 
// and seconds from a TimeCode object with the GetComponents function, converts them into total seconds with 
// the ComponentsToSeconds function, compare the total seconds of the two objects. 
bool TimeCode::operator > (const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);
    if(ComponentsToSeconds(h,m,s) > ComponentsToSeconds(newh,newm,news)){
        return true;
    }
    else{
        return false;
    }
}

// This function overloads the '!=' operator to compare if two TimeCode objects are different or equal. It uses the hours, minutes, 
// and seconds from a TimeCode object with the GetComponents function, converts them into total seconds with 
// the ComponentsToSeconds function, compare the total seconds of the two objects. 
bool TimeCode::operator >= (const TimeCode& other) const{
    unsigned int h, m, s;
    unsigned int newh, newm, news;
    GetComponents(h,m,s);
    other.GetComponents(newh, newm, news);
    if(ComponentsToSeconds(h,m,s) >= ComponentsToSeconds(newh,newm,news) || ComponentsToSeconds(h,m,s) == ComponentsToSeconds(newh,newm,news)){
        return true;
    }
    else{
        return false;
    }
}