#pragma once
#include "Table.h"
#include <iomanip>
#include <fstream>
#include <sstream>

const char TABLE_DELIM = '|';
const char FORMATTING_CHAR = ':';
const char FORMAT_ROW_FILL_CHAR = '-';
const unsigned FORMATTING_ROW_INDEX = 1;
const unsigned HEADER_ROW_INDEX = 0;

class MarkdownTableParser
{
	private:
		std::fstream file;
		Table table;
		bool IsLoaded();
		bool hasFormating;
		void SetTable(char);
		void InitialiseTable();
		void ParseSetCurrentLine(char*, unsigned);
		size_t GetLongestHeader();
		ColumnFormating FormatingAt(unsigned);
		void PrintNoFormat(std::ostream& stream);
		void PrintFormated(std::ostream& stream);
	public:

		MarkdownTableParser();
		MarkdownTableParser(const char*);
		FunctionStatus LoadFile(const char*);	
		FunctionStatus SaveToFile(const char*);
		FunctionStatus Print(std::ostream& stream);
		
		~MarkdownTableParser();
};

