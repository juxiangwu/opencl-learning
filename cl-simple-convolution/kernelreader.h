#ifndef KERNELREADER_H
#define KERNELREADER_H

#include <iostream>

class KernelReader
{
public:
    KernelReader();
    static std::string read(const std::string & filename);
};

#endif // KERNELREADER_H
