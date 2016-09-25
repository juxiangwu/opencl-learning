#ifndef KERNELCONFIGPARSER_H
#define KERNELCONFIGPARSER_H

#include <tinyxml2.h>
#include "basetype.h"
#include "clkernelconfig.h"
using namespace tinyxml2;
class CLKernelConfigParser
{
public:
    CLKernelConfigParser();
    CLKernelConfig parase(const std::string & configpath);
private:
    void parseKernelArg(XMLElement * root, CLKernelConfig &config);
};

#endif // KERNELCONFIGPARSER_H
