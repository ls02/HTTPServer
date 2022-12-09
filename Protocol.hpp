#pragma once 
#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Util.hpp"
#include "Log.hpp"

#define SEP ": "

#define OK 200
#define NOT_FOUND 404
#define WEB_ROOT "wwwroot"
#define HOME_PAGE "index.html"

class HttpRequest 
{
    public:
        std::string _request_line;
        std::vector<std::string> _request_header;
        std::string _blank;
        std::string _request_body;

        //解析完毕之后的结果
        std::string _method;
        std::string _uri;
        std::string _version;

        std::unordered_map<std::string, std::string> _header_kv;
        int _content_length;
        std::string _path;
        std::string _query_string;

    public:
        HttpRequest()
            :_content_length(0)
        {}
        ~HttpRequest()
        {}
};

class HttpResponse 
{
    public:
        std::string _status_line;
        std::vector<std::string> _response_header;
        std::string _blank;
        std::string _response_body;

        int _status_code;
    public:
        HttpResponse()
            :_status_code(OK)
        {}
        ~HttpResponse()
        {}
};

//读取请求，分析请求，构建响应
//IO通信
class EndPoint 
{
    private:
        int _sock;
        HttpRequest _http_request;
        HttpResponse _http_response;
    private:
        //处理请求行
        void RecvHttpRequestLine()
        {
            auto& line = _http_request._request_line;
            Util::ReadLine(_sock, line);
            line.resize(line.size() - 1);
            LOG(INFO, _http_request._request_line);
        }

        //处理请求报头
        //分离出报头和空行
        void RecvHttpRequestHeader()
        {
            std::string line;
            while (true)
            {
                line.clear();
                Util::ReadLine(_sock, line);

                if (line == "\n")
                {
                    _http_request._blank = line;
                    break;
                }

                line.resize(line.size() - 1);
                _http_request._request_header.push_back(line);
                LOG(INFO, line);
            }
        }

        //分离出请求行的三个字段
        void ParseHttpRequestLine()
        {
            auto& line = _http_request._request_line;
            std::stringstream ss(line);
            ss >> _http_request._method >> _http_request._uri >> _http_request._version;

            LOG(INFO, _http_request._method);
            LOG(INFO, _http_request._uri);
            LOG(INFO, _http_request._version);
        }

        //分离报头的字段，用一个map存储起来
        void ParseHttpRequestHeader()
        {
            std::string key;
            std::string value;
            
            for (auto& iter : _http_request._request_header)
            {
                if (Util::CutString(iter, key, value,  SEP))
                {
                    _http_request._header_kv.insert({key, value});
                }
            }
        }
        
        bool IsNeedRecvHttpRequestBody()
        {
            auto method = _http_request._method;
            if (method == "POST")
            {
                auto& header_kv = _http_request._header_kv;
                auto iter = header_kv.find("Content-Length");
                if (iter != header_kv.end())
                {
                    _http_request._content_length = atoi(iter->second.c_str());

                    return true;
                }
            }

            return false;
        }

        //处理正文
        void RecvHttpRequestBody()
        {
            if (IsNeedRecvHttpRequestBody())
            {
                int content_length = _http_request._content_length;
                auto& body = _http_request._request_body;

                char ch = 0;

                while (content_length)
                {
                    ssize_t s = recv(_sock, &ch, 1, 0);
                    if (s > 0)
                    {
                        body.push_back(ch);
                        content_length--;
                    }
                    else 
                    {
                        break;
                    }
                }
            }
        }

    public:
        EndPoint(int sock)
            :_sock(sock)
        {}

        void RecvHttpRequest()
        {
            RecvHttpRequestLine();
            RecvHttpRequestHeader();
            ParseHttpRequestLine();
            ParseHttpRequestHeader();
            RecvHttpRequestBody();
        }
        
        //解析请求
        void BuildHttpResponse()
        {
            std::string path;
            auto& code = _http_response._status_code;
            if (_http_request._method != "GET" && _http_request._method != "POST")
            {
                //处理非法请求
                LOG(WARNING, "method is not right"); 
                code = NOT_FOUND;
                goto END;
            }
            if (_http_request._method == "GET") 
            {
                size_t pos = _http_request._uri.find('?');
                if (pos != std::string::npos)
                {
                    Util::CutString(_http_request._uri, _http_request._path, _http_request._query_string, "?");
                }
                else 
                {
                    _http_request._path = _http_request._uri;
                }
            }
            path = _http_request._path;
            _http_request._path = WEB_ROOT;
            _http_request._path += path;
            if (_http_request._path[_http_request._path.size() - 1] == '/')
            {
                _http_request._path += HOME_PAGE;
            }

            struct stat st;
            if (stat(_http_request._path.c_str(), &st) == 0)
            {
                //说明资源是存在的
               if (S_ISDIR(st.st_mode)) 
               {
                   //说明请求的资源是一个目录，web里面的目录不被允许的，需要做一下相关处理
                   //虽然是也给目录，但是觉对不会以 '/' 结尾
                   _http_request._path += "/";
                   _http_request._path += HOME_PAGE;
               }

               if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
               {
                   //特殊处理
               }
            }
            else 
            {
                //说明资源是不存在的
                std::string info = _http_request._path;
                info += " Not Found!";
                LOG(WARNING, info);
                    code = NOT_FOUND;
                goto END;
            }

END:
            return;
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

#ifdef DEBUG 
#define DEBUG

            //For Test
            char buffer[4096];
            recv(sock, buffer, sizeof(buffer), 0);
            
            std::cout << "-------------------begin------------------" << std::endl;
            std::cout << buffer << std::endl;
            std::cout << "-------------------end--------------------" << std::endl;

#else 
            EndPoint* ep = new EndPoint(sock);
            ep->RecvHttpRequest();
            ep->BuildHttpResponse();
            ep->SendHttpResponse();

            delete ep;
#endif
            LOG(INFO, "Hander Request End");
            return nullptr;
        }
};

#endif
