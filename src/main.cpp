#include <iostream>
#include "io/server.h"
#include <string>
#include <sstream>
#include <condition_variable>

int main(int argc, char* argv[]) 
{
    std::condition_variable cv;
    std::mutex mutex;
    const std::string logFileName = "logs/current_log.log";

    std::cout << "👋\n";
    std::cout << "Welcome to Wave CPP!" << std::endl;

    if (argc == 2) {
        
        writeToLog(logFileName, getInfoType(), "Wave CPP starded with the following arguments:");

        std::string arg_str(argv[1]); 
        writeToLog(logFileName, getInfoType(), "ARG1 - Requested Listening port = " + arg_str);
              
      
        std::thread server_thread([&] {
            startServer(cv,stoi(arg_str), logFileName);
        });
        writeToLog(logFileName, getInfoType(), "Server initiating..");

        // Block the main thread until the server thread signals the condition variable
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock);

        writeToLog(logFileName, getInfoType(), "Server has finished");
        server_thread.join();
        
    } else {
        std::cout << "Program requires 1 parameter at startup:" << std::endl;
        std::cout << "Argument 1 = port" << std::endl;
        std::cout << "Example: \"./wavecpp 8080\"" << std::endl;
    }
    
}