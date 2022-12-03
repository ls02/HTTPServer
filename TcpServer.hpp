#pragma once
#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BACKLOG 5

class TcpServer
{
    private:
        int _port;
        int _listen_sock;
        static TcpServer* svr;

    private:
        TcpServer(int port)
           : _port(port)
             ,_listen_sock(-1)
        {}

        TcpServer(const TcpServer &s){}

    public:
        static TcpServer* GetInistance(int port)
        {
            static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
                if (nullptr == svr)
                {
                    pthread_mutex_lock(&lock);
                    if (nullptr == svr)
                    {
                        svr = new TcpServer(port);
                        svr->InitServer();
                    }
                    pthread_mutex_unlock(&lock);
                }

                return svr;
        }
        void InitServer()
        {
            Socket();
            Bind();
            Listen();
        }

        void Socket()
        {
            _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listen_sock < 0)
            {
                exit(1);
            }

            int opt = 1;
            setsockopt(_listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        }

        void Bind()
        {
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = INADDR_ANY;


            if (bind(_listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
            {
                exit(2);
            }
        }

        void Listen()
        {
            if (listen(_listen_sock, BACKLOG) < 0)
            {
                exit(3);
            }
        }

        int Sock()
        {
            return _listen_sock;
        }

        ~TcpServer()
        {}
};

TcpServer* TcpServer::svr = nullptr;

#endif
