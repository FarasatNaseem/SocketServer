#include "ServerListener.hpp"

ServerListener::ServerListener(ServerListenerListenAddress *listenAddress)
{
    this->listenAddress = listenAddress;
    this->parser = new CommandParser();
    this->fileSystemHandler = new FileSystemHandler();
}

ServerListenerListenAddress *ServerListener::GetListenAddress()
{
    return this->listenAddress;
}

void ServerListener::SetListenAddress(ServerListenerListenAddress *listenAddress)
{
    this->listenAddress = listenAddress;
}

void ServerListener::CreateSocket()
{
    this->listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (this->listeningSocket == -1)
    {
        throw "Cant create socket!\n";
    }
}

void ServerListener::Bind()
{
    this->serverInfo.sin_family = AF_INET;
    this->serverInfo.sin_port = htons(this->listenAddress->GetPort());
    this->serverInfo.sin_addr.s_addr = INADDR_ANY;

    memset(this->serverInfo.sin_zero, 8, 0);

    int length = sizeof(this->serverInfo);

    if (bind(this->listeningSocket, (sockaddr *)&this->serverInfo, length) == -1)
    {
        throw "Cant bind";
    }
}

void ServerListener::Listen()
{
    if (listen(this->listeningSocket, 5) == -1)
    {
        throw "Cant listen";
    }
}

int ServerListener::Accept()
{
    this->addressLength = sizeof(sockaddr_in);
    int clientSocket;

    if ((clientSocket = accept(this->listeningSocket, (sockaddr *)&this->clientInfo, &addressLength)) == -1)
    {
        throw "Unexpected error!\n";
    }

    return clientSocket;
}

void ServerListener::Close()
{
    close(this->listeningSocket);
}

void ServerListener::Start()
{
    this->CreateSocket();

    this->Bind();

    this->Listen();

    std::vector<std::thread> workers;

    while (true)
    {
        std::cout << "\nServer is listening\n";
        std::cout << "==================\n";
        int clientSocket = this->Accept();

        workers.push_back(std::thread(&ServerListener::HandleClient, this, clientSocket));
    }

    std::for_each(workers.begin(), workers.end(), [](std::thread &t)
                  { t.join(); });

    this->Close();
}

