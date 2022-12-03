#include <iostream>
#include <string>
#include <memory>
#include "TcpServer.hpp"
#include "HttpServer.hpp"
#include <ctime>

static void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << "port" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << time(nullptr) << std::endl;
    std::cout << __FILE__ << ": " << __LINE__ << std::endl;
//    if (argc != 2)
//    {
//        Usage(argv[0]);
//
//        exit(4);
//    }
//
//    int port = atoi(argv[1]);
//
//    std::shared_ptr<HttpServer> http_server(new HttpServer(port));
//    
//    http_server->InitServer();
//    http_server->Loop();

    return 0;
}
