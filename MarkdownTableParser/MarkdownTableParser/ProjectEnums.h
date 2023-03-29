#pragma once
enum class FunctionStatus
{
	InvalidInput,
	ExecutionFailed,
	Success
};

enum class ColumnFormating
{
	None,
	Left,
	Right ,
	Center = Left | Right,
	Default = Left
};

