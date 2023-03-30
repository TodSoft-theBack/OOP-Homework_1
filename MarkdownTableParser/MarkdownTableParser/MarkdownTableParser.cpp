#include "MarkdownTableParser.h"

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

unsigned CountCharInString(const char* string, char symbol)
{
	unsigned int count = 0;
	for (size_t i = 0; string[i] != '\0'; i++)
		if (string[i] == symbol)
			count++;
	return count;
}
unsigned CountSubstringInString(const char* string, char symbol)
{
	size_t len = strlen(string);
	bool isInQuotes = false;
	unsigned int count = 0;
	for (size_t i = 0; i < len; i++)
		if (string[i] == symbol && !isInQuotes)
			count++;
		else if (string[i] == SUBSTRING_DELIM)
			isInQuotes = !isInQuotes;
	return count;
}

void Trim(char* string) {
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

void Trim(char* string, char symbol) {
	size_t len = strlen(string);
	size_t beginIndex = 0;
	int endIndex = len - 1;
	for (; beginIndex < len && string[beginIndex] == symbol; beginIndex++);
	for (; endIndex >= 0 && string[endIndex] == symbol; endIndex--);
	if (beginIndex != 0)
		for (size_t i = beginIndex; i <= endIndex; i++)
			string[i - beginIndex] = string[i];
	string[endIndex - beginIndex + 1] = '\0';
}

bool ContainsChar(const char* string, char symbol) {
	size_t len = strlen(string);
	for (size_t i = 0; i < len; i++)
		if (string[i] == symbol)
			return true;
	return false;
}

char** Split(const char* string, size_t& count)
{
	if (string == nullptr)
		return nullptr;

	count = CountCharInString(string, ' ') + 1;
	char** output = new char*[count];

	size_t spaceIndex = 0;
	for (size_t stringIndex = 0, argumentIterator = 0 ; argumentIterator < count ; stringIndex++)
		if (string[stringIndex] == ' ' || string[stringIndex] == '\0')
		{
			if (spaceIndex > stringIndex)
				return nullptr;
			size_t len = stringIndex - spaceIndex;
			if (len > MAX_VALUE_SIZE)
				return nullptr;
			output[argumentIterator] = new char[len + 1];
			for (size_t i = spaceIndex; i < stringIndex; i++)
				output[argumentIterator][i - spaceIndex] = string[i];
			output[argumentIterator][stringIndex - spaceIndex] = '\0';
			spaceIndex = stringIndex + 1;
			argumentIterator++;
		}
	return output;
}

char** SubstringSplit(const char* string, size_t& count)
{
	if (string == nullptr)
		return nullptr;

	count = CountSubstringInString(string, ' ') + 1;
	char** output = new char* [count];
	size_t spaceIndex = 0;
	for (size_t stringIndex = 0, argumentIterator = 0; argumentIterator < count; stringIndex++)
		if (string[stringIndex] == ' ' || string[stringIndex] == '\0')
		{
			if (spaceIndex > stringIndex)
				return nullptr;
			size_t len = stringIndex - spaceIndex;
			if (len > MAX_VALUE_SIZE)
				return nullptr;
			output[argumentIterator] = new char[len + 1];
			for (size_t i = 0; i < len; i++)
			{
				output[argumentIterator][i] = string[i];
			}
			output[argumentIterator][stringIndex - spaceIndex] = '\0';
			spaceIndex = stringIndex + 1;
			argumentIterator++;
		}
	return output;
}

void Append(char result[MAX_LINE_BUFFER_SIZE], const char* value)
{
	int inserIndex = -1;
	size_t len = strlen(value);
	for (size_t i = 0; i < MAX_LINE_BUFFER_SIZE; i++)
	{
		if (result[i] == '\0')
			if (i + len + 1 < MAX_LINE_BUFFER_SIZE)
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

bool TryParse(const char* string, unsigned& result) 
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

void HandleFunctionResult(FunctionStatus functionResult, const char* messageOnSuccess)
{
	if (functionResult == FunctionStatus::Success)
	{
		if (messageOnSuccess != nullptr)
			std::cout << messageOnSuccess << std::endl;
	}
	else if (functionResult == FunctionStatus::InvalidInput)
		std::cout << "Invalid Arguments!" << std::endl;
	else if (functionResult == FunctionStatus::ExecutionFailed)
			std::cout << "File is not loaded!" << std::endl;
}

void FreeMemory(char** strings, size_t count) {
	for (size_t i = 0; i < count; i++)
		delete[] strings[i];
	delete[] strings;
}

MarkdownTableParser::MarkdownTableParser()
{ 
	//Empty because there is no need to do anything without a loaded file
}

MarkdownTableParser::MarkdownTableParser(const char* filename)
{
	LoadFile(filename);
}

bool MarkdownTableParser::IsLoaded() 
{
	return file.is_open();
}

bool MarkdownTableParser::HasFormating() 
{
	for (size_t column = 0; column < table.ColumnCount(); column++)
		if (ContainsChar(table.ValueAt(FORMATING_ROW_INDEX, column), FORMATING_CHAR))
			return true;
	return false;
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
	if (IsLoaded())
		file.close();
	if (!table.IsInitialised())
		return FunctionStatus::ExecutionFailed;
	file.open(filename, std::ios::out | std::ios::_Noreplace);
	if (!file)
		return FunctionStatus::InvalidInput;
	Print(file);
	file.flush();
	file.close();
	return FunctionStatus::Success;
}

size_t GetColumnCount(const char* string)
{
	return CountCharInString(string, TABLE_DELIM) - 1;
}

void MarkdownTableParser::ParseSetCurrentLine(char* line, unsigned rowIndex) {
	std::stringstream processor(line);
	processor.ignore();

	char columnBuffer[MAX_VALUE_SIZE];
	unsigned columnIndex = 0;
	do {
		processor.getline(columnBuffer, MAX_VALUE_SIZE, TABLE_DELIM);
		if (strcmp(columnBuffer, "") == 0)
			break;
		Trim(columnBuffer);
		table.SetValueAt(rowIndex, columnIndex++, columnBuffer);
	} while (true);
}

void MarkdownTableParser::SetTable()
{
	char buffer[MAX_LINE_BUFFER_SIZE];
	unsigned rowIndex = 0;
	do
	{
		file.getline(buffer, MAX_LINE_BUFFER_SIZE);
		if (file.eof())
			break;
		ParseSetCurrentLine(buffer, rowIndex++);
	} while (true);
}

void MarkdownTableParser::InitialiseRowMask() 
{
	for (size_t i = 0; i < table.RowCount(); i++)
		rowMask[i] = true;
}

void MarkdownTableParser::InitialiseTable()
{
	size_t rowsCount = CountCharInFile(file, '\n');
	char buffer[MAX_LINE_BUFFER_SIZE];
	file.getline(buffer, MAX_LINE_BUFFER_SIZE);
	file.seekg(0, std::ios::beg);
	size_t columnCount = GetColumnCount(buffer);
	table = Table(rowsCount, columnCount);
	InitialiseRowMask();
	SetTable();
}

ColumnFormating MarkdownTableParser::FormatingAt(unsigned column) {
	if (!HasFormating())
		return ColumnFormating::Default;
	ColumnFormating formating = ColumnFormating::None;
	const char* formatString = table.ValueAt(FORMATING_ROW_INDEX, column);
	size_t size = strlen(formatString);
	if (formatString[0] == FORMATING_CHAR)
		formating = (ColumnFormating)((unsigned)formating | (unsigned)ColumnFormating::Left);
	if (formatString[size - 1] == FORMATING_CHAR)
		formating = (ColumnFormating)((unsigned)formating | (unsigned)ColumnFormating::Right);

	return  formating == ColumnFormating::None ? ColumnFormating::Default : formating;
}

size_t MarkdownTableParser::GetLongest(unsigned rowIndex) {
	size_t length = 0;
	for (size_t i = 0; i < table.ColumnCount(); i++)
	{
		size_t currentLen = strlen(table.ValueAt(rowIndex, i));
		if (currentLen > length)
			length = currentLen;
	}
	return length;
}

size_t MarkdownTableParser::GetLongestCell() {
	size_t length = 0;
	for (size_t row = 0; row < table.RowCount(); row++)
	{
		size_t currentLen = GetLongest(row);
		if (currentLen > length)
			length = currentLen;
	}
	return length;
}

FunctionStatus MarkdownTableParser::Print(std::ostream& stream)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;

	size_t width = GetLongestCell();

	if (HasFormating())
		PrintFormated(stream, width);
	else
		PrintNoFormat(stream, width);

	return FunctionStatus::Success;
}

void MarkdownTableParser::PrintNoFormat(std::ostream& stream, size_t width)
{
	for (size_t row = 0; row < table.RowCount(); row++)
		if (rowMask[row])
		{
			for (size_t column = 0; column < table.ColumnCount(); column++)
				stream << TABLE_DELIM << ' ' << std::setw(width) << std::left << table.ValueAt(row, column) << ' ';
			stream << TABLE_DELIM << std::endl;
		}
}

void MarkdownTableParser::PrintFormated(std::ostream& stream, size_t width)
{
	for (size_t row = 0; row < table.RowCount(); row++)
		if (rowMask[row])
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
						if (spaceDiff < 0)
							stream << ' ';
						stream << ' ';
						break;
				}
			}
			stream << TABLE_DELIM << std::endl;
		}
}

