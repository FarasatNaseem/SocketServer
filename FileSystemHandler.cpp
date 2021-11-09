#include "FileSystemHandler.hpp"
#include <algorithm>

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

int FileSystemHandler::GetLastID(std::string receiverName)
{
    std::ifstream counterFile("Database/" + receiverName + "/Counter.txt");

    std::string id;
    getline(counterFile, id);

    counterFile.close();

    return std::stoi(id);
}

bool FileSystemHandler::MatchTwoString(std::string one, std::string two)
{
    for (int i = 0; i < one.size(); i++)
    {
        if (one[i] != two[i])
        {
            return false;
        }
    }

    return true;
}

std::string FileSystemHandler::GetList(std::string receiver)
{
    DIR *dir;
    int status;
    struct dirent *data;
    std::string p;

    std::string userCompleteData;
    int counter = 0;

    std::string path = "Database/" + receiver;

    dir = opendir(path.c_str());

    if (dir == NULL)
    {
        throw "Directory is empty";
    }

    std::string found;

    while ((data = readdir(dir)) != NULL)
    {
        found = data->d_name;

        if (found == ".." || found == "." || found == "Counter.txt")
        {
            continue;
        }

        p = "Database/" + receiver + "/" + found;

        std::ifstream messageFile(p);

        std::string fileData;
        getline(messageFile, fileData);

        userCompleteData += "\nMessage: ";
        userCompleteData += fileData;
        userCompleteData += " Receiver:";
        userCompleteData += receiver;
        userCompleteData += "Subject: ";
        found = found.substr(0, found.size() - 4);
        userCompleteData += found;
        userCompleteData += " \n";

        messageFile.close();
    }

    closedir(dir);

    return userCompleteData;
}

std::string FileSystemHandler::Read(std::string receiver, std::string subject)
{
    DIR *dir;
    int status;
    struct dirent *data;
    std::string p;

    std::string path = "Database/" + receiver;

    dir = opendir(path.c_str());
    std::string found;

    if (dir == NULL)
    {
        throw "Error";
    }

    while ((data = readdir(dir)) != NULL)
    {
        found = data->d_name;
        std::cout << found << std::endl;

        found = found.substr(0, found.size() - 4);

        if (this->MatchTwoString(found, subject))
        {
            p = "Database/" + receiver + "/" + found + ".txt";

            std::ifstream messageFile(p);

            std::string fileData;
            getline(messageFile, fileData);

            messageFile.close();

            return fileData;
        }
    }

    closedir(dir);

    return " ";
}

bool FileSystemHandler::Delete(std::string receiver, std::string subject)
{
    DIR *dir;
    int status;
    struct dirent *data;
    std::string p;

    std::string path = "Database/" + receiver;

    dir = opendir(path.c_str());
    std::string found;

    if (dir == NULL)
    {
        throw "Error";
    }

    while ((data = readdir(dir)) != NULL)
    {
        found = data->d_name;
        std::cout << found << std::endl;

        found = found.substr(0, found.size() - 4);

        if (this->MatchTwoString(found, subject))
        {
            p = "Database/" + receiver + "/" + found + ".txt";

            status = remove(p.c_str());

            if (status == 0)
            {
                return true;
            }
        }
    }

    closedir(dir);

    return false;
}

bool FileSystemHandler::Save(std::string sender, std::string receiver, std::string subject, std::string message)
{
    if (this->CreateFolder(receiver))
    {
        // Create ids file and open a text file
        std::ofstream counterFile("Database/" + receiver + "/Counter.txt");

        // Write
        counterFile << "0";

        counterFile.close();

        // create file to store user data.
        std::ofstream userDataFile("Database/" + receiver + "/" + subject.c_str() + ".txt");

        // Write
        userDataFile << message;

        userDataFile.close();

        return true;
    }
    else
    {
        int lastId = this->GetLastID(receiver);

        // Create ids file and open a text file
        std::ofstream counterFile("Database/" + receiver + "/Counter.txt");

        // Write
        counterFile << std::to_string(lastId + 1);

        counterFile.close();

        // create file to store user data.
        std::ofstream userDataFile("Database/" + receiver + "/" + subject.c_str() + ".txt");

        // Write
        userDataFile << message;

        userDataFile.close();

        return true;
    }

    return false;
}