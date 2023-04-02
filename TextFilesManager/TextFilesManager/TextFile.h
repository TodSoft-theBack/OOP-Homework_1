#pragma once
#include <iostream>
#include "DateTime.h"

enum class FunctionResult 
{

};

const size_t FILENAME_SIZE = 64;
const size_t FILE_CONTENT_SIZE = 512;
const size_t RIGHTS_SIZE = 3 * 3 + 1; // 3 group with 3 chars each and 1 for \0
const char RIGHTS_CHARS[4] = { '-', 'r', 'w', 'e' };
const char DEFAULT_RIGHTS[] =
{
	RIGHTS_CHARS[1], RIGHTS_CHARS[2], RIGHTS_CHARS[3],
	RIGHTS_CHARS[1], RIGHTS_CHARS[0], RIGHTS_CHARS[3],
	RIGHTS_CHARS[1], RIGHTS_CHARS[0], RIGHTS_CHARS[0],
	'\0'
};

class TextFile
{
	private:
		//For all char[] max size is SIZE - 1 due to the sentinel at the end
		char filename[FILENAME_SIZE];
		char content[FILE_CONTENT_SIZE];
		size_t fileSize; //In bytes ( Content is in stored in a char array so filesize sould be just the length of the currently stored file because sizeof(char) = 1B)
		DateTime timeCreated;
		DateTime timeLastModified;
		char rights[RIGHTS_SIZE];
	
		void SetTimeCreated(const DateTime& time);
		void SetLastModified(const DateTime& time);
	
	
		friend int CompareFileSize(const TextFile& f, const TextFile& s);
		friend int CompareTimeCreated(const TextFile& f, const TextFile& s);
		friend int CompareTimeLastModified(const TextFile& f, const TextFile& s);
		friend int CompareFilename(const TextFile& f, const TextFile& s);
	
	public:
		TextFile();
		TextFile(const char* filename, uint8_t hours, uint8_t minute, uint8_t seconds, uint8_t day, uint8_t month, unsigned year, const char rights[RIGHTS_SIZE]);
		const char* Filename() const;
		const char* Rights() const;
		void Edit(const char* newContent, const DateTime& currentTime);
		void Append(const char* content, const DateTime& currentTime);
		void FileInfo(std::ostream& output) const;
		void PrintToStream(std::ostream& output) const;
};

int CompareFileSize(const TextFile& f, const TextFile& s);

int CompareTimeCreated(const TextFile& f, const TextFile& s);

int CompareFilename(const TextFile& f, const TextFile& s);

int CompareTimeLastModified(const TextFile& f, const TextFile& s);