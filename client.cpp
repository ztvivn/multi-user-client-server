#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main() 
{
    auto clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(8080);
    clientAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
    sizeof(clientAddress);

    std::string message;

    while(true)
    {
        std::cout << "-> ";
        std::getline(std::cin, message);

        if(message == "exit")
        {
            break;
        }

        send(clientSocket, message.c_str(), message.size(), 0);
    }

    close(clientSocket);

    return 0;
}