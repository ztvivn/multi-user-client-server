#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <cstring>

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        
        if (bytesReceived <= 0) {
            break;
        }

        std::cout << "Сообщение от клиента: " << buffer << '\n';
        std::memset(buffer, 0, sizeof(buffer)); // Очищаем буфер перед следующим приемом
    }

    close(clientSocket);  // Закрываем соединение после завершения
}

int main() 
{
    auto serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    std::vector<std::thread> clientThreads;

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Ошибка подключения клиента!\n";
            continue;
        }

        std::cout << "Клиент подключен!\n";

        // Запускаем новый поток для обработки клиента
        clientThreads.push_back(std::thread(handleClient, clientSocket));
    }

    // Ждем завершения всех потоков перед выходом
    for (auto& th : clientThreads) {
        if (th.joinable()) {
            th.join();
        }
    }

    close(serverSocket);

    return 0;
}
