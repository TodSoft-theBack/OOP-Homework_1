#include "MarkdownTableParser.h"

unsigned CountCharInFile(std::fstream& file, char symbol)
{
	size_t initialPosition = file.tellg();
	file.seekg(0, std::ios::beg);

	if (!file.is_open())
		return 0;

	unsigned int count = 0;

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

bool ContainsChar(char* string, char symbol) {
	size_t len = strlen(string);
	for (size_t i = 0; i < len; i++)
		if (string[i] == symbol)
			return true;
	return false;
}

MarkdownTableParser::MarkdownTableParser() 
{ 
	hasFormating = false;
}

MarkdownTableParser::MarkdownTableParser(const char* filename)
{
	hasFormating = false;
	LoadFile(filename);
}

bool MarkdownTableParser::IsLoaded() 
{
	return file.is_open();
}

FunctionStatus MarkdownTableParser::LoadFile(const char* filename)
{
	if (filename == nullptr)
		return FunctionStatus::InvalidInput;
	if (IsLoaded())
		file.close();
	file.open(filename, std::ios::in | std::ios::_Nocreate);
	if (!file)
		return FunctionStatus::InvalidInput;
	InitialiseTable();
	return FunctionStatus::Success;
}

FunctionStatus MarkdownTableParser::SaveToFile(const char* filename)
{
	if (filename == nullptr)
		return FunctionStatus::InvalidInput;
	file.open(filename, std::ios::out | std::ios::_Nocreate);
	if (!file)
		return FunctionStatus::InvalidInput;


	return FunctionStatus::Success;
}

size_t GetColumnCount(const char* string, char columnDelim)
{
	std::stringstream processor(string);
	processor.ignore();
	size_t columnCount = 0;
	const unsigned boundariedEmptySpacesCount = 2;
	char columnBuffer[MAX_VALUE_SIZE + boundariedEmptySpacesCount];
	do {
		processor.getline(columnBuffer, MAX_VALUE_SIZE + boundariedEmptySpacesCount, columnDelim);
		if (strcmp(columnBuffer, "") == 0)
			break;
		columnCount++;
	} while (true);
	return columnCount;
}

void MarkdownTableParser::ParseSetCurrentLine(char* line, unsigned rowIndex) {
	std::stringstream processor(line);
	processor.ignore();

	char columnBuffer[MAX_VALUE_SIZE];
	unsigned columnIndex = 0;
	do {
		processor >> columnBuffer;
		char delim[] = { TABLE_DELIM,'\0'};
		if (strcmp(columnBuffer, delim) == 0)
			continue;
		if (strcmp(columnBuffer, "") == 0)
			break;
		
		table.SetValueAt(rowIndex, columnIndex++, columnBuffer);
	} while (true);
}

void MarkdownTableParser::SetTable(char columnDelim)
{
	char buffer[256];
	unsigned rowIndex = 0;
	do
	{
		file.getline(buffer, 256);
		if (file.eof())
			break;
		if (rowIndex == FORMATTING_ROW_INDEX)
			if (ContainsChar(buffer, FORMATTING_CHAR))
				hasFormating = true;
		ParseSetCurrentLine(buffer, rowIndex++);
	} while (true);
}

void MarkdownTableParser::InitialiseTable()
{
	size_t rowsCount = CountCharInFile(file, '\n');
	char buffer[256];
	file.getline(buffer, 256);
	file.seekg(0, std::ios::beg);
	size_t columnCount = GetColumnCount(buffer, TABLE_DELIM);
	table = Table(rowsCount, columnCount);
	SetTable(TABLE_DELIM);
}

ColumnFormating MarkdownTableParser::FormatingAt(unsigned column) {
	if (!hasFormating)
		return ColumnFormating::Default;
	ColumnFormating formating = ColumnFormating::None;
	const char* formatString = table.ValueAt(FORMATTING_ROW_INDEX, column);
	size_t size = strlen(formatString);
	if (formatString[0] == FORMATTING_CHAR)
		formating = (ColumnFormating)((unsigned)formating | (unsigned)ColumnFormating::Left);
	if (formatString[size - 1] == FORMATTING_CHAR)
		formating = (ColumnFormating)((unsigned)formating | (unsigned)ColumnFormating::Right);

	return  formating == ColumnFormating::None ? ColumnFormating::Default : formating;
}

size_t MarkdownTableParser::GetLongestHeader() {
	size_t length = 0;
	for (size_t i = 0; i < table.ColumnCount(); i++)
	{
		size_t currentLen = strlen(table.ValueAt(HEADER_ROW_INDEX, i));
		if (currentLen > length)
			length = currentLen;
	}
	return length;
}

FunctionStatus MarkdownTableParser::Print(std::ostream& stream)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;

	if (hasFormating)
		PrintFormated(stream);
	else
		PrintNoFormat(stream);

	return FunctionStatus::Success;
}


void MarkdownTableParser::PrintNoFormat(std::ostream& stream)
{
	unsigned width = GetLongestHeader();

	for (size_t row = 0; row < table.RowCount(); row++)
	{
		if (row == FORMATTING_ROW_INDEX)
		{
			for (size_t column = 0; column < table.ColumnCount(); column++)
				stream << TABLE_DELIM << ' ' << std::setw(width) << std::left << std::setfill(FORMAT_ROW_FILL_CHAR) << ' ' << std::setfill(' ') << ' ';
			stream << TABLE_DELIM << std::endl;
			continue;
		}
		for (size_t column = 0; column < table.ColumnCount(); column++)
			stream << TABLE_DELIM << ' ' << std::setw(width) << std::left << table.ValueAt(row, column) << ' ';
		stream << TABLE_DELIM << std::endl;
	}
}

void MarkdownTableParser::PrintFormated(std::ostream& stream)
{
	unsigned width = GetLongestHeader();

	for (size_t row = 0; row < table.RowCount(); row++)
	{
		for (size_t column = 0; column < table.ColumnCount(); column++)
		{
			ColumnFormating currentFormat = FormatingAt(column);
			switch (currentFormat)
			{
				case ColumnFormating::Left:
					stream << TABLE_DELIM << ' ' << std::setw(width) << std::left << table.ValueAt(row, column) << ' ';
					break;
				case ColumnFormating::Right:
					stream << TABLE_DELIM << ' ' << std::setw(width) << std::right << table.ValueAt(row, column) << ' ';
					break;
				case ColumnFormating::Center:
					stream << TABLE_DELIM << ' ';
					const char* value = table.ValueAt(row, column);
					size_t len = strlen(value);
					size_t emptySpaceCount = (width - len) / 2;
					for (size_t i = 0; i < emptySpaceCount; i++)
						stream << ' ';
					stream << value;
					for (size_t i = 0; i < emptySpaceCount; i++)
						stream << ' ';
					int spaceDiff = 2 * emptySpaceCount + len - width;
					if ( spaceDiff < 0)
						stream << ' ';							
					stream << ' ';
					
					break;
			}
			
		}
		stream << TABLE_DELIM << std::endl;
	}
}


MarkdownTableParser::~MarkdownTableParser()
{
	if(IsLoaded())
		file.close();
}
