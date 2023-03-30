#include "Table.h"

Table::Table() : Table(0, 0) 
{ 
	_isInitialised = false;
}

Table::Table(size_t rows, size_t columns)
{
	SetRowCount(rows);
	SetColumnCount(columns);
	size_t default_len = strlen(DEFAULT_VALUE) + 1;
	for (size_t row = 0; row < _rowCount; row++)
		for (size_t column = 0; column < _columnCount; column++)
			strcpy_s(_values[row][column], default_len, DEFAULT_VALUE);
	_isInitialised = true;
}

void Table::SetRowCount(size_t value)
{
	if (value <= MAX_ROW_COUNT)
		_rowCount = value;
	else
		_rowCount = MAX_ROW_COUNT;
}
void Table::SetColumnCount(size_t value)
{
	if (value <= MAX_COLUMN_COUNT)
		_columnCount = value;
	else
		_columnCount = MAX_COLUMN_COUNT;
}

FunctionStatus Table::AddRow(char* values[MAX_VALUE_SIZE], size_t columnCount)
{
	if (values == nullptr)
		return FunctionStatus::InvalidInput;
	if (columnCount == 0)
		return FunctionStatus::InvalidInput;
	if (_rowCount + 1 > MAX_ROW_COUNT)
		return FunctionStatus::InvalidInput;
	if (columnCount != _columnCount)
		return FunctionStatus::InvalidInput;
	size_t len = 0;
	for (size_t i = 0; i < columnCount; i++)
	{
		len = strlen(values[i]) + 1;
		if (len > MAX_VALUE_SIZE)
			return FunctionStatus::InvalidInput;
		if (len == 0)
			return FunctionStatus::InvalidInput;
		strcpy_s(_values[_rowCount][i], len, values[i]);
	}
	_rowCount++;
	return FunctionStatus::Success;
}

FunctionStatus Table::SetValueAt(unsigned row, unsigned column, const char* value)
{
	if (value == nullptr)
		return FunctionStatus::InvalidInput;
	size_t len = strlen(value) + 1;
	if (len > MAX_VALUE_SIZE)
		return FunctionStatus::InvalidInput;
	if (row > _rowCount)
		return FunctionStatus::InvalidInput;
	if (column > _columnCount)
		return FunctionStatus::InvalidInput;
	strcpy_s(_values[row][column], len, value);

	return FunctionStatus::Success;
}

size_t Table::RowCount() const
{
	return _rowCount;
}

size_t Table::ColumnCount() const
{
	return _columnCount;
}

bool Table::IsInitialised() const {
	return _isInitialised;
}

const char* Table::ValueAt(unsigned row, unsigned column) const
{
	return _values[row][column];
}

Table::~Table()
{
	//there are no dynamic resourse in this class so we do nothings
}