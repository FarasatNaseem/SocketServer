#include <iostream>
#include <string>
#include <vector>

class FileSystemHandler
{
private:
    /* data */
public:
    bool Save();
    bool Delete();
    std::vector<std::string> FetchAll();
    std::string Read();
};