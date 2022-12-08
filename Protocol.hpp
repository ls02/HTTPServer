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
#include "Log.hpp"

class HttpRequest 
{
    public:
        std::string _request_line;
        std::vector<std::string> _request_header;
        std::string _blank;
        std::string _request_body;
};

class HttpResponse 
{
    public:
        std::string _status_line;
        std::vector<std::string> _response_header;
        std::string _blank;
        std::string _response_body;
};

//读取请求，分析请求，构建响应
//IO通信
class EndPoint 
{
    public:
        int _sock;
        HttpRequest _http_request;
        HttpResponse _http_response;
    private:
        void RecvHttpRequestLine()
        {
            Util::ReadLine(_sock, _http_request._request_line);
        }

        void RecvHttpRequestHeader()
        {

        }

    private:
        EndPoint(int sock)
            :_sock(sock)
        {}

        void RcvHttpRequest()
        {

        }
        
        void ParseHttpRequest()
        {

        }

        void BuildttpResponse()
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
            LOG(INFO, "Hander Request Begin");
            int sock = *(int*)_sock;
            delete (int*)_sock;

#ifndef DEBUG 
#define DEBUG

            //For Test
            char buffer[4096];
            recv(sock, buffer, sizeof(buffer), 0);
            
            std::cout << "-------------------begin------------------" << std::endl;
            std::cout << buffer << std::endl;
            std::cout << "-------------------end--------------------" << std::endl;

#else 
            EndPoint* ep = new EndPoint(_sock);
            ep->RcvHttpRequest();
            ep->ParseHttpRequest();
            ep->BuildHttpResponse();
            ep->SendHttpResponse();

            delete ep;
#endif
            LOG(INFO, "Hander Request End");
            return nullptr;
        }
};

#endif
