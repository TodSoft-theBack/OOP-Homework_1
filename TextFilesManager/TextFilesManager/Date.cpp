#include "Date.h"

bool Date::IsLeapYear() const
{
	return (year % 4) == 0;
}

Date::Date(uint8_t day, uint8_t month, unsigned year)
{
	SetYear(year);
	SetMonth(month);
	SetDay(day);
}

uint8_t Date::Day() const
{
	return day;
}

uint8_t Date::Month() const
{
	return month;
}

unsigned Date::Year() const
{
	return year;
}

void Date::SetDay(uint8_t day)
{

}

void Date::SetMonth(uint8_t month)
{

}

void Date::SetYear(unsigned year)
{

}

int CompareDates(const Date& f, const Date& s)
{
	if (f.year - s.year != 0)
		return f.year - s.year;
	else if (f.month - s.month != 0)
		return f.month - s.month;
	else if (f.day - s.day != 0)
		return f.day - s.day;
	return  0;
}

void Date::Print(std::ostream& stream) const
{

}