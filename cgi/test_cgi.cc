#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    std::string method = getenv("METHOD");
    cerr << "Debug METHOD : " << getenv("METHOD") << endl;

    std::string query_string;

    cerr << "...................................." << std::endl;
    if ("GET" == method)
    {
        query_string = getenv("QUERY_STRING");
        cerr << "Debug QUERY_STRING: " << query_string << std::endl;
    }
    else if ("POST" == method)
    {

    }
    else 
    {

    }

    cerr << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    return 0;
}
