#include "Inject.h"

void PrintPurple(const std::string& text) {
    std::cout << "\033[1;35m" << text << "\033[0m";
}

void PrintRed(const std::string& text) {
    std::cout << "\033[1;31m" << text << "\033[0m";
}

void PrintGreen(const std::string& text) {
    std::cout << "\033[1;32m" << text << "\033[0m";
}

int main() {
    std::string dllPath;
    std::string processName;
    int processId = 0;

    PrintPurple(R"(  
  _____       _           _             _____                    _          _ 
 |_   _|     (_)         | |           |  __ \                  | |        | |
   | |  _ __  _  ___  ___| |_ ___  _ __| |__) |___  ___ ___   __| | ___  __| |
   | | | '_ \| |/ _ \/ __| __/ _ \| '__|  _  // _ \/ __/ _ \ / _` |/ _ \/ _` |
  _| |_| | | | |  __/ (__| || (_) | |  | | \ \  __/ (_| (_) | (_| |  __/ (_| |
 |_____|_| |_| |\___|\___|\__\___/|_|  |_|  \_\___|\___\___/ \__,_|\___|\__,_|
            _/ |                                                              
           |__/                                                               
)");

    PrintPurple("\n[+] Enter DLL path: ");
    std::getline(std::cin, dllPath);

    PrintPurple("[+] Enter process name or ID: ");
    std::getline(std::cin, processName);

    if (isdigit(processName[0])) {
        processId = std::stoi(processName);
    }

    Inject injector;
    if (injector.InjectDLL(dllPath, processName, processId)) {
        PrintGreen("[+] Injection successful!");
    }
    else {
        PrintRed("[+] Injection failed!");
    }

    return 0;
}
