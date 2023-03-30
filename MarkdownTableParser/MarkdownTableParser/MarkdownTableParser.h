#pragma once
#include "Table.h"
#include <iomanip>
#include <fstream>
#include <sstream>


const char TABLE_DELIM = '|';
const char SUBSTRING_DELIM = '\'';
const char FORMATING_CHAR = ':';
const char FORMAT_ROW_FILL_CHAR = '-';
const unsigned FORMATING_ROW_INDEX = 1;
const unsigned HEADER_ROW_INDEX = 0;
const size_t MAX_LINE_BUFFER_SIZE = 256;

unsigned CountCharInFile(std::fstream& file, char symbol);
unsigned CountCharInString(const char* string, char symbol);
void Trim(char* string);
bool ContainsChar(const char* string, char symbol);
char** Split(const char* string, size_t& count);
char** SubstringSplit(const char* string, size_t& count);
bool TryParse(const char* string, unsigned& result);
void Append(char buffer[MAX_LINE_BUFFER_SIZE], const char* value);
void HandleFunctionResult(FunctionStatus functionResult, const char* messageOnSuccess);
void FreeMemory(char** strings, size_t count);

class MarkdownTableParser
{
	private:
		std::fstream file;		
		Table table;
		bool rowMask[MAX_ROW_COUNT];
		bool IsLoaded();
		bool HasFormating();
		void SetTable();
		void InitialiseTable();
		void InitialiseRowMask();
		void ParseSetCurrentLine(char* line, unsigned rowIndex);
		size_t GetLongest(unsigned rowIndex);
		size_t GetLongestCell();
		ColumnFormating FormatingAt(unsigned column);
		void PrintNoFormat(std::ostream& stream, size_t width);
		void PrintFormated(std::ostream& stream, size_t width);
		int ContainsColumn(char* columnName);

	public:
		MarkdownTableParser();
		MarkdownTableParser(const char*);
		FunctionStatus LoadFile(const char* filename);	
		FunctionStatus SaveToFile(const char* filename);
		FunctionStatus Print(std::ostream& stream);
		FunctionStatus AddRow(char** columns, size_t columnsCount);
		FunctionStatus ChangeColumnName(char* columnName, char* newColumnName);
		FunctionStatus ChangeValue(unsigned row,  char* columnName, char* newValue);
		FunctionStatus ChangeValue(char* columnName, char* currentValue, char* newValue);
		FunctionStatus Select(char* columnName, char* value);
		
	~MarkdownTableParser();
};

