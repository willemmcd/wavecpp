#include <iostream>
#include "io/server.h"
#include <string>
using namespace std;

int main(int argc, char* argv[]) 
{
    cout << "👋\n";
    cout << "Welcome to Wave CPP!" << endl;

    if (argc == 2) {
        cout << "Wave CPP starded with the following arguments:" << endl;

        
        cout << "ARG1 - Requested Listening port = " << argv[1] << endl;
        string arg_str(argv[1]);
        startServer((stoi(arg_str)));
    } else {
        cout << "Program requires 1 parameter at startup:" << endl;
        cout << "Argument 1 = port" << endl;
        cout << "Example: \"./wavecpp 8080\"" << endl;
    }
    
}