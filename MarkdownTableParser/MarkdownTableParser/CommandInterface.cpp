#include "CommandInterface.h"

const size_t COMMAND_BUFFER = MAX_BUFFER / 8;
const size_t ARGUMENT_BUFFER = MAX_BUFFER - COMMAND_BUFFER;
const size_t MAX_COMMANDS_COUNT = 6;
const size_t MAX_MODIFIERSxCOMANDS_COUNT = 7;
const size_t MAX_COMMAND_SIZE = 16;
const size_t MAX_ARGUMENT_COUNT = 5;
const char* COMMANDS[MAX_COMMAND_SIZE] = { "Load", "Print", "Add", "Select", "Save", "Change" };
const bool HAS_MODIFIERS[MAX_COMMANDS_COUNT] = { 0, 0, 1, 0, 0, 1 };
const int INDEX_BINDING[MAX_COMMANDS_COUNT] = { 0, 1, 2, 3, 4, 5 };
const char* MODIFIERSxCOMANDS[MAX_COMMAND_SIZE] = { "Load", "Print", "Row", "Select", "Save", "ColcumnName", "Value" };

CommandInterface::CommandInterface()
{
	Run();
}

void CommandInterface::Run()
{
	std::cout << "Starting MarkdownTableParser ..." << std::endl;

	char line[MAX_BUFFER];
	char command[COMMAND_BUFFER];
	do
	{
		std::cout << std::endl << "@Table parser > ";
		std::cin.getline(line, MAX_BUFFER);

		std::stringstream arguments(line);
		arguments.getline(command, COMMAND_BUFFER, ' ');
		ExecuteOperation(command, arguments);

	} while (strcmp(command, "Quit") != 0);
	std::cout << "Starting MarkdownTableParser ..." << std::endl;
}

void CommandInterface::ExecuteOperation(char* command, std::stringstream& processor)
{
	size_t count = 0;
	String* arguments;
	char message[MAX_BUFFER];
	message[0] = '\0';
	FunctionStatus functionResult = FunctionStatus::None;

	if (strcmp(command, "\0") == 0)
	{
		std::cout << "Please, donot spam empty commands! I have worked hard on this!" << std::endl;
		return;
	}

	int index = GetCommandIndex(command, processor);
	if (index < 0)
	{
		HandleFunctionResult(FunctionStatus::InvalidInput, message);
		return;
	}

	arguments = ExtractArguments(processor, count);

	functionResult = tableParser.ExecuteAt(index, arguments, count);
	HandleFunctionResult(functionResult, message);
	FreeMemory(arguments, count);
}

int CommandInterface::GetCommandIndex(const char* command, std::stringstream& processor)
{
	for (size_t i = 0; i < MAX_COMMANDS_COUNT; i++)
		if (strcmp(COMMANDS[i], command) == 0)
		{
			if (HAS_MODIFIERS[i])
				return GetModifierIndex(i, processor);
			return i;
		}

	return -1;
}

int CommandInterface::GetModifierIndex(size_t currentIndex, std::stringstream& processor) 
{
	int index = currentIndex;

	char modifier[COMMAND_BUFFER];

	processor.getline(modifier, COMMAND_BUFFER, ' ');

	if (modifier[0] == '\0')
		processor.getline(modifier, COMMAND_BUFFER, '\0');

	size_t upperBound = currentIndex + 1 < MAX_COMMANDS_COUNT ? INDEX_BINDING[currentIndex + 1] : MAX_MODIFIERSxCOMANDS_COUNT;

	for (size_t i = currentIndex; i < upperBound; i++)
		if (strcmp(modifier, MODIFIERSxCOMANDS[i]) == 0)
			return i;

	return -1;
}

String* CommandInterface::ExtractArguments(std::stringstream& processor, size_t& count)
{
	char argumentData[ARGUMENT_BUFFER];
	processor.getline(argumentData, ARGUMENT_BUFFER);
	String::Trim(argumentData);
	return SplitStringToArguments(argumentData, count);
}

String* CommandInterface::SplitStringToArguments(const char* string, size_t& argumentCount)
{
	argumentCount = String::CountCharInString(string, ' ') + 1;
	String* result = new String[argumentCount];
	unsigned index = 0;
	for (size_t i = 0; string[i] != '\0'; i++)
	{
		if (index >= argumentCount)
			return result;

		if (string[i] != ' ')
			result[index].InsertChar(string[i]);

		else
			index++;
	}
	return result;
}

void CommandInterface::HandleFunctionResult(FunctionStatus functionResult, const char* messageOnSuccess) const
{
	if (functionResult == FunctionStatus::Success)
	{
		if (messageOnSuccess != nullptr && messageOnSuccess[0] != '\0')
			std::cout << messageOnSuccess << std::endl;
	}
	else if (functionResult == FunctionStatus::InvalidInput)
		std::cout << "Invalid Arguments!" << std::endl;
	else if (functionResult == FunctionStatus::ExecutionFailed)
		std::cout << "File is not loaded!" << std::endl;
}

void CommandInterface::FreeMemory(String* strings, size_t count)
{
	delete[] strings;
}

CommandInterface::~CommandInterface()
{

}