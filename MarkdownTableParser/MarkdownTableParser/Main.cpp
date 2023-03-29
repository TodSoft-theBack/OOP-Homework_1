#include "MarkdownTableParser.h"
#include <sstream>

const size_t MAX_BUFFER = 256;
const size_t COMMAND_BUFFER = MAX_BUFFER / 8;
const size_t ARGUMENT_BUFFER = MAX_BUFFER - COMMAND_BUFFER;

int main()
{
	std::cout << "Starting MarkdownTableParser ..." << std::endl;
    MarkdownTableParser tableParser;
	char line[MAX_BUFFER];
	do
	{
		std::cout << "Table parser>  ";
		std::cin.getline(line, MAX_BUFFER);
		std::stringstream processor(line);
		char command[COMMAND_BUFFER];
		processor.getline(command, COMMAND_BUFFER, ' ');
		if (strcmp(command, "Load") == 0)
		{
			char argument[ARGUMENT_BUFFER];
			processor.getline(argument, ARGUMENT_BUFFER);
			FunctionStatus functionResult = tableParser.LoadFile(argument);
			if (functionResult == FunctionStatus::InvalidInput)
				std::cout << "Invalid file!" << std::endl;
			if (functionResult == FunctionStatus::Success)
				std::cout << "Sucessfully opened " << argument << std::endl;
		}
		else if (strcmp(command, "Print") == 0)
			tableParser.Print(std::cout);
		else if (strcmp(command, "AddRow") == 0)
		{

		}
		else if (strcmp(command, "Change") == 0)
		{

		}
		else if (strcmp(command, "Save") == 0)
		{
			char argument[ARGUMENT_BUFFER];
			processor.getline(argument, ARGUMENT_BUFFER);
			FunctionStatus functionResult = tableParser.SaveToFile(argument);
			if (functionResult == FunctionStatus::InvalidInput)
				std::cout << "Invalid filename!" << std::endl;
			if (functionResult == FunctionStatus::Success)
				std::cout << "Sucessfully saved to " << argument << std::endl;
		}

		
	} while (strcmp(line, "Quit") != 0);
	std::cout << "Closing MarkdownTableParser ..." << std::endl;
}