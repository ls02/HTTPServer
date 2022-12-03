#pragma once 

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

//工具类
class Util 
{
    public:
        static int ReadLine(int sock, std::string& out)
        {
            char ch = 'X';

            while (ch !='\n')
            {
               ssize_t s = recv(sock, &ch, 1, 0); 
               if (s > 0)
               {
                   if (ch == '\r')
                   {
                       //\r->\n or \r\n->\n
                       recv(sock, &ch, 1, MSG_PEEK);
                       if (ch == '\n')
                       {
                           //把\r\n->\n
                           //窥探成功，这个字符一定存在
                           recv(sock, &ch, 1, 0);
                       }
                       else 
                       {
                           ch = '\n';
                       }
                   }

                   //1.普通字符
                   //2. \n
                   out.push_back(ch);
               }
               else if (s == 0)
               {
                   return 0;
               }
               else 
               {
                   return -1;
               }

            return out.size();
        }
};
