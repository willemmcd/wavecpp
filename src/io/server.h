#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include "../logging/logger.h"

std::string extractJsonBody(const std::string& httpRequest) {
    size_t pos = httpRequest.find("\r\n\r\n");
    if (pos == std::string::npos) {
        return "";
    }
    return httpRequest.substr(pos + 4);
}

void handleClient(int client_socket, std::string logFileName) {
    char buffer[1024] = {0};
    const std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";

    int valread = read(client_socket, buffer, 1024);
    std::string strBuffer(buffer);
    writeToLog(logFileName, getInfoType(), "Received HTTP request: " + strBuffer);
    std::string jsonBody = extractJsonBody(buffer);
    std::string echo = response + jsonBody;
    send(client_socket, echo.c_str(), echo.length(), 0);
    writeToLog(logFileName, getInfoType(), "Echoed HTTP response: " + echo);
    close(client_socket);
}

bool startServer(std::condition_variable& cv, int port, std::string logFileName) {
    bool started = false;
    struct sockaddr_in address;
    int server_fd;
    int opt = 1;
    int addrlen = sizeof(address);
    std::vector<std::thread> threads;
    
    writeToLog(logFileName, getInfoType(), "Executing start server.. Port = " + std::to_string(port));

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        writeToLog(logFileName, getErrorType(), "Operation failed: socket file descriptor failed");
        return(false);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0) {
        writeToLog(logFileName, getErrorType(), "Operation failed: setsockopt failed");        
        return(false);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
         writeToLog(logFileName, getErrorType(), "Operation failed: Could not bind to port");
         return(false);
    } 

    if (listen(server_fd, 3) < 0) {
        writeToLog(logFileName, getErrorType(), "Operation failed: listen failed");
        exit(4);
    }

    while (true) {
        int new_socket;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            writeToLog(logFileName, getErrorType(), "Operation failed, could not accept client");
            continue;
        }
        threads.emplace_back(std::thread(handleClient, new_socket, logFileName));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    cv.notify_one();

    return started;
   
}