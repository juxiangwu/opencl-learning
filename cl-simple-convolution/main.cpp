#include <iostream>
#include "clkernelconfigparser.h"
#include <assert.h>

using namespace std;

int main(int argc, char *argv[])
{
    CLKernelConfigParser parser;
    CLKernelConfig cfg =parser.parase("/home/jenson/Develop/QtCreator/opencl-cpp/datas/cl-config-template.xml");
    //assert(cfg.setKernelPath() != "",true);
    std::cout << "kernel path:" << cfg.getKernelPath() << std::endl;
    std::cout << "kernel name:" << cfg.getKernelName() << std::endl;
    CLKernelArg * arg1 = cfg.getKernelArg("vec2_mem");
    std::cout << "arg index:" << arg1->getIndex() << std::endl;
    std::cout << "arg name:" << arg1->getName() << std::endl;
    std::cout << "arg data:" << arg1->getData() << std::endl;
    std::cout << "arg data type:" << arg1->getDataType() << std::endl;
    return 0;
}
