#include <iostream>
#include <vector>
#include <string>

class CommandParser
{
public:
    std::string ParseDatabaseActionCommand(char buffer[], int size);
    std::string ParseSenderName(char buffer[], int size);
    std::string ParseReceiverName(char buffer[], int size);
    std::string ParseSubject(char buffer[], int size);
    std::string ParseMessage(char buffer[], int size);

    std::vector<std::string> ParseAllCommand(char buffer[], int size);
};