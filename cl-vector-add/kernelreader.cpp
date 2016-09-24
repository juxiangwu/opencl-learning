#include "kernelreader.h"
#include <boost/filesystem.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;


KernelReader::KernelReader()
{

}

std::string KernelReader::read(const std::string & filename){

    // Create the file mapping
    file_mapping fm(filename.c_str(), read_only);
    // Map the file in memory
    mapped_region region(fm, read_only);
    char * addr = (char *)region.get_address();
    std::string content(addr);
   // std::cout << addr << std::cout;
    return content;
}
