#include "CommandParser.hpp"

// DONE.
std::string CommandParser::ParseDatabaseActionCommand(char buffer[], int size)
{
    std::string command;

    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == '-')
        {
            break;
        }
        command += buffer[i];
    }

    return command;
}

// Dont need it now.
std::string CommandParser::ParseSenderName(char buffer[], int size)
{
    std::string actionCommand = this->ParseDatabaseActionCommand(buffer, size);

    std::vector<std::string> allCommands;

    if (actionCommand == "SEND")
    {
        allCommands = this->ParseAllCommand(buffer, size);
    }

    return allCommands[1];
}

// Done.
std::string CommandParser::ParseReceiverName(char buffer[], int size)
{
    std::string actionCommand = this->ParseDatabaseActionCommand(buffer, size);

    std::vector<std::string> allCommands;

    if (actionCommand == "LIST")
    {
        allCommands = this->ParseAllCommand(buffer, size);
        return allCommands[1];
    }

    if (actionCommand == "DEL")
    {
        allCommands = this->ParseAllCommand(buffer, size);
        return allCommands[1];
    }

    if (actionCommand == "READ")
    {
        allCommands = this->ParseAllCommand(buffer, size);
        return allCommands[1];
    }

    return " ";
}

std::string CommandParser::ParseSubject(char buffer[], int size)
{
    std::string actionCommand = this->ParseDatabaseActionCommand(buffer, size);

    std::vector<std::string> allCommands;
    allCommands = this->ParseAllCommand(buffer, size);

    if (actionCommand == "SEND")
    {
        return allCommands[allCommands.size() - 2];
    }
    else if (actionCommand == "DEL" || actionCommand == "READ")
    {
        return allCommands[2];
    }

    return " ";
}

std::string CommandParser::ParseMessage(char buffer[], int size)
{
    std::string actionCommand = this->ParseDatabaseActionCommand(buffer, size);

    std::vector<std::string> allCommands;

    if (actionCommand == "SEND")
    {
        allCommands = this->ParseAllCommand(buffer, size);
    }

    return allCommands[allCommands.size() - 1];
}

std::vector<std::string> CommandParser::ParseAllReceiverName(char buffer[], int size)
{
    std::string actionCommand = this->ParseDatabaseActionCommand(buffer, size);

    std::vector<std::string> allReceiver;

    std::vector<std::string> allCommands;

    if (actionCommand == "SEND")
    {
        allCommands = this->ParseAllCommand(buffer, size);

        for (int i = 1; i < (allCommands.size() - 2); i++)
        {
            allReceiver.push_back(allCommands[i]);
        }
    }

    return allReceiver;
}

std::vector<std::string> CommandParser::ParseAllCommand(char buffer[], int size)
{
    std::string command;
    std::vector<std::string> allCommands;

    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == '-')
        {
            allCommands.push_back(command);
            command.erase(0, command.size());
            continue;
        }

        command += buffer[i];
    }

    if (allCommands[allCommands.size() - 1] != command)
    {
        allCommands.push_back(command);
    }

    return allCommands;
}