#include <iostream>
using namespace std;

class Date
{
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    bool isOurEra;

    int dayStepFromMonth(int month) {
        int dayStep = 31;
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) dayStep = 31;
        else if (month == 4 || month == 6 || month == 9 || month == 11) dayStep = 30;
        else if (month == 2 && ((this->year % 4 == 0 && this->year % 100 != 0) || (this->year % 400 == 0))) dayStep = 29;
        else if (month == 2) dayStep = 28;
        return dayStep;
    }

public:

    Date() {
        isOurEra = false;
        year = 1960;
        month = 6;
        day = 15;
        hour = 12;
        minute = 53;
        second = 24;
    }

    Date(bool isOurEra, int year, int month, int day, int hour, int minute, int second) {
        this->isOurEra = isOurEra;

        if (year < 1) year = 1;
        this->year = year;

        if (month < 1) month = 1;
        else if (month > 12) month = 12;
        this->month = month;


        if (day < 1) day = 1;
        this->day = day;

        if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31) day = 31;
        else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) day = 30;
        else if (month == 2 && ((this->year % 4 == 0 && this->year % 100 != 0) || (this->year % 400 == 0)) && day > 29) day = 29;
        else if (month == 2 && day > 28) day = 28;
        this->day = day;


        if (hour > 23) hour = 23;
        else if (hour < 0) hour = 0;
        this->hour = hour;


        if (minute > 59) minute = 59;
        else if (minute < 0) minute = 0;
        this->minute = minute;

        if (second > 59) second = 59;
        else if (second < 0) second = 0;
        this->second = second;
    }

    Date add(int year, int month, int day, int hour, int minute, int second) {
        Date res;
        res = *this;
        int dayStep, oldDayStep;

        if (year < 0) year = 0;
        if (month < 0) month = 0;
        if (day < 0) day = 0;
        if (hour < 0) hour = 0;
        if (minute < 0) minute = 0;
        if (second < 0) second = 0;

        year += res.year; month += res.month; hour += res.hour; minute += res.minute; second += res.second;

        for (second; second >= 60; second = second - 60) minute++;
        for (minute; minute >= 60; minute = minute - 60) hour++;
        for (hour; hour >= 24; hour = hour - 24) day++;
        for (month; month > 12; month = month - 12) year++;

        dayStep = dayStepFromMonth(month);
        if (res.day > dayStep) res.day = dayStep;
        day += res.day;
        oldDayStep = dayStep;

        for (day; day > dayStep; day = day - oldDayStep) {
            oldDayStep = dayStep;
            month++;
            if (month == 13) {
                month = month - 12;
                year++;
            }
            dayStep = dayStepFromMonth(month);
        }

        res.year = year; res.month = month; res.day = day; res.hour = hour; res.minute = minute; res.second = second;

        return res;
    }

    Date subtract(int year, int month, int day, int hour, int minute, int second) {
        Date res;
        res = *this;
        int dayStep;

        if (year < 0) year = 0;
        if (month < 0) month = 0;
        if (day < 0) day = 0;
        if (hour < 0) hour = 0;
        if (minute < 0) minute = 0;
        if (second < 0) second = 0;

        year = res.year - year; month = res.month - month; hour = res.hour - hour; minute = res.minute - minute; second = res.second - second;

        for (second; second < 0; second = second + 60) minute--;
        for (minute; minute < 0; minute = minute + 60) hour--;
        for (hour; hour < 0; hour = hour + 24) res.day--;
        for (month; month < 1; month = month + 12) year--;

        dayStep = dayStepFromMonth(month);
        if (res.day > dayStep) res.day = dayStep;
        day = res.day - day;
        for (day; day < 1; day = day + dayStep) {
            month--;
            if (month == 0) {
                month = month + 12;
                year--;
            }
            dayStep = dayStepFromMonth(month);
        }

        dayStep = dayStepFromMonth(month);

        if (year <= 0) {
            year--;
            year = year * -1;
            res.isOurEra = !res.isOurEra;
        }

        res.year = year; res.month = month; res.day = day; res.hour = hour; res.minute = minute; res.second = second;

        return res;
    }

    friend ostream& operator<<(ostream& os, const Date& self)
    {
        if (self.day < 10)
            os << "0";
        os << self.day << ".";

        if (self.month < 10)
            os << "0";
        os << self.month << ".";

        if (self.year < 1000) {
            os << "0";
            if (self.year < 100) {
                os << "0";
                if (self.year < 10) {
                    os << "0";
                }
            }
        }
        os << self.year << " ";

        if (self.hour < 10)
            os << "0";
        os << self.hour << ":";
        if (self.minute < 10)
            os << "0";
        os << self.minute << ":";
        if (self.second < 10)
            os << "0";
        os << self.second;

        if (self.isOurEra) os << " of our Era";
        else os << " before our Era";
        return os;
    }

    Date& operator= (const Date& other)
    {
        this->year = other.year;
        this->month = other.month;
        this->day = other.day;
        this->hour = other.hour;
        this->minute = other.minute;
        this->second = other.second;
        this->isOurEra = other.isOurEra;

        return *this;
    }

    Date operator+(const Date& other)
    {
        Date res;
        res = *this;

        //Era check is done?
        if ((res.isOurEra && other.isOurEra) || (!res.isOurEra && !other.isOurEra))
            res = res.add(other.year, other.month, other.day, other.hour, other.minute, other.second);
        else if ((res.isOurEra && !other.isOurEra) || (!res.isOurEra && other.isOurEra))
            res = res.subtract(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return res;
    }

    Date& operator+= (const Date& other)
    {
        if ((this->isOurEra && other.isOurEra) || (!this->isOurEra && !other.isOurEra))
            *this = this->add(other.year, other.month, other.day, other.hour, other.minute, other.second);
        else if ((this->isOurEra && !other.isOurEra) || (!this->isOurEra && other.isOurEra))
            *this = this->subtract(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return *this;
    }

    Date operator-(const Date& other)
    {
        Date res;
        res = *this;

        //Era check is done?
        if ((res.isOurEra && other.isOurEra) || (!res.isOurEra && !other.isOurEra))
            res = res.subtract(other.year, other.month, other.day, other.hour, other.minute, other.second);
        else if ((res.isOurEra && !other.isOurEra) || (!res.isOurEra && other.isOurEra))
            res = res.add(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return res;
    }

    Date& operator-= (const Date& other)
    {
        if ((this->isOurEra && other.isOurEra) || (!this->isOurEra && !other.isOurEra))
            *this = this->subtract(other.year, other.month, other.day, other.hour, other.minute, other.second);
        else if ((this->isOurEra && !other.isOurEra) || (!this->isOurEra && other.isOurEra))
            *this = this->add(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return *this;
    }

    friend bool operator> (const Date& first, const Date& second)
    {
        bool res = true;

        if (first.isOurEra && second.isOurEra) {
            if (first.year < second.year) res = false;
            else if (first.year == second.year) {
                if (first.month < second.month) res = false;
                else if (first.month == second.month) {
                    if (first.day < second.day) res = false;
                    else if (first.day == second.day) {
                        if (first.hour < second.hour) res = false;
                        else if (first.hour == second.hour) {
                            if (first.minute < second.minute) res = false;
                            else if (first.minute == second.minute) {
                                if (first.second <= second.second) res = false;
                            }
                        }
                    }
                }
            }
        }

        else if (!first.isOurEra && !second.isOurEra) {
            if (first.year > second.year) res = false;
            else if (first.year == second.year) {
                if (first.month > second.month) res = false;
                else if (first.month == second.month) {
                    if (first.day > second.day) res = false;
                    else if (first.day == second.day) {
                        if (first.hour > second.hour) res = false;
                        else if (first.hour == second.hour) {
                            if (first.minute > second.minute) res = false;
                            else if (first.minute == second.minute) {
                                if (first.second >= second.second) res = false;
                            }
                        }
                    }
                }
            }
        }

        else if (!first.isOurEra && second.isOurEra) {
            res = false;
        }

        return res;
    }

    friend bool operator< (const Date& first, const Date& second)
    {
        bool res = true;

        if (!first.isOurEra && !second.isOurEra) {
            if (first.year < second.year) res = false;
            else if (first.year == second.year) {
                if (first.month < second.month) res = false;
                else if (first.month == second.month) {
                    if (first.day < second.day) res = false;
                    else if (first.day == second.day) {
                        if (first.hour < second.hour) res = false;
                        else if (first.hour == second.hour) {
                            if (first.minute < second.minute) res = false;
                            else if (first.minute == second.minute) {
                                if (first.second <= second.second) res = false;
                            }
                        }
                    }
                }
            }
        }

        else if (first.isOurEra && second.isOurEra) {
            if (first.year > second.year) res = false;
            else if (first.year == second.year) {
                if (first.month > second.month) res = false;
                else if (first.month == second.month) {
                    if (first.day > second.day) res = false;
                    else if (first.day == second.day) {
                        if (first.hour > second.hour) res = false;
                        else if (first.hour == second.hour) {
                            if (first.minute > second.minute) res = false;
                            else if (first.minute == second.minute) {
                                if (first.second >= second.second) res = false;
                            }
                        }
                    }
                }
            }
        }

        else if (first.isOurEra && !second.isOurEra) {
            res = false;
        }

        return res;
    }

    friend bool operator== (const Date& first, const Date& second)
    {
        bool res = false;

        if (first.isOurEra == second.isOurEra &&
            first.year == second.year &&
            first.month == second.month &&
            first.day == second.day &&
            first.hour == second.hour &&
            first.minute == second.minute &&
            first.second == second.second)
            res = true;

        return res;
    }
};

int main()
{

}
