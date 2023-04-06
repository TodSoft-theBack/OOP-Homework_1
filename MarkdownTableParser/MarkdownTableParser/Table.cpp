#include "Table.h"

Table::Table()
{ 
	_columnCount = _rowCount = 0;
	_isInitialised = false;
	InitialiseRowMask(false);
}

Table::Table(size_t rows, size_t columns, std::istream& stream)
{
	SetRowCount(rows);
	SetColumnCount(columns);
	size_t default_len = strlen(DEFAULT_VALUE) + 1;
	for (size_t row = 0; row < _rowCount; row++)
		for (size_t column = 0; column < _columnCount; column++)
			_values[row][column] = String(DEFAULT_VALUE, 12);
	LoadDataFromStream(stream);
	InitialiseRowMask(true);
	_isInitialised = true;
}

void Table::LoadCurrentLine(char* line, unsigned row)
{
	std::stringstream processor(line);
	processor.ignore();

	char columnBuffer[MAX_VALUE_SIZE];
	unsigned columnIndex = 0;
	do {
		processor.getline(columnBuffer, MAX_VALUE_SIZE, TABLE_DELIM);
		if (strcmp(columnBuffer, "") == 0)
			break;
		String::Trim(columnBuffer);
		SetValueAt(row, columnIndex++, columnBuffer);
	} while (true);
}

void Table::LoadDataFromStream(std::istream& stream)
{
	char buffer[MAX_BUFFER];
	unsigned rowIndex = 0;
	do
	{
		stream.getline(buffer, MAX_BUFFER);
		if (!stream)
			break;
		LoadCurrentLine(buffer, rowIndex++);
	} while (true);
}

void Table::InitialiseRowMask(bool value)
{
	if (!_isInitialised)
	{

	}
	for (size_t i = 0; i < _rowCount; i++)
		rowMask[i] = value;
}