int MarkdownTableParser::ContainsColumn(char* columnName)
{
	if (columnName == nullptr)
		return -1;
	for (size_t i = 0; i < table.ColumnCount(); i++)
		if (strcmp(table.ValueAt(HEADER_ROW_INDEX, i), columnName) == 0)
			return i;
	return -1;
}

FunctionStatus MarkdownTableParser::AddRow(char** columns, size_t columnsCount)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	FunctionStatus result = table.AddRow(columns, columnsCount);
	rowMask[table.RowCount() - 1] = true;	
	return result;
}

FunctionStatus MarkdownTableParser::ChangeColumnName(char* columnName, char* newColumnName)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	if (columnName == nullptr || newColumnName == nullptr)
		return FunctionStatus::InvalidInput;
	int columnIndex = ContainsColumn(columnName);
	if (columnIndex == -1)
		return FunctionStatus::InvalidInput;
	return table.SetValueAt(HEADER_ROW_INDEX, columnIndex, newColumnName);
}

FunctionStatus MarkdownTableParser::ChangeValue(unsigned row, char* columnName, char* newValue)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	if (columnName == nullptr || newValue == nullptr)
		return FunctionStatus::InvalidInput;
	int columnIndex = ContainsColumn(columnName);
	if (columnIndex < 0)
		return FunctionStatus::InvalidInput;
	if (row < 1 || row > table.RowCount() - 1)
		return FunctionStatus::InvalidInput;
	if (strlen(newValue) > MAX_VALUE_SIZE)
		return FunctionStatus::InvalidInput;
	return table.SetValueAt(row, columnIndex, newValue);
}

