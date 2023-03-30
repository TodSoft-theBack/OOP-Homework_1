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
		bool _isInitialised;
		void SetRowCount(size_t rowCount);
		void SetColumnCount(size_t columnCount);

	public:
		Table();
		Table(size_t, size_t);
		FunctionStatus AddRow(char** columns, size_t columnsCount);
		FunctionStatus SetValueAt(unsigned row, unsigned column, const char* value);
		const char* ValueAt(unsigned row, unsigned column) const;
		size_t RowCount() const;
		size_t ColumnCount() const;
		bool IsInitialised() const;
	~Table();
};

