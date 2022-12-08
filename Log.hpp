#pragma once 
#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>
#include <string>
#include <ctime>

#define INFO    1
#define WARNING 2
#define ERROR   3
#define FATAL   4

#define LOG(level, message) Log(#level, message, __FILE__, __LINE__)

void Log(std::string level, std::string message, std::string file_name, int line)
{
    char str[25] = { 0  };
    time_t cur_t = time(nullptr);
    struct tm* t = gmtime(&cur_t);
    t->tm_hour += 8;
    std::string format = "%Y-%m-%d:%H:%M:%S";
    std::string temp;
    strftime(str, sizeof(str), format.c_str(), t);

    temp = str;

    std::cout << "[" << level << "]" << "["<< temp << "]" << "[" << message << "]" << "[" << file_name << "]" << "[" << line << "]" << std::endl;
}

#endif