void ServerListener::HandleClient(int clientSocket)
{
    printf("Connection accepted from %s:%d\n", inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
    char buffer[1024];
    std::string command;

    std::string senderName;
    std::string receiverName;
    std::string subject;
    std::string userMessage;
    std::vector<std::string> allReceiverName;
    while (true)
    {
        int receivedBytes = recv(clientSocket, buffer, 1024, 0);

        if (receivedBytes > 0)
        {
            command = this->parser->ParseDatabaseActionCommand(buffer, receivedBytes);
            std::cout << command << std::endl;

            if (command == "SEND")
            {
                std::cout << "Performing send operation";

                // Sender name: (IP Address)
                senderName = inet_ntoa(clientInfo.sin_addr);
                senderName += ":";
                senderName += ntohs(clientInfo.sin_port);

                // Receiver Names:
                allReceiverName = this->parser->ParseAllReceiverName(buffer, receivedBytes);

                // Subject:
                subject = this->parser->ParseSubject(buffer, receivedBytes);

                // User message text:
                userMessage = this->parser->ParseMessage(buffer, receivedBytes);

                try
                {
                    if (this->fileSystemHandler->Save(senderName, allReceiverName, subject, userMessage))
                    {
                        if (send(clientSocket, "OK", 3, 0) == -1)
                        {
                            throw "send answer failed";
                        }
                    }
                    else
                    {
                        if (send(clientSocket, "Err", 3, 0) == -1)
                        {
                            throw "send answer failed";
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    if (send(clientSocket, "Err", 3, 0) == -1)
                    {
                        throw "send answer failed";
                    }
                }
            }
            else if (command == "READ")
            {
                subject = this->parser->ParseSubject(buffer, receivedBytes);
                receiverName = this->parser->ParseReceiverName(buffer, receivedBytes);

                std::string data = this->fileSystemHandler->Read(receiverName, subject);

                try
                {
                    if (data != " ")
                    {
                        if (send(clientSocket, data.c_str(), data.size() + 1, 0) == -1)
                        {
                            throw "Read failed";
                        }
                    }
                    else
                    {
                        if (send(clientSocket, "Err", 3, 0) == -1)
                        {
                            throw "send answer failed";
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    if (send(clientSocket, "Err", 3, 0) == -1)
                    {
                        throw "send answer failed";
                    }
                }
            }
            else if (command == "DEL")
            {
                subject = this->parser->ParseSubject(buffer, receivedBytes);
                receiverName = this->parser->ParseReceiverName(buffer, receivedBytes);

                try
                {
                    if (this->fileSystemHandler->Delete(receiverName, subject))
                    {
                        if (send(clientSocket, "OK", 3, 0) == -1)
                        {
                            throw "send answer failed";
                        }
                    }
                    else
                    {
                        if (send(clientSocket, "Err", 3, 0) == -1)
                        {
                            throw "send answer failed";
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    if (send(clientSocket, "Err", 3, 0) == -1)
                    {
                        throw "send answer failed";
                    }
                }
            }
            else if (command == "LIST")
            {
                std::string data = this->fileSystemHandler->GetList(this->parser->ParseReceiverName(buffer, receivedBytes));

                try
                {
                    if (data != " ")
                    {
                        if (send(clientSocket, data.c_str(), data.size() + 1, 0) == -1)
                        {
                            throw "Read failed";
                        }
                    }
                    else
                    {

                        if (send(clientSocket, "Err", 3, 0) == -1)
                        {
                            throw "send answer failed";
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    if (send(clientSocket, "Err", 3, 0) == -1)
                    {
                        throw "send answer failed";
                    }
                }
            }
        }
    }

    // while (true)
    // {
    //     int receivedBytes = recv(clientSocket, buffer, 1024, 0);

    //     memset(buffer, 1024, 0);
    //     if (receivedBytes > 0)
    //     {
    //         command = this->parser->ParseDatabaseActionCommand(buffer, receivedBytes);

    //         if (command == "SEND") // Done.
    //         {
    //             senderName = this->parser->ParseSenderName(buffer, receivedBytes);
    //             receiverName = this->parser->ParseReceiverName(buffer, receivedBytes);
    //             subject = this->parser->ParseSubject(buffer, receivedBytes);
    //             userMessage = this->parser->ParseMessage(buffer, receivedBytes);

    //             try
    //             {
    //                 if (this->fileSystemHandler->Save(senderName, receiverName, subject, userMessage))
    //                 {
    //                     if (send(clientSocket, "OK", 3, 0) == -1)
    //                     {
    //                         throw "send answer failed";
    //                     }
    //                 }
    //                 else
    //                 {
    //                     if (send(clientSocket, "Err", 3, 0) == -1)
    //                     {
    //                         throw "send answer failed";
    //                     }
    //                 }
    //                 /* code */
    //             }
    //             catch (const std::exception &e)
    //             {
    //                 if (send(clientSocket, "Err", 3, 0) == -1)
    //                 {
    //                     throw "send answer failed";
    //                 }
    //             }
    //         }
    //         else if (command == "LIST")
    //         {
    //             receiverName = this->parser->ParseReceiverName(buffer, receivedBytes);
    //             std::string data = this->fileSystemHandler->GetList(receiverName);

    //             try
    //             {
    //                 if (data != " ")
    //                 {
    //                     if (send(clientSocket, data.c_str(), data.size() + 1, 0) == -1)
    //                     {
    //                         throw "Read failed";
    //                     }
    //                 }
    //                 else
    //                 {

    //                     if (send(clientSocket, "Err", 3, 0) == -1)
    //                     {
    //                         throw "send answer failed";
    //                     }
    //                 }
    //             }
    //             catch (const std::exception &e)
    //             {
    //                 if (send(clientSocket, "Err", 3, 0) == -1)
    //                 {
    //                     throw "send answer failed";
    //                 }
    //             }
    //         }
    //         else if (command == "DEL") // Done
    //         {
    //             receiverName = this->parser->ParseReceiverName(buffer, receivedBytes);
    //             subject = this->parser->ParseSubject(buffer, receivedBytes);

    //             try
    //             {
    //                 if (this->fileSystemHandler->Delete(receiverName, subject))
    //                 {
    //                     if (send(clientSocket, "OK", 3, 0) == -1)
    //                     {
    //                         throw "send answer failed";
    //                     }
    //                 }
    //                 else
    //                 {
    //                     if (send(clientSocket, "Err", 3, 0) == -1)
    //                     {
    //                         throw "send answer failed";
    //                     }
    //                 }
    //             }
    //             catch (const std::exception &e)
    //             {
    //                 if (send(clientSocket, "Err", 3, 0) == -1)
    //                 {
    //                     throw "send answer failed";
    //                 }
    //             }
    //         }
    //         else if (command == "READ") // Done
    //         {
    //             receiverName = this->parser->ParseReceiverName(buffer, receivedBytes);
    //             subject = this->parser->ParseSubject(buffer, receivedBytes);

    //             std::string data = this->fileSystemHandler->Read(receiverName, subject);

    //             try
    //             {
    //                 if (data != " ")
    //                 {
    //                     try
    //                     {
    //                         if (send(clientSocket, data.c_str(), data.size() + 1, 0) == -1)
    //                         {
    //                             throw "Read failed";
    //                         }
    //                     }
    //                     catch (const std::exception &e)
    //                     {
    //                         std::cerr << e.what() << '\n';
    //                     }
    //                 }
    //                 else
    //                 {
    //                     if (send(clientSocket, "Err", 3, 0) == -1)
    //                     {
    //                         throw "send answer failed";
    //                     }
    //                 }
    //             }
    //             catch (const std::exception &e)
    //             {
    //                 if (send(clientSocket, "Err", 3, 0) == -1)
    //                 {
    //                     throw "send answer failed";
    //                 }
    //             }
    //         }
    //         else if (command == "QUIT")
    //         {
    //             break;
    //         }
    //     }
    // }

    close(clientSocket);
}

ServerListener::~ServerListener()
{
    delete this->listenAddress;
}