#include <iostream>
#include <string>
#include "ServerListener.cpp"

class Server
{
private:
    ServerListener *listener;

public:
    Server(std::string localIpAddress, int port);
    void Start();
};