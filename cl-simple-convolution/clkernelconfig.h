#ifndef CLKERNELCONFIG_H
#define CLKERNELCONFIG_H

#include <map>
#include <iostream>
#include "clkernelarg.h"

typedef std::map<const std::string,CLKernelArg> clkernel_args_t;

class CLKernelConfig
{
public:
    CLKernelConfig();
    ~CLKernelConfig();
    void setKernelPath(const std::string & path);
    std::string getKernelPath()const;
    void setKernelName(const std::string & name);
    std::string getKernelName()const;
    CLKernelArg getKernelArg(const std::string name)const;
    void addKernelArg(const std::string name, CLKernelArg arg);
    clkernel_args_t getAllKernelArgs()const;
private:
    std::string m_kernel_path;
    std::string m_kernel_name;
    clkernel_args_t m_kernel_args;
};

#endif // CLKERNELCONFIG_H