bool Table::IsInitialised() 
{
	return _isInitialised;
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

FunctionStatus Table::SetValueAt(unsigned row, unsigned column, const String& value)
{
	if (row > _rowCount || column > _columnCount || value.Length() == 0)
		return FunctionStatus::InvalidInput;

	size_t len = value.Length() + 1;
	if (len > MAX_VALUE_SIZE)
		return FunctionStatus::InvalidInput;

	_values[row][column] = value;
	return FunctionStatus::Success;
}

bool Table::ValidateArguments(const String* argumnets, size_t count, size_t requiredCount) const 
{
	if (argumnets == nullptr || count != requiredCount)
		return false;
	for (size_t i = 0; i < count; i++)
	{
		size_t len = argumnets[i].Length();
		if (len + 1 > MAX_VALUE_SIZE || len == 0)
			return false;
	}
	return true;
}

bool Table::HasFormating() const
{
	for (size_t column = 0; column < _columnCount; column++)
		if (_values[FORMATING_ROW_INDEX][column].ContainsChar(FORMATING_CHAR))
			return true;
	return false;
}

bool Table::HasFormating(unsigned column) const
{
	return _values[FORMATING_ROW_INDEX][column].ContainsChar(FORMATING_CHAR);
}

size_t Table::GetColumnWidth(unsigned column) const {
	size_t length = 0;
	for (size_t row = 0; row < _rowCount; row++)
	{
		size_t currentLen = _values[row][column].Length();
		if (currentLen > length)
			length = currentLen;
	}
	return length;
}

ColumnFormating Table::FormatingAt(unsigned column) const {
	if (!HasFormating())
		return ColumnFormating::Default;
	ColumnFormating formating = ColumnFormating::None;
	if (_values[FORMATING_ROW_INDEX][column][0] == FORMATING_CHAR)
		formating = (ColumnFormating)((unsigned)formating | (unsigned)ColumnFormating::Left);
	if (_values[FORMATING_ROW_INDEX][column].Last() == FORMATING_CHAR)
		formating = (ColumnFormating)((unsigned)formating | (unsigned)ColumnFormating::Right);

	return  formating == ColumnFormating::None ? ColumnFormating::Default : formating;
}

void Table::PrintFormated(std::ostream& stream) const
{
	size_t widths[MAX_COLUMN_COUNT];
	for (size_t column = 0; column < _columnCount; column++)
		widths[column] = GetColumnWidth(column);

	for (size_t row = 0; row < _rowCount; row++)
		if (rowMask[row])
		{
			for (size_t column = 0; column < _columnCount; column++)
			{
				size_t len = _values[row][column].Length();
				ColumnFormating currentFormat = FormatingAt(column);
				switch (currentFormat)
				{
					case ColumnFormating::Left:				
						stream << TABLE_DELIM << ' ' << _values[row][column];
						for (size_t i = 0; i < widths[column] - len; i++)
							stream << ' ';
						stream << ' ';
						break;

					case ColumnFormating::Right:
						stream << TABLE_DELIM << ' ';
						for (size_t i = 0; i < widths[column] - len; i++)
							stream << ' ';
						stream << _values[row][column] << ' ';
						break;

					case ColumnFormating::Center:
						stream << TABLE_DELIM << ' ';
						size_t emptySpaceCount = (widths[column] - len) / 2;
						for (size_t i = 0; i < emptySpaceCount; i++)
							stream << ' ';
						stream << _values[row][column];
						for (size_t i = 0; i < emptySpaceCount; i++)
							stream << ' ';
						int spaceDiff = 2 * emptySpaceCount + len - widths[column];
						if (spaceDiff < 0)
							stream << ' ';
						stream << ' ';
						break;
				}
			}
			stream << TABLE_DELIM << std::endl;
		}
}

int Table::ContainsColumn(const String columnName) const
{
	if (columnName.Length() == 0)
		return -1;
	for (size_t i = 0; i < _columnCount; i++)
		if (_values[HEADER_ROW_INDEX][i].CompareWith(columnName) == 0)
			return i;
	return -1;
}

FunctionStatus Table::AddRow(const String* argumnets, size_t count)
{
	if (argumnets == nullptr)
		return FunctionStatus::InvalidInput;

	if (!ValidateArguments(argumnets, count, _columnCount))
		return FunctionStatus::InvalidInput;

	if (_rowCount + 1 > MAX_ROW_COUNT)
		return FunctionStatus::InvalidInput;

	size_t len = 0;
	for (size_t i = 0; i < count; i++)
	{
		len = argumnets[i].Length() + 1;
		if (len > MAX_VALUE_SIZE)
			return FunctionStatus::InvalidInput;
		if (len == 0)
			return FunctionStatus::InvalidInput;

		_values[_rowCount][i] = argumnets[i];
	}

	_rowCount++;
	return FunctionStatus::Success;
}

FunctionStatus Table::PrintToStream(std::ostream& stream) const
{
	if (!_isInitialised)
		return FunctionStatus::ExecutionFailed;
	PrintFormated(stream);
	return FunctionStatus::Success;
}

FunctionStatus Table::ChangeColumnName(const String* argumnets, size_t count)
{
	if (!_isInitialised)
		return FunctionStatus::ExecutionFailed;
	if (!ValidateArguments(argumnets, count, _columnCount))
		return FunctionStatus::InvalidInput;
	int columnIndex = ContainsColumn(argumnets[0]);
	if (columnIndex == -1)
		return FunctionStatus::InvalidInput;
	return SetValueAt(HEADER_ROW_INDEX, columnIndex, argumnets[1]);
}

FunctionStatus Table::ChangeValue(const String* argumnets, size_t count)
{
	if (!_isInitialised)
		return FunctionStatus::ExecutionFailed;

	if (!ValidateArguments(argumnets, count, 3))
		return FunctionStatus::InvalidInput;

	unsigned row = 0;
	if (argumnets[0].TryParse(row))
	{
		int columnIndex = ContainsColumn(argumnets[1]);
		if (columnIndex < 0)
			return FunctionStatus::InvalidInput;
		if (row < 1 || row > _rowCount - 1)
			return FunctionStatus::InvalidInput;
		return SetValueAt(row, columnIndex, argumnets[2]);
	}

	int columnIndex = ContainsColumn(argumnets[0]);
	if (columnIndex < 0)
		return FunctionStatus::InvalidInput;
	for (size_t row = 0; row < _rowCount; row++)
		if (_values[row][columnIndex].CompareWith(argumnets[1]) == 0)
			return SetValueAt(row, columnIndex, argumnets[2]);

	return FunctionStatus::Success;
}

FunctionStatus Table::Select(const String* argumnets, size_t count)
{
	if (!_isInitialised)
		return FunctionStatus::ExecutionFailed;

	ValidateArguments(argumnets, count, 2);

	int columnIndex = ContainsColumn(argumnets[0]);
	if (columnIndex == -1)
		return FunctionStatus::InvalidInput;

	for (size_t row = 0; row < _rowCount; row++)
		if (row != HEADER_ROW_INDEX && row != FORMATING_ROW_INDEX)
			rowMask[row] = _values[row][columnIndex].CompareWith(argumnets[1]) == 0;
	return FunctionStatus::Success;
}

Table::~Table()
{
	//there are no dynamic resourse in this class so we do nothings
}