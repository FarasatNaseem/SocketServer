#include <iostream>
#include <string>
#include "Server.cpp"

int main(int argc, char const *argv[])
{

    Server *server = new Server("localhost", 1111);
    server->Start();

    return 0;
}