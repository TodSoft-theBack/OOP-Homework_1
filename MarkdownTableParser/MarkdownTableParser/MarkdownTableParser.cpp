#include "MarkdownTableParser.h"

bool MarkdownTableParser::IsLoaded() 
{
	return file.is_open();
}

FunctionStatus MarkdownTableParser::LoadFile(const String& filename)
{
	if (filename.Length() == 0)
		return FunctionStatus::InvalidInput;
	if (IsLoaded())
		file.close();
	file.open(filename.Value(), std::ios::in | std::ios::_Nocreate);
	if (!file)
		return FunctionStatus::InvalidInput;
	return InitialiseTable();
}

FunctionStatus MarkdownTableParser::SaveToFile(const String& filename)
{
	if (filename.Length() == 0)
		return FunctionStatus::InvalidInput;
	if (IsLoaded())
		file.close();
	if (!table.IsInitialised())
		return FunctionStatus::ExecutionFailed;
	file.open(filename.Value(), std::ios::out | std::ios::_Noreplace);
	if (!file)
		return FunctionStatus::InvalidInput;
	table.PrintToStream(file);
	file.flush();
	file.close();
	return FunctionStatus::Success;
}

size_t MarkdownTableParser::GetColumnCount(const char* string) const
{
	return String::CountCharInString(string, TABLE_DELIM) - 1;
}

FunctionStatus MarkdownTableParser::InitialiseTable()
{
	size_t rowsCount = CountCharInFile(file, '\n');
	char buffer[MAX_BUFFER];
	file.getline(buffer, MAX_BUFFER);
	file.seekg(0, std::ios::beg);
	size_t columnCount = GetColumnCount(buffer);
	table = Table(rowsCount, columnCount, file);
	return FunctionStatus::Success;
}

FunctionStatus MarkdownTableParser::Print(const String* arguments, size_t count)
{
	return table.PrintToStream(std::cout);
}

FunctionStatus MarkdownTableParser::Load(const String* arguments, size_t count)
{
	if (count != 1)
		return FunctionStatus::InvalidInput;
	return LoadFile(arguments[0]);
}

FunctionStatus MarkdownTableParser::AddRow(const String* arguments, size_t count)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	return table.AddRow(arguments, count);
}

FunctionStatus MarkdownTableParser::ChangeColumnName(const String* arguments, size_t count)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	return table.ChangeColumnName(arguments, count);
}

FunctionStatus MarkdownTableParser::ChangeValue(const String* arguments, size_t count)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	return table.ChangeValue(arguments, count);
}

FunctionStatus MarkdownTableParser::Select(const String* arguments, size_t count)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	return table.Select(arguments, count);
}

FunctionStatus MarkdownTableParser::SaveToFile(const String* arguments, size_t count)
{
	if (!IsLoaded())
		return FunctionStatus::ExecutionFailed;
	if (count != 1)
		return FunctionStatus::InvalidInput;
	return SaveToFile(arguments[0]);
}

FunctionStatus MarkdownTableParser::ExecuteAt(int index, const String* arguments, size_t count)
{
	if (index < 0)
		return FunctionStatus::InvalidInput;
	if (!IsLoaded() && index != 0)
		return FunctionStatus::ExecutionFailed;
	return (this->*Operations[index])(arguments, count);
}


MarkdownTableParser::~MarkdownTableParser()
{
	if(IsLoaded())
		file.close();
}
