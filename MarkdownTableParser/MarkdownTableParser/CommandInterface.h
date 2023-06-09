#pragma once
#include "MarkdownTableParser.h"
#include <iostream>
#include <sstream>


class CommandInterface
{
	private:
		MarkdownTableParser tableParser;

		void ExecuteOperation(char* command, std::stringstream& processor);
		int GetCommandIndex(const char* command, std::stringstream& processor) const;
		int GetModifierIndex(size_t currentIndex, std::stringstream& processor) const;
		void GetSuccessMessage(char* message, int index, const String* arguments, size_t count) const;
		String* ExtractArguments(std::stringstream& processor, size_t& count) const;
		String* SplitStringToArguments(const char* string, size_t& argumentCount) const;
		void HandleFunctionResult(FunctionStatus functionResult, const char* messageOnSuccess) const;
		void FreeMemory(String* strings, size_t count);

	public:
		CommandInterface();
		void Run();

	~CommandInterface();
};

