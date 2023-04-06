#pragma once
#include "Table.h"
#include "GenericFunctons.h"
#include <fstream>
#include <sstream>

const size_t MAX_FUNCTION_COUNT = 7;

class MarkdownTableParser
{
	private:

		FunctionStatus(MarkdownTableParser::* Operations[MAX_FUNCTION_COUNT])(const String*, size_t) = 
		{ 
			&MarkdownTableParser::Load,
			&MarkdownTableParser::Print,
			&MarkdownTableParser::AddRow,
			&MarkdownTableParser::ChangeColumnName,
			&MarkdownTableParser::ChangeValue,
			&MarkdownTableParser::Select,
			&MarkdownTableParser::SaveToFile,
		};

		std::fstream file;	
		Table table;	
		bool IsLoaded();
		FunctionStatus InitialiseTable();
		FunctionStatus LoadFile(const String& filename);
		FunctionStatus SaveToFile(const String& filename);
		size_t GetColumnCount(const char* string) const;
		FunctionStatus Load(const String* arguments, size_t count);
		FunctionStatus Print(const String* arguments, size_t count);
		FunctionStatus AddRow(const String* arguments, size_t count);
		FunctionStatus ChangeColumnName(const String* arguments, size_t count);
		FunctionStatus ChangeValue(const String* arguments, size_t count);
		FunctionStatus Select(const String* arguments, size_t count);
		FunctionStatus SaveToFile(const String* arguments, size_t count);

	public:
		FunctionStatus ExecuteAt(int index, const String* arguments, size_t count);

		
	~MarkdownTableParser();
};

