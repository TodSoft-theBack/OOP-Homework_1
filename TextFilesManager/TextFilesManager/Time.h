#pragma once
#include <iostream>

//compied from github
class Time
{
	private:
		uint8_t seconds; //[0...59]
		uint8_t minutes; //0...59
		uint8_t hours; // 0...23	

	public:
		Time(); // 00:00:00
		Time(uint8_t hours, uint8_t minutes, uint8_t seconds);
		Time(size_t seconds);


		uint8_t Seconds() const;
		uint8_t Minutes() const;
		uint8_t Hours() const;

		void SetSeconds(uint8_t seconds);
		void SetMinutes(uint8_t mins);
		void SetHours(uint8_t hours);

		unsigned long TotalSeconds() const;
		void Print(std::ostream& stream) const;
};

