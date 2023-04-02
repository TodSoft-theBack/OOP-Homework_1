#include "Time.h"


Time::Time() : Time(0,0,0)
{

}

Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	SetSeconds(seconds);
	SetMinutes(minutes);
	SetHours(hours);
}

Time::Time(size_t seconds)
{
	SetSeconds(seconds % 60);
	seconds /= 60;
	SetMinutes(seconds % 60);
	seconds /= 60;
	SetHours(seconds);
}


uint8_t Time::Seconds() const
{
	return seconds;
}

uint8_t Time::Minutes() const
{
	return minutes;
}

uint8_t Time::Hours() const
{
	return hours;
}

void Time::SetHours(uint8_t hours)
{
	if (hours < 24)
		this->hours = hours;
	else
		this->hours = 0;
}

void Time::SetMinutes(uint8_t minutes)
{
	if (minutes < 60)
		this->minutes = minutes;
	this->minutes = 0;
}

void Time::SetSeconds(uint8_t seconds)
{
	if (seconds < 60)
		this->seconds = seconds;
	this->seconds = 0;
}

unsigned long Time::TotalSeconds() const
{
	return seconds + 60 * minutes + 3600 * hours;
}

void Time::Print(std::ostream& stream) const
{

}