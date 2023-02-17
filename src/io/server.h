#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>

std::string extractJsonBody(const std::string& httpRequest) {
    size_t pos = httpRequest.find("\r\n\r\n");
    if (pos == std::string::npos) {
        return "";
    }
    return httpRequest.substr(pos + 4);
}

void handleClient(int client_socket) {
    char buffer[1024] = {0};
    const std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";

    int valread = read(client_socket, buffer, 1024);
    std::cout << "Received HTTP request: " << buffer << std::endl;
    std::string jsonBody = extractJsonBody(buffer);
    std::string echo = response + jsonBody;
    send(client_socket, echo.c_str(), echo.length(), 0);
    std::cout << "Echoed HTTP response: " << echo << std::endl;
    close(client_socket);
}

bool startServer(int port) {
    bool started = false;
    struct sockaddr_in address;
    int server_fd;
    int opt = 1;
    int addrlen = sizeof(address);
    std::vector<std::thread> threads;
    
    std::cout << "Executing start server.. Port = " + std::to_string(port) << std::endl;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Operation failed: socket file descriptor failed" << std::endl;
        exit(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0) {
        std::cerr << "Operation failed: setsockopt failed" << std::endl;
        std::cout << errno;
        exit(2);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
         std::cerr << "Operation failed: Could not bind to port" << std::endl;
         exit(3);
    } 

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Operation failed: listen failed" << std::endl;
        exit(4);
    }

    while (true) {
        int new_socket;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Operation failed, could not accept client" << std::endl;
            continue;
        }
        threads.emplace_back(std::thread(handleClient, new_socket));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return started;
   
}