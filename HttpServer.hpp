#pragma once 
#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include <iostream>
#include <pthread.h>
#include "Log.hpp"
#include "TcpServer.hpp"
#include "Protocol.hpp"

#define PORT 8083

class HttpServer 
{
    private:
        int _port;
        TcpServer* _tcp_server;
        bool _stop;
    public:
    HttpServer(int port = PORT)
        :_port(port)
         ,_tcp_server(nullptr)
         ,_stop(false)
    {}

    void InitServer()
    {
        _tcp_server = TcpServer::GetInstance(_port);
    }

    void Loop()
    {
        TcpServer* tsvr = TcpServer::GetInstance(_port);
        LOG(INFO, "Loop begin");
        int listen_sock = _tcp_server->Sock();
        while (!_stop)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
            if (sock < 0)
            {
                continue;
            }
            LOG(INFO, "Get a new link");
            int* _sock = new int(sock);
            pthread_t tid;
            pthread_create(&tid, nullptr, Entrance::HandlerRequest, _sock);
            pthread_detach(tid);
        }
    }

    ~HttpServer()
    {}

};

#endif
