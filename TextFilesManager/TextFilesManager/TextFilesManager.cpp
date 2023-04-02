#include "TextFilesManager.h"

//array of function pointers inspired by c++ documentation & my creativity
int (*Compare[4])(const TextFile& f, const TextFile& s) { CompareFilename, CompareTimeCreated, CompareTimeLastModified, CompareFileSize };

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

void Swap(TextFile& a, TextFile& b)
{
	TextFile temp = a;
	a = b;
	b = temp;
}

int MinIndexOf(const TextFile* array, size_t size, SortOptions options)
{
	int minIndex = 0;
	for (int i = 1; i < size; i++)
	{
		//we use the enum as an index because its values go from 0 to 3 by default 
		//So if we ordered our func pointer array correctly this automaticlly decides which compare fucntion to call
		if ((*Compare[(int)options])(array[i] , array[minIndex]) < 0)
			minIndex = i;
	}
	return minIndex;
}

//using selection sort because it minimises the swaps needed to sort the array
void Sort(TextFile* array, size_t size, SortOptions options)
{
	for (int i = 0; i < size - 1; i++)
	{
		int minIndex = i + MinIndexOf(array + i, size - i, options);

		if (minIndex != i)
			Swap(array[minIndex], array[i]);
	}
}


void TextFilesManager::CopyMemberData(const TextFilesManager& textFileManager)
{
	FreeMemberMemory();
	this->capacity = textFileManager.capacity;
	this->currentCount = textFileManager.currentCount;
	this->textfiles = new TextFile[textFileManager.capacity];
	Memory_Copy(textfiles, textFileManager.textfiles, sizeof(textFileManager.textfiles));
}

void TextFilesManager::FreeMemberMemory()
{
	delete[] textfiles;
}

int TextFilesManager::FindFile(const char* filename)
{
	int index = -1;
	for (size_t i = 0; i < currentCount; i++)
		if (strcmp(textfiles[i].Filename(), filename) == 0)
			return i;
	return index;
}

bool TextFilesManager::HasUserRights(Operation operation, User user, const char rights[RIGHTS_SIZE])
{
	unsigned index = 3 * (unsigned)user + (unsigned)operation;
	return rights[index] != '-';
}

TextFilesManager::TextFilesManager(size_t capacity)
{
	this->capacity = capacity;
	textfiles = new TextFile[capacity];
	currentCount = 0;
}

TextFilesManager::TextFilesManager(const TextFilesManager& copy)
{
	CopyMemberData(copy);
}

TextFilesManager& TextFilesManager::operator=(const TextFilesManager& textFileManager)
{
	if (this != &textFileManager)
	{
		FreeMemberMemory();
		CopyMemberData(textFileManager);
	}
	return *this;
}

void TextFilesManager::AddFile(const char* filename, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year, const char rights[RIGHTS_SIZE])
{
	if (filename == nullptr)
		return;

	if (currentCount + 1 < capacity)
		textfiles[currentCount++] = TextFile(filename, hours, minutes, seconds, day, month, year, rights);
}

void TextFilesManager::EditFile(const char* filename, const char* newContent, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year, User user)
{
	if (filename == nullptr)
		return;
	int index = FindFile(filename);
	if (index < 0)
		return;

	if (!HasUserRights(Operation::Write, user, textfiles[index].Rights()))
		return; // doesn't have rights

	DateTime current(hours, minutes, seconds, day, month, year);
	textfiles[index].Edit(newContent, current);
}

void TextFilesManager::AppendToFile(const char* filename, const char* content, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year, User user)
{
	if (filename == nullptr)
		return;
	int index = FindFile(filename);
	if (index < 0)
		return;

	if (!HasUserRights(Operation::Write, user, textfiles[index].Rights()))
		return; // doesn't have rights

	DateTime current(hours, minutes, seconds, day, month, year);
	textfiles[index].Append(content, current);
}

void TextFilesManager::FileInfo(const char* filename)
{
	if (filename == nullptr)
		return;

	int index = FindFile(filename);
	if (index < 0)
		return;

	textfiles[index].FileInfo(std::cout);
}

void TextFilesManager::PrintFile(const char* filename, User user)
{
	if (filename == nullptr)
		return;

	int index = FindFile(filename);
	if (index < 0)
		return;

	if (!HasUserRights(Operation::Write, user, textfiles[index].Rights()))
		return; // doesn't have rights

}

void TextFilesManager::SortFiles(SortOptions options)
{
	Sort(this->textfiles, this->currentCount, options);
}

void TextFilesManager::Print()
{
	for (size_t i = 0; i < currentCount; i++)
		textfiles[i].PrintToStream(std::cout);
}

void TextFilesManager::DeleteFile(const char* filename)
{
	int index = FindFile(filename);
	if (index < 0)
		return;

	delete &textfiles[index];

	for (size_t i = index; i < currentCount - 1; i++)
	{
		textfiles[i] = textfiles[i + 1];
	}
}

TextFilesManager::~TextFilesManager()
{
	FreeMemberMemory();
}
