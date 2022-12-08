#pragma once 
#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Util.hpp"

class Entrance 
{
    public:
        static void* HandlerRequest(void* _sock)
        {
            int sock = *(int*)_sock;
            delete (int*)_sock;

//            std::cout << "Get a New Link ... : " << sock << std::endl;
//
//#ifndef DEBUG 
//#define DEBUG
//
//            //For Test
//            char buffer[4096];
//            recv(sock, buffer, sizeof(buffer), 0);
//            
//            std::cout << "-------------------begin------------------" << std::endl;
//            std::cout << buffer << std::endl;
//            std::cout << "-------------------end--------------------" << std::endl;
//
//#endif

            std::string line;
            Util::ReadLine(sock, line);

            std::cout << line << std::endl;

            close(sock);

            return nullptr;
        }
};

#endif
