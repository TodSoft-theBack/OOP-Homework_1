#include <iostream>
#include "MarkdownTableParser.h"

const size_t MAX_BUFFER = 256;
const size_t COMMAND_BUFFER = MAX_BUFFER / 8;
const size_t ARGUMENT_BUFFER = MAX_BUFFER - COMMAND_BUFFER;
const size_t ARGUMENT_WITH_MODS_BUFFER = MAX_BUFFER - 2 * COMMAND_BUFFER;
const size_t MAX_ARGUMENT_COUNT = 10;

int main()
{
	std::cout << "Starting MarkdownTableParser ..." << std::endl;
    MarkdownTableParser tableParser;
	
	char line[MAX_BUFFER];
	char command[COMMAND_BUFFER];
	do
	{
		std::cout << std::endl << "@Table parser > ";
		std::cin.getline(line, MAX_BUFFER);

		std::stringstream processor(line);
		processor.getline(command, COMMAND_BUFFER, ' ');

		if (strcmp(command, "Load") == 0)
		{
			char argument[ARGUMENT_BUFFER];
			processor.getline(argument, ARGUMENT_BUFFER);

			FunctionStatus functionResult = tableParser.LoadFile(argument);
			HandleFunctionResult(functionResult, "Successfully loaded a file");
		}
		else if (strcmp(command, "Print") == 0)
		{
			FunctionStatus functionResult = tableParser.Print(std::cout);
			HandleFunctionResult(functionResult, nullptr);
		}
		else if (strcmp(command, "Add") == 0)
		{
			char commandModifier[COMMAND_BUFFER];
			processor.getline(commandModifier, COMMAND_BUFFER, ' ');

			if (strcmp(commandModifier, "Row") == 0)
			{
				char argumentData[ARGUMENT_WITH_MODS_BUFFER];
				processor.getline(argumentData, ARGUMENT_WITH_MODS_BUFFER);
				Trim(argumentData);

				char** arguments;
				size_t count = 0;
				if (ContainsChar(argumentData, SUBSTRING_DELIM))
					arguments = SubstringSplit(argumentData, count);
				else
					arguments = Split(argumentData, count);

				FunctionStatus functionResult = tableParser.AddRow(arguments, count);
				HandleFunctionResult(functionResult, "Successfully added a row!");

				FreeMemory(arguments, count);
			}
			else if (strcmp(commandModifier, "Column") == 0)
				std::cout << "Missing feature! Comming soon!" << std::endl;
			else
				std::cout << "No such modifier! Please, kindly read the documentaton!" << std::endl;
		}
		else if (strcmp(command, "Change") == 0)
		{
			char commandModifier[COMMAND_BUFFER];
			processor.getline(commandModifier, COMMAND_BUFFER, ' ');
			if (strcmp(commandModifier, "ColumnName") == 0)
			{
				char argumentData[ARGUMENT_WITH_MODS_BUFFER];
				processor.getline(argumentData, ARGUMENT_WITH_MODS_BUFFER);
				Trim(argumentData);
				char** arguments;
				size_t count = 0;

				if (ContainsChar(argumentData, SUBSTRING_DELIM))
					arguments = SubstringSplit(argumentData, count);
				else
					arguments = Split(argumentData, count);

				if (count != 2)
				{
					std::cout << "Invalid argumnets!" << std::endl;
					continue;
				}
				

				FunctionStatus functionResult = tableParser.ChangeColumnName(arguments[0], arguments[1]);

				char message[MAX_BUFFER];
				message[0] = '\0';
				Append(message, "Successfully changed ");
				Append(message, arguments[0]);
				Append(message, " to ");
				Append(message, arguments[1]);

				HandleFunctionResult(functionResult, message);

				FreeMemory(arguments, count);
			}
			else if (strcmp(commandModifier, "Value") == 0)
			{
				char argumentData[ARGUMENT_WITH_MODS_BUFFER];
				processor.getline(argumentData, ARGUMENT_WITH_MODS_BUFFER);
				Trim(argumentData);

				size_t count = 0;
				char** arguments = Split(argumentData, count);
				if (count != 3)
				{
					std::cout << "Invalid argumnets!" << std::endl;
					continue;
				}
				char message[MAX_BUFFER];
				message[0] = '\0';
				FunctionStatus functionResult;
				unsigned rowIndex;
				if (TryParse(arguments[0], rowIndex))
				{
					functionResult = tableParser.ChangeValue(rowIndex, arguments[1], arguments[2]);
					Append(message, "Successfully changed at row: ");
					Append(message, arguments[0]);
					Append(message, " column: ");
					Append(message, arguments[1]);
					Append(message, " to ");
					Append(message, arguments[2]);
				}
				else
				{
					functionResult = tableParser.ChangeValue(arguments[0], arguments[1], arguments[2]);
					
					Append(message, "Successfully changed ");
					Append(message, arguments[1]);
					Append(message, " to ");
					Append(message, arguments[2]);
				}

				HandleFunctionResult(functionResult, message);
				FreeMemory(arguments, count);
			}
			else
				std::cout << "No such modifier! Please, kindly read the documentaton!" << std::endl;
		}
		else if (strcmp(command, "Select") == 0)
		{
			char argumentData[ARGUMENT_BUFFER];
			processor.getline(argumentData, ARGUMENT_BUFFER);
			size_t count = 0;
			char** arguments;
			if (ContainsChar(argumentData, SUBSTRING_DELIM))
				arguments = SubstringSplit(argumentData, count);
			else
				arguments = Split(argumentData, count);

			if (count != 2)
			{
				std::cout << "Invalid argumnets!" << std::endl;
				continue;
			}
			FunctionStatus functionResult = tableParser.Select(arguments[0], arguments[1]);
			HandleFunctionResult(functionResult, "Selection successfull!");

			FreeMemory(arguments, count);
		}
		else if (strcmp(command, "Save") == 0)
		{
			char argument[ARGUMENT_BUFFER];
			processor.getline(argument, ARGUMENT_BUFFER);
			FunctionStatus functionResult = tableParser.SaveToFile(argument);
			HandleFunctionResult(functionResult, "File saved successfully!");
		}
		else if (strcmp(command, "\0") == 0)
			std::cout << "Please, donot spam empty commands! I have worked hard on this!" << std::endl;
		else 
			std::cout << "Unrecognised command!" << std::endl;
	} while (strcmp(command, "Quit") != 0);
	std::cout << std::endl << "Closing MarkdownTableParser ..." << std::endl;
}