#include <iostream>
#include <string>
#include "Server.cpp"
#include <signal.h>

int abortRequested;
int new_socket;
int create_socket;

void signalHandler(int sig)
{
    if (sig == SIGINT)
    {
        throw("abort Requested... ");

        abortRequested = 1;

        if (new_socket != -1)
        {
            if (shutdown(new_socket, SHUT_RDWR) == -1)
            {
                throw("shutdown new_socket");
            }
            if (close(new_socket) == -1)
            {
                throw("close new_socket");
            }
            new_socket = -1;
        }

        if (create_socket != -1)
        {
            if (shutdown(create_socket, SHUT_RDWR) == -1)
            {
                throw("shutdown create_socket");
            }
            if (close(create_socket) == -1)
            {
                throw("close create_socket");
            }
            create_socket = -1;
        }
    }
    else
    {
        exit(sig);
    }
}

int main(int argc, char const *argv[])
{
    try
    {
        if (signal(SIGINT, signalHandler) == SIG_ERR)
        {
            throw "Signal error";
        }

        if (argc < 2)
        {
            std::cout << "Argument out of range exception\n";
        }

        int port = std::stoi(argv[2]);
        std::string ipAddress = argv[1];

        Server *server = new Server(ipAddress, port);
        server->Start();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}