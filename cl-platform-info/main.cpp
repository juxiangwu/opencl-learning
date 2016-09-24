#include <iostream>
#include "clexecutor.h"

using namespace std;

int main(int argc, char *argv[])
{
    bool res  = false;
    CLExecutor executor;
    res = executor.init();
    if(!res){
        std::cerr << "cannot init CLExecutor.\n";
        return -1;
    }
    //std::string vendor = executor.getPlatformVendor();
    std::cout << "cl vendor:" << executor.getPlatformVendor() << std::endl;
    std::cout << "cl name:" << executor.getPlatformName() << std::endl;
    std::cout << "cl profile:" << executor.getPlatformProfile() << std::endl;
    std::cout << "cl extensions:\n" << executor.getPlatformExtensions() << std::endl;
    executor.release();
    return 0;
}
