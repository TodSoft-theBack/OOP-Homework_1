#include "GenericFunctons.h"

//used internet for this I had to because I cannnot use for loop to copy memory in general fucntions
//I decided to implement my own memcpy() although I think using the embeded would be better but we have not studied it yet
void* Memory_Copy(void* destination, void* source, unsigned size)
{
	// uint8_t = 1 Byte of memory

	uint8_t* destinationInBytesPtr = (uint8_t*)destination;
	uint8_t* sourceInBytesPtr = (uint8_t*)source;

	for (size_t i = 0; i < size; i++)
		destinationInBytesPtr[i] = sourceInBytesPtr[i];

	return destination;
}

unsigned CountCharInFile(std::fstream& file, char symbol)
{
	//Взето от гитхъб (от лекции)
	size_t initialPosition = file.tellg();
	file.seekg(0, std::ios::beg);

	if (!file.is_open())
		return 0;

	unsigned count = 0;

	while (true)
	{
		char current = file.get();
		if (file.eof())
			break;

		if (current == symbol)
			count++;
	}

	file.clear();
	file.seekg(initialPosition);
	return count;
}


//char** Split(const char* string, size_t& count, char delim)
//{
//	if (string == nullptr)
//		return nullptr;
//
//	count = CountCharInString(string, delim) + 1;
//	char** output = new char* [count];
//
//	size_t spaceIndex = 0;
//	for (size_t stringIndex = 0, argumentIterator = 0; argumentIterator < count; stringIndex++)
//		if (string[stringIndex] == delim || string[stringIndex] == '\0')
//		{
//			if (spaceIndex > stringIndex)
//				return nullptr;
//			size_t len = stringIndex - spaceIndex;
//			output[argumentIterator] = new char[len + 1];
//			for (size_t i = spaceIndex; i < stringIndex; i++)
//				output[argumentIterator][i - spaceIndex] = string[i];
//			output[argumentIterator][stringIndex - spaceIndex] = '\0';
//			spaceIndex = stringIndex + 1;
//			argumentIterator++;
//		}
//	return output;
//}

void Append(char result[MAX_BUFFER], const char* value)
{
	int inserIndex = -1;
	size_t len = strlen(value);
	for (size_t i = 0; i < MAX_BUFFER; i++)
	{
		if (result[i] == '\0')
			if (i + len + 1 < MAX_BUFFER)
			{
				inserIndex = i;
				break;
			}
	}
	if (inserIndex >= 0)
	{
		for (size_t i = 0; i < len; i++)
			result[inserIndex + i] = value[i];
		result[inserIndex + len] = '\0';
	}
}

