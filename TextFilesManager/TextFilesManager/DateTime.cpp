#include "DateTime.h"


DateTime::DateTime()
{
	time = Time();
	date = Date();
}

DateTime::DateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year)
{
	time = Time(hours, minutes, seconds);
	date = Date(day, month, year);
}

void DateTime::Print(std::ostream& stream) const
{
	date.Print(stream);
	stream << " at ";
	time.Print(stream);
	stream << "Oclock";
}

int CompareDateTimes(const DateTime& f, const DateTime& s)
{
	int res = CompareDates(f.date, s.date);
	if (res != 0)
		return res;
	return f.time.TotalSeconds() - s.time.TotalSeconds();
}