#include "CommandParser.hpp"

std::string CommandParser::ParseDatabaseActionCommand(char buffer[], int size)
{
    std::string command;

    for (int i = 0; i < size; i++)
    {
        if(buffer[i] == '-'){
            break;
        }
        command += buffer[i];
    }
    
    return command;
}