#include <iostream>
#include <string>

class ServerListenerListenAddress
{
private:
    int port;
    std::string localIpAddress;
    
    void SetLocalIpAddress(std::string localIpAddress);
    void SetPort(int port);

public:
    ServerListenerListenAddress(std::string localIpAddress, int port);
    std::string GetLocalIpAddress();
    int GetPort();
};