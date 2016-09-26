#ifndef CLLOG_H
#define CLLOG_H

#include <string>
#include <iostream>
class CLLog
{
public:
    CLLog();
    static void d(const std::string&tag,const std::string & msg);
};

#endif // CLLOG_H
