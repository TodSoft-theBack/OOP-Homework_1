#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

//this file is like a configuratio file for the tipe of tables this project can parse

//My global constants 

enum class FunctionStatus
{
	None,
	InvalidInput,
	ExecutionFailed,
	Success
};

const size_t MAX_BUFFER = 256;

void* Memory_Copy(void* destination, void* source, unsigned size);
unsigned CountCharInFile(std::fstream& file, char symbol);
void Append(char buffer[MAX_BUFFER], const char* value);
