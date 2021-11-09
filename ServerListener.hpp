#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "ServerListenerListenAddress.cpp"
#include "CommandParser.cpp"
#include "FileSystemHandler.cpp"

class ServerListener
{
private:
    ServerListenerListenAddress *listenAddress;
    void SetListenAddress(ServerListenerListenAddress *listenAddress);
    CommandParser *parser;
    FileSystemHandler *fileSystemHandler;

    int listeningSocket;
    // int clientSocket;

    sockaddr_in serverInfo;
    sockaddr_in clientInfo;
    socklen_t addressLength;

    void CreateSocket();
    void Bind();
    void Listen();
    int Accept();
    void Close();

    void HandleClient(int clientSocket);

public:
    ServerListener(ServerListenerListenAddress *listenAddress);
    ServerListenerListenAddress *GetListenAddress();

    void Start();

    virtual ~ServerListener();
};