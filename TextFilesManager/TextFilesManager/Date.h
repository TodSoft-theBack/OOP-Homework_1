#pragma once
#include <iostream>

//copied for github
class Date
{
	private:
		uint8_t MONTH_LENGTHS[12] =
		{
			31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
		};

		uint8_t day = 1;
		uint8_t month = 1;
		unsigned year = 1;
		mutable bool isModified = true;
		mutable int dayOfWeek = -1;
		bool IsLeapYear() const;
		friend int CompareDates(const Date& f, const Date& s);
	
	public:
		Date() = default;
		Date(uint8_t day, uint8_t month, unsigned year);
		uint8_t Day() const;
		uint8_t Month() const;
		unsigned Year() const;
	
		void SetDay(uint8_t day);
		void SetMonth(uint8_t month);
		void SetYear(unsigned year);
		void Print(std::ostream& stream) const;
};

int CompareDates(const Date& f, const Date& s);