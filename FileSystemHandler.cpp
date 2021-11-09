#include "FileSystemHandler.hpp"

bool FileSystemHandler::CreateFolder(std::string folderName)
{
    std::string path = "Database/" + folderName;

    bool isCreated = mkdir(path.c_str(), 0777);

    if (isCreated)
    {
        return false;
    }

    return true;
}

bool FileSystemHandler::Save(std::string sender, std::string receiver, std::string subject, std::string message)
{
    if (this->CreateFolder(receiver))
    {
        // Create ids file and open a text file
        std::ofstream counterFile("Database/" + receiver + "/Counter.txt");

        // Write
        counterFile << this->counter++;

        counterFile.close();

        // create file to store user data.
        std::ofstream userDataFile("Database/" + receiver + "/" + subject.c_str() + ".txt");

        // Write
        userDataFile << message;

        userDataFile.close();
    }
    else
    {
        // Create ids file and open a text file
        std::ofstream counterFile("Database/" + receiver + "/Counter.txt");

        // Write
        counterFile << this->counter++;

        counterFile.close();

        // create file to store user data.
         std::ofstream userDataFile("Database/" + receiver + "/" + subject.c_str() + ".txt");

        // Write
        userDataFile << message;

        userDataFile.close();
    }

    return true;
}