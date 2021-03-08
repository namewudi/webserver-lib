#pragma once
#include <chrono>
#include <iostream>
namespace base{
    class TimeStamp{
    public:
        std::chrono::system_clock::time_point tp;
        TimeStamp(){}
        TimeStamp& setNow() {
            tp = std::chrono::system_clock::now(); 
            return *this;
        }
        void operator += (int second){
            std::chrono::seconds sec(second);
            tp += sec;
        }
        std::string toString(){
            time_t a = std::chrono::system_clock::to_time_t(tp);
            tm temp;
            localtime_r(&a, &temp);
            
            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;
            year = temp.tm_year + 1900;
            month = temp.tm_mon + 1;
            day = temp.tm_mday;
            hour = temp.tm_hour;
            minute = temp.tm_min;
            second = temp.tm_sec;
            return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day)
            + "-" + std::to_string(hour) + "-" + std::to_string(minute) + "-" + std::to_string(second);
        }
        std::string toCookieString(){
            std::string result;
            auto UTCTime = tp + std::chrono::hours(8);
            time_t a = std::chrono::system_clock::to_time_t(UTCTime);
            tm temp;
            localtime_r(&a, &temp);
            switch(temp.tm_wday){
            case 0:
                result.append("Sun");
                break;
            case 1:
                result.append("Mon");
                break;
            case 2:
                result.append("Tue");
                break;
            case 3:
                result.append("Wed");
                break;
            case 4:
                result.append("Thu");
                break;
            case 5:
                result.append("Fri");
                break;
            case 6:
                result.append("Sat");
                break;
            }
            result.append(", ");
            if(temp.tm_mday < 10){
                result.append("0");
            }
            result.append(std::to_string(temp.tm_mday));
            result.append(" ");
            switch(temp.tm_mon){
            case 0:
                result.append("Jan");
                break;
            case 1:
                result.append("Feb");
                break;
            case 2:
                result.append("Mar");
                break;
            case 3:
                result.append("Apr");
                break;
            case 4:
                result.append("May");
                break;
            case 5:
                result.append("Jun");
                break;
            case 6:
                result.append("Jul");
                break;
            case 7:
                result.append("Aug");
                break;
            case 8:
                result.append("Sep");
                break;
            case 9:
                result.append("Oct");
                break;
            case 10:
                result.append("Nov");
                break;
            case 11:
                result.append("Dec");
                break;
            }
            result.append(" ");
            result.append(std::to_string(temp.tm_year + 1900));
            result.append(" ");
            if(temp.tm_hour < 10){
                result.append("0");
            }
            result.append(std::to_string(temp.tm_hour));
            result.append(":");
            if(temp.tm_min < 10){
                result.append("0");
            }
            result.append(std::to_string(temp.tm_min));
            result.append(":");
            if(temp.tm_sec < 10){
                result.append("0");
            }
            result.append(std::to_string(temp.tm_sec));
            result.append(" UTC");
            return result;
        }
        friend bool operator < (const TimeStamp& a, const TimeStamp& b){
            time_t tta = std::chrono::system_clock::to_time_t(a.tp);
            tm tempA;
            localtime_r(&tta, &tempA);
            int yearA = tempA.tm_year + 1900;;
            int monthA = tempA.tm_mon + 1;
            int dayA = tempA.tm_mday;
            int hourA = tempA.tm_hour;
            int minuteA = tempA.tm_min;
            int secondA  = tempA.tm_sec;
            time_t ttb = std::chrono::system_clock::to_time_t(b.tp);
            tm tempB;
            localtime_r(&ttb, &tempB);
            int yearB = tempB.tm_year + 1900;;
            int monthB = tempB.tm_mon + 1;
            int dayB = tempB.tm_mday;
            int hourB = tempB.tm_hour;
            int minuteB = tempB.tm_min;
            int secondB  = tempB.tm_sec;
            if(yearA < yearB) return true;
            if(yearA > yearB) return false;
            if(monthA < monthB) return true;
            if(monthA > monthB) return false;
            if(dayA < dayB) return true;
            if(dayA > dayB) return false;
            if(hourA < hourB) return true;
            if(hourA > hourB) return false;
            if(minuteA < minuteB) return true;
            if(minuteA > minuteB) return false;
            if(secondA < secondB) return true;
            if(secondA > secondB) return false;
            return true;
        }
        
    };
}
