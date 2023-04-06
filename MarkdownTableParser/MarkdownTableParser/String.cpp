#include "String.h"

void String::FreeMemberMemory()
{
	delete[] container;
	container = nullptr;
}

void String::CopyMemberData(const String& string)
{
	container = new char[string.capacity];
	strcpy_s(container, string.capacity, string.container);
	capacity = string.capacity;
	length = string.length;
}

String::String()
{
	container = new char[DEFAULT_BUFFER_SIZE];
	capacity = DEFAULT_BUFFER_SIZE;
	container[0] = '\0';
	length = 0;
}

String::String(size_t capacity)
{
	container = new char[capacity];
	this->capacity = capacity;
	container[0] = '\0';
	length = 0;
}

String::String(const char* string)
{
	size_t len = strlen(string) + 1;
	container = new char[len];
	strcpy_s(container, len, string);
	capacity = len;
	length = len - 1;
}

String::String(const char* string, size_t len)
{
	container = new char[len];
	strcpy_s(container, len, string);
	capacity = len;
	length = len;
}

String::String(const String& copy)
{
	CopyMemberData(copy);
}

String& String::operator=(const String& string)
{
	if (this != &string)
	{
		FreeMemberMemory();
		CopyMemberData(string);
	}
	return *this;
}

void String::Resize(size_t newCapacity)
{
	String copy(*this);
	FreeMemberMemory();
	this->container = new char[newCapacity];
	Memory_Copy(container, copy.container, copy.length);
	capacity = newCapacity;
	length = copy.length;
}

char String::operator[](unsigned index)const
{
	return container[index];
}

void String::SetAtIndex(unsigned index, char value)
{
	container[index] = value;
}

void String::InsertChar(const char symbol)
{
	if (length + 1  < capacity)
	{
		container[length] = symbol;
		container[length + 1] = '\0';
		length++;
		return;
	}
	Resize(capacity * 2);
	container[length] = symbol;
	container[length + 1] = '\0';
	length++;
}

int String::CompareWith(const char* string) const
{
	return strcmp(container, string);
}

int String::CompareWith(const String& string) const
{
	return strcmp(container, string.container);
}

const char* String::Value() const
{
	return container;
}

char String::First() const
{
	return container[0];
}

char String::Last() const
{
	return container[length - 1];
}

size_t String::Length() const
{
	return length;
}

bool String::IsEmpty() const
{
	return length == 0;
}

bool String::ContainsChar(char symbol) const
{
	for (size_t i = 0; i < length; i++)
		if (container[i] == symbol)
			return true;
	return false;
}

unsigned String::CountCharInString(const char* string, char symbol)
{
	size_t len = strlen(string);
	unsigned int count = 0;
	for (size_t i = 0; i < len; i++)
		if (string[i] == symbol)
			count++;
	return count;
}

unsigned String::CountCharInString(char symbol) const
{
	unsigned int count = 0;
	for (size_t i = 0; i < length; i++)
		if (container[i] == symbol)
			count++;
	return count;
}

void String::Trim(char * string)
{
	size_t len = strlen(string);
	size_t beginIndex = 0;
	int endIndex = len - 1;
	for (; beginIndex < len && string[beginIndex] == ' '; beginIndex++);
	for (; endIndex >= 0 && string[endIndex] == ' '; endIndex--);
	if (beginIndex != 0)
		for (size_t i = beginIndex; i <= endIndex; i++)
			string[i - beginIndex] = string[i];
	string[endIndex - beginIndex + 1] = '\0';
}

void String::Trim(String& string)
{
	size_t beginIndex = 0;
	int endIndex = string.Length() - 1;
	for (; beginIndex < string.Length() && string[beginIndex] == ' '; beginIndex++);
	for (; endIndex >= 0 && string[endIndex] == ' '; endIndex--);
	if (beginIndex != 0)
		for (size_t i = beginIndex; i <= endIndex; i++)
			string.SetAtIndex(i - beginIndex, string[i]);
	string.SetAtIndex(endIndex - beginIndex + 1,  '\0');
}

bool String::TryParse(const char* string, unsigned& result)
{
	if (string == nullptr || string[0] == '\0')
		return false;
	result = 0;
	size_t len = strlen(string);
	for (size_t i = 0; i < len; i++)
	{
		if (string[i] < '0' || string[i] > '9')
			return false;
		(result *= 10) += string[i] - '0';
	}
	return true;
}

bool String::TryParse(unsigned& result) const
{
	return String::TryParse(container, result);
}

std::ostream& operator<<(std::ostream& stream, const String& string)
{
	stream << string.container;
	return stream;
}

String::~String()
{
	FreeMemberMemory();
}
