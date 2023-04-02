#pragma once
#include <iostream>
#include "GenericFunctons.h"

class String
{
	static const size_t DEFAULT_BUFFER_SIZE = 32;

	private:
		char* container;
		size_t capacity;
		size_t length;
		void CopyMemberData(const String& string);
		void FreeMemberMemory();	
		void Resize(size_t newCapacity);
	
	public:
		String();
		String(size_t capacity);
		String(const char* string);
		String(const char* string, size_t len);
		String(const String& copy);
		String& operator=(const String& string);
		char operator[](unsigned index) const;
		void SetAtIndex(unsigned index, char value);
		void InsertChar(const char symbol);
		friend std::ostream& operator<<(std::ostream& stream, const String& string);
		int CompareWith(const char* string) const;
		int CompareWith(const String& string) const;
		char First() const;
		char Last() const;
		const char* Value() const;
		size_t Length() const;
		bool IsEmpty() const;
		bool ContainsChar(char symbol) const;
		static unsigned CountCharInString(const char* string, char symbol);
		unsigned CountCharInString(char symbol) const;
		static void Trim(char* string);	
		static void Trim(String& string);
		bool TryParse(unsigned& result) const;
		static bool TryParse(const char* string, unsigned& result);
		~String();
};

std::ostream& operator<<(std::ostream& stream, const String& string);