#pragma once 
#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Util.hpp"

class HttpRequest
{
    public:
        std::string request_line;
        std::vector<std::string> request_header;
        std::string blank;
        std::string request_body;
};

class HttpResponse
{
    public:
        std::string status_line;
        std::vector<std::string> response_header;
        std::string blank;
        std::string response_body;
};

//读取请求，分析请求，构建响应
//IO 通信
class EndPoint
{
    private:
        int _sock;
        HttpRequest http_request;
        HttpResponse http_response;

    private:
        void RecvHttpRequestLine()
        {
            Util::ReadLine(_sock, http_request.request_line);
        }

        void RecvHttpRequestHeader()
        {
            
        }
        
    public:
        EndPoint(int sock)
            :_sock(sock)
        {}

        void RcvHttpRequest()
        {

        }

        void ParseHttpRequest()
        {

        }

        void BuildHttpResponse()
        {

        }

        void SendHttpResponse()
        {

        }

        ~EndPoint()
        {
            close(_sock);
        }
};

class Entrance
{
    public:
        static void* HandlerRequest(void* _sock)
        {
            int sock = *(int*)_sock;
            delete(int*)_sock;

//            std::cout << "get a new link ... : " << sock << std::endl;

#ifdef DEBUG 

            char buffer[4096];
            recv(sock, buffer, sizeof(buffer), 0);

            std::cout << "--------------begin---------------------" << std::endl;

            std::cout << buffer << std::endl;
            std::cout << "--------------end---------------------" << std::endl;
#else 
            EndPoint* ep = new EndPoint(sock); 
            ep->RcvHttpRequest();
            ep->ParseHttpRequest();
            ep->BuildHttpResponse();
            ep->SendHttpResponse();
            delete ep;
#endif

            return nullptr;
        }
};

#endif
