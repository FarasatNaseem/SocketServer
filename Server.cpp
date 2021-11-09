#include "Server.hpp"

Server::Server(std::string localIpAddress, int port)
{
    this->listener = new ServerListener(new ServerListenerListenAddress(localIpAddress, port));
}

void Server::Start()
{
    this->listener->Start();
}