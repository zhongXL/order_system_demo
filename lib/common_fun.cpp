#include"common_fun.h"
bool isNumber(const string& s)
{
    return s.find_first_not_of("0123456789") == string::npos;
}

bool isDouble(const string& s) 
{
    if (s.find_first_not_of(".0123456789") == string::npos) 
    {
        if (s.find_first_of(".") == string::npos) 
        {
            return true;
        }
        else 
        {
            int length = s.length() - 4;
            if ((s.find_first_of(".") == s.find_last_of(".")) && (s.find_first_of(".") != 0) && ((s.find(".") > length) || (s.length()<4)))
            {
                return true;
            }
        }
    }
    return false;
}

bool isIP(char* address)
{
    if (address == "localhost") 
    {
        return true;
    }
    const char* d = ".";
    int length = 0;
    char* p;
    char* ptr = NULL;
    p = strtok_s(address, d,&ptr);
    while (p)
    {
        length += 1;
        if (length > 4 || !(isNumber(p)))
        {
            return false;
        }
        int temp = atoi(p);
        if (temp < 0 || temp>256)
        {
            return false;
        }
        p = strtok_s(NULL, d,&ptr);
    }
    return length == 4;
}

bool isUrl(const string& url)
{
    regex reg("^(((file|gopher|news|nntp|telnet|http|ftp|https|ftps|sftp)://)|(www\.))+(([a-zA-Z0-9\._-]+\.[a-zA-Z]{2,6})|([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}))(/[a-zA-Z0-9\&%_\./-~-]*)?$");
    return regex_match(url, reg);
}

bool isTradeNo(const string& trade) 
{
   
    std::regex reg("([-\\w\*]){6,32}");
    return std::regex_match(trade, reg);
}