FunctionStatus MarkdownTableParser::ChangeValue(char* columnName, char* currentValue, char* newValue)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	if (columnName == nullptr || currentValue == nullptr || newValue == nullptr)
		return FunctionStatus::InvalidInput;
	int columnIndex = ContainsColumn(columnName);
	if (columnIndex == -1)
		return FunctionStatus::InvalidInput;
	if (strlen(currentValue) > MAX_VALUE_SIZE || strlen(newValue) > MAX_VALUE_SIZE)
		return FunctionStatus::InvalidInput;
	for (size_t row = 0; row < table.RowCount(); row++)
		if (strcmp(table.ValueAt(row, columnIndex), currentValue) == 0)
			return table.SetValueAt(row, columnIndex, newValue);
	return FunctionStatus::Success;
}

FunctionStatus MarkdownTableParser::Select(char* columnName, char* value)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	if (columnName == nullptr || value == nullptr)
		return FunctionStatus::InvalidInput;
	int columnIndex = ContainsColumn(columnName);
	if (columnIndex == -1)
		return FunctionStatus::InvalidInput;
	for (size_t row = 0; row < table.RowCount(); row++)
		if (row != HEADER_ROW_INDEX && row != FORMATING_ROW_INDEX)
			rowMask[row] = strcmp(table.ValueAt(row, columnIndex), value) == 0;
	return FunctionStatus::Success;
}

MarkdownTableParser::~MarkdownTableParser()
{
	if(IsLoaded())
		file.close();
}
