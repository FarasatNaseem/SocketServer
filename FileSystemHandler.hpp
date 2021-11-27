#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <thread>
#include <mutex>

class FileSystemHandler
{
private:
    /* data */
    bool CreateFolder(std::string folderName);
    int counter = 0;
    int GetLastID(std::string receiverName);
    bool MatchTwoString(std::string One, std::string two);

public:
    bool Save(std::string sender, std::vector<std::string> receivers, std::string subject, std::string message);
    bool SaveSingleMessage(std::string sender, std::string receiver, std::string subject, std::string message);
    bool Delete(std::string reveiver, std::string subject);
    std::vector<std::string> FetchAll();
    std::string Read(std::string reveiver, std::string subject);
    std::string GetList(std::string receiver);
    std::string GetFileData(std::string path);
};