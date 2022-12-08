#pragma once 
#ifndef __UTIL_HPP__
#define __UTIL_HPP__ 

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

//工具类
class Util 
{
    public:
        static int ReadLine(int sock, std::string& out)
        {
            char ch = 'X';
            while (ch != '\n')
            {
                ssize_t s = recv(sock, &ch, 1, 0);
                if (s > 0)
                {
                    if (ch == '\r')
                    {
                        //\r->\r or \r\n->\r
                        recv(sock, &ch, 1, MSG_PEEK);
                        if (ch == '\n')
                        {
                            //窥探成功，这个字符一定存在
                            recv(sock, &ch, 1, 0);
                        }
                        else 
                        {
                            ch = '\n';
                        }
                    }
                    //1. 普通字符
                    //2. \n
                    out.push_back(ch);
                }
                else if ( s == 0)
                {
                    return 0;
                }
                else 
                {
                    return -1;
                }
            }

            return out.size();
        }
};

#endif
