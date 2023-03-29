#pragma once
#include <iostream>
#include "ProjectEnums.h"

const size_t MAX_ROW_COUNT = 50;
const size_t MAX_COLUMN_COUNT = 10;
const size_t MAX_VALUE_SIZE = 20;
const char DEFAULT_VALUE[] = "< default >";
class Table
{
	private:
		char _values[MAX_ROW_COUNT][MAX_COLUMN_COUNT][MAX_VALUE_SIZE];
		size_t _rowCount;
		size_t _columnCount;

		void SetRowCount(size_t);
		void SetColumnCount(size_t);

	public:
		Table();
		Table(size_t, size_t);
		FunctionStatus AddRow(char**, size_t);
		FunctionStatus SetValueAt(unsigned, unsigned, const char*);
		const char* ValueAt(unsigned, unsigned);
		size_t RowCount();
		size_t ColumnCount();
		~Table();
};

