#include "TextFile.h"

int CompareFileSize(const TextFile& f, const TextFile& s)
{
	return f.fileSize - s.fileSize;
}

int CompareTimeCreated(const TextFile& f, const TextFile& s)
{
	return CompareDateTimes(f.timeCreated, s.timeCreated);
}

int CompareFilename(const TextFile& f, const TextFile& s)
{
	return strcmp(f.filename, s.filename);
}

int CompareTimeLastModified(const TextFile& f, const TextFile& s)
{
	return CompareDateTimes(f.timeLastModified, s.timeLastModified);
}


void TextFile::SetTimeCreated(const DateTime& time)
{
	this->timeCreated = time;
}

void TextFile::SetLastModified(const DateTime& time)
{
	this->timeLastModified = time;
}


TextFile::TextFile()
{
	fileSize = 0;
	timeCreated = DateTime();
	timeLastModified = DateTime();
}

TextFile::TextFile(const char* filename, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, unsigned year, const char rights[RIGHTS_SIZE])
{
	strcpy_s(this->filename, FILENAME_SIZE, filename);
	SetTimeCreated(DateTime(hours, minutes, seconds, day, month, year));
	SetLastModified(DateTime(hours, minutes, seconds, day, month, year));
	strcpy_s(this->rights, RIGHTS_SIZE, rights);
}

const char* TextFile::Filename() const
{
	return filename;
}

const char* TextFile::Rights() const
{
	return rights;
}

void TextFile::Edit(const char* newContent,const DateTime& currentTime)
{
	if (newContent == nullptr)
		return;
	size_t len = strlen(newContent) + 1;
	strcpy_s(content, len, newContent);
	SetLastModified(currentTime);
}

void TextFile::Append(const char* content,const DateTime& currentTime)
{
	if (content == nullptr)
		return;
	size_t len = strlen(content) + 1;

	if (len > FILE_CONTENT_SIZE)
		return;

	for (size_t i = fileSize; i < len; i++)
		this->content[i] = content[i - fileSize];

	SetLastModified(currentTime);
}

void TextFile::FileInfo(std::ostream& output) const
{
	output << "Filename: " << filename;
	output << "Filesize: " << fileSize;
	output << "Created on: ";

	output << "Last modified on: ";
	output << "Rights: " << rights;
}

void TextFile::PrintToStream(std::ostream& output) const
{
	output << content;
}