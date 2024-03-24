#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>

class Inject
{
public:
    bool InjectDLL(const std::string& dllPath, const std::string& processName, int processId);
};
