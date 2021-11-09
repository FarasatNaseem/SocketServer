#include "ServerListenerListenAddress.hpp"

ServerListenerListenAddress::ServerListenerListenAddress(std::string localIpAddress, int port)
{
    this->localIpAddress = localIpAddress;
    this->port = port;
}

int ServerListenerListenAddress::GetPort()
{
    return this->port;
}

std::string ServerListenerListenAddress::GetLocalIpAddress()
{
    return this->localIpAddress;
}

void ServerListenerListenAddress::SetLocalIpAddress(std::string localIpAddress)
{
    this->localIpAddress = localIpAddress;
}

void ServerListenerListenAddress::SetPort(int port)
{
    this->port = port;
}