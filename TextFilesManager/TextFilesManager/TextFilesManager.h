#pragma once
#include <iostream>
#include "TextFile.h"

enum class SortOptions 
{
	Name,
	TimeCreated,
	TimeLastModified,
	Size,
	Default = Name
};

enum class User 
{
	Owner, 
	GroupOwner,
	WhoTfIsThis, 
	Default = WhoTfIsThis
};

enum class Operation
{
	Empty,
	Read,
	Write,
	Execute
};

//Selection sort functions in was inspired by Angeld55's github
void Swap(TextFile& a, TextFile& b);
int MinIndexOf(const TextFile* arr, size_t size, SortOptions options);
void Sort(TextFile* arr, size_t size, SortOptions options);

class TextFilesManager
{
	private:
		TextFile* textfiles;
		size_t currentCount;
		size_t capacity;
		void CopyMemberData(const TextFilesManager& textFileManager);
		void FreeMemberMemory();
		int FindFile(const char* filename);
		bool HasUserRights(Operation operation, User user, const char rights[RIGHTS_SIZE]);

	public:
		TextFilesManager(size_t capacity);
		TextFilesManager(const TextFilesManager& copy);
		TextFilesManager& operator=(const TextFilesManager& textFileManager);
		void AddFile(const char* filename, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year,const char rights[RIGHTS_SIZE]);
		void EditFile(const char* filename, const char* newContent, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year, User user);
		void AppendToFile(const char* filename, const char* initialContent, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year, User user);
		void FileInfo(const char* filename);
		void PrintFile(const char* filename, User user);
		void SortFiles(SortOptions options);
		void Print();
		void DeleteFile(const char* filename);

	~TextFilesManager();
};

