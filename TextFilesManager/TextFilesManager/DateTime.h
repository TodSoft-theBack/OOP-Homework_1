#pragma once
#include <iostream>
#include "Date.h"
#include "Time.h"

class DateTime
{
	private:
		Time time;
		Date date;
		
		friend int CompareDateTimes(const DateTime& f, const DateTime& s);


	public:
		DateTime();
		DateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year);
		void Print(std::ostream& stream) const;
};

int CompareDateTimes(const DateTime& f, const DateTime& s);
