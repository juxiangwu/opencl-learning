#ifndef CLARGDATACONVERTER_H
#define CLARGDATACONVERTER_H
#include "basetype.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

class CLArgDataConverter
{
public:
    CLArgDataConverter();
    static int getDataType(const std::string & name);
    static int getReadWriteType(const std::string & name);
    static int getInputType(const std::string & name);
    static bool getBooleanType(const std::string & name);
    static size_t getDataElementSizeByType(int type);
    /**
     *convert string,such as "1,2,3,4",to numeric array[1,2,3,4]
     */
    template<typename T>
    static void stringToNumericArray(const std::string & input,std::vector<T>& output);

    static void * createBufferByDataType(int type,size_t size);
    static void * createbufferFromStringWithType(const std::string arr,int type,size_t size);
};

#endif // CLARGDATACONVERTER_H
