#include "kernelreader.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

KernelReader::KernelReader()
{

}

std::string KernelReader::read(const std::string & filename){
    boost::filesystem::fstream fs;
    fs.open(filename);
    std::string contents;
    fs >> contents;
    fs.close();
    return contents;
}
