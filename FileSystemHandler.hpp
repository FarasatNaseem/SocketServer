#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

class FileSystemHandler
{
private:
    /* data */
    bool CreateFolder(std::string folderName);
    int counter = 0;

public:
    bool Save(std::string sender, std::string receiver, std::string subject, std::string message);
    bool Delete();
    std::vector<std::string> FetchAll();
    std::string Read();
};