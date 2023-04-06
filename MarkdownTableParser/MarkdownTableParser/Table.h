#pragma once
#include "String.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "GenericFunctons.h"

const size_t MAX_ROW_COUNT = 50;
const size_t MAX_COLUMN_COUNT = 10;
const size_t MAX_VALUE_SIZE = 20;
const char DEFAULT_VALUE[] = "< default >";
const char TABLE_DELIM = '|';
const char SUBSTRING_DELIM = '\'';
const char FORMATING_CHAR = ':';
const char FORMAT_ROW_FILL_CHAR = '-';
const unsigned FORMATING_ROW_INDEX = 1;
const unsigned HEADER_ROW_INDEX = 0;

enum class ColumnFormating
{
	None,
	Left,
	Right,
	Center = Left | Right, //01 | 10 = 11
	Default = Left
};

class Table
{
	private:
		String _values[MAX_ROW_COUNT][MAX_COLUMN_COUNT];
		bool rowMask[MAX_ROW_COUNT];
		size_t _rowCount;
		size_t _columnCount;
		bool _isInitialised;
		void SetRowCount(size_t rowCount);
		void SetColumnCount(size_t columnCount);
		FunctionStatus SetValueAt(unsigned row, unsigned column, const String& value);
		bool ValidateArguments(const String* argumnets, size_t count, size_t requiredCount) const;
		bool HasFormating() const;
		bool HasFormating(unsigned column) const;
		size_t GetColumnWidth(unsigned column) const;
		ColumnFormating FormatingAt(unsigned column) const;
		void PrintFormated(std::ostream& stream) const;
		int ContainsColumn(const String columnName) const;
		void LoadCurrentLine(char* line, unsigned row);
		void LoadDataFromStream(std::istream& stream);
		void InitialiseRowMask(bool value);
	public:
		Table();
		Table(size_t, size_t, std::istream& stream);
		bool IsInitialised();
		FunctionStatus AddRow(const String* argumnets, size_t count);
		FunctionStatus PrintToStream(std::ostream& stream) const;
		FunctionStatus ChangeColumnName(const String* argumnets, size_t count);
		FunctionStatus ChangeValue(const String* argumnets, size_t count);
		FunctionStatus Select(const String* argumnets, size_t count);
	~Table();
};

