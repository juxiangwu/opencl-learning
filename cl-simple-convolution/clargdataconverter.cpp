
#include "clargdataconverter.h"
#include <boost/lexical_cast.hpp>

CLArgDataConverter::CLArgDataConverter()
{

}

 int CLArgDataConverter::getDataType(const std::string & name){
    int type = ARG_DATA_TYPE_NONE;
    if(name == ARG_DATA_TYPE_CHAR_NAME){
        type = ARG_DATA_TYPE_CHAR;
    }else if(name == ARG_DATA_TYPE_UCHAR_NAME){
        type = ARG_DATA_TYPE_UCHAR;
    }else if(name == ARG_DATA_TYPE_SHORT_NAME){
        type = ARG_DATA_TYPE_SHORT;
    }else if(name == ARG_DATA_TYPE_USHORT_NAME){
        type = ARG_DATA_TYPE_USHORT;
    }else if(name == ARG_DATA_TYPE_INT_NAME){
        type = ARG_DATA_TYPE_INT;
    }else if(name == ARG_DATA_TYPE_UINT_NAME){
        type = ARG_DATA_TYPE_UINT;
    }else if(name == ARG_DATA_TYPE_LONG_NAME){
        type = ARG_DATA_TYPE_LONG;
    }else if(name == ARG_DATA_TYPE_ULONG_NAME){
        type = ARG_DATA_TYPE_ULONG;
    }else if(name == ARG_DATA_TYPE_FLOAT_NAME){
        type = ARG_DATA_TYPE_FLOAT;
    }else if(name == ARG_DATA_TYPE_DOUBLE_NAME){
        type = ARG_DATA_TYPE_DOUBLE;
    }else{
        type = ARG_DATA_TYPE_NONE;
    }
    return type;
 }

 int CLArgDataConverter::getReadWriteType(const std::string & name){
    int type = ARG_READ_WRITE_NONE;
    if(name == ARG_READ_ONLY_NAME){
        type = ARG_READ_ONLY;
    }else if(name == ARG_WRITE_ONLY_NAME){
        type = ARG_WRITE_ONLY;
    }else if(name == ARG_READ_WRITE_NAME){
        type = ARG_READ_WRITE;
    }else{
        type = ARG_READ_WRITE_NONE;
    }
    return type;
 }

 int CLArgDataConverter::getInputType(const std::string & name){
    int type = ARG_DATA_INPUT_OUTPUT_NONE;
    if(name == ARG_DATA_INPUT_NAME){
        type = ARG_DATA_INPUT;
    }else if(name == ARG_DATA_OUTPUT_NAME){
        type = ARG_DATA_OUTPUT;
    }else{
        type = ARG_DATA_INPUT_OUTPUT_NONE;
    }
    return type;
}

bool CLArgDataConverter::getBooleanType(const std::string & name){
    bool type = ARG_BOOL_TYPE_FALSE;
    if(name == ARG_BOOL_TYPE_FALSE_NAME){
        type = ARG_BOOL_TYPE_FALSE;
    }else if(name == ARG_BOOL_TYPE_TRUE_NAME){
        type = ARG_BOOL_TYPE_TRUE;
    }
    return type;
 }

template<typename T>
static void CLArgDataConverter::stringToNumericArray(const std::string & input,std::vector<T>& output){
    std::vector<std::string> results;
    boost::split(results,input,boost::is_any_of(","));
    size_t size = results.size();
    for(int i = 0;i < size;i++){
        T val = boost::lexical_cast<T>(results[i]);
        output.push_back(val);
    }
    results.clear();
}

size_t CLArgDataConverter::getDataElementSizeByType(int type){
    switch (type) {
    case ARG_DATA_TYPE_CHAR:
        return sizeof(char);
    case ARG_DATA_TYPE_UCHAR:
        return sizeof(unsigned char);
    case ARG_DATA_TYPE_SHORT:
        return sizeof(short);
    case ARG_DATA_TYPE_USHORT:
        return sizeof(unsigned short);
    case ARG_DATA_TYPE_INT:
        return sizeof(int);
    case ARG_DATA_TYPE_UINT:
        return sizeof(unsigned int);
    case ARG_DATA_TYPE_LONG:
        return sizeof(long);
    case ARG_DATA_TYPE_ULONG:
        return sizeof(unsigned long);
    case ARG_DATA_TYPE_FLOAT:
        return sizeof(float);
    case ARG_DATA_TYPE_DOUBLE:
        return sizeof(double);
    default:
        return 0;
    }
}

void * CLArgDataConverter::createBufferByDataType(int type,size_t size){
    void * buffer = NULL;
    switch(type){
    case ARG_DATA_TYPE_CHAR:
        buffer = (void*)new char[size];

        break;
    case ARG_DATA_TYPE_UCHAR:
        buffer = (void*)new unsigned char[size];
        break;
    case ARG_DATA_TYPE_SHORT:
        buffer = (void*)new short[size];
        break;
    case ARG_DATA_TYPE_USHORT:
        buffer = (void*)new unsigned short[size];
        break;
    case ARG_DATA_TYPE_INT:
        buffer = (void*)new int[size];
        break;
    case ARG_DATA_TYPE_UINT:
        buffer = (void*)new unsigned int[size];
        break;
    case ARG_DATA_TYPE_LONG:
        buffer = (void*)new long[size];
        break;
    case ARG_DATA_TYPE_ULONG:
        buffer = (void*)new unsigned long[size];
        break;
    case ARG_DATA_TYPE_FLOAT:
        buffer = (void*)new float[size];
        break;
    case ARG_DATA_TYPE_DOUBLE:
        buffer = (void*)new double[size];
        break;
    }

    return buffer;
}

void* CLArgDataConverter::createbufferFromStringWithType(const std::string arr,int type,size_t size){
    void * buffer = NULL;
    std::vector<char> vec_chars;
    std::vector<unsigned char> vec_uchars;
    std::vector<short> vec_shorts;
    std::vector<unsigned short> vec_ushorts;
    std::vector<int> vec_ints;
    std::vector<unsigned int> vec_uints;
    std::vector<long> vec_longs;
    std::vector<unsigned long> vec_ulongs;
    std::vector<float> vec_floats;
    std::vector<double> vec_doubles;

    switch(type){
    case ARG_DATA_TYPE_CHAR:
        buffer = (void*)new char[size];
        stringToNumericArray<char>(arr,vec_chars);
        memcpy(buffer,vec_chars.data(),size*sizeof(char));
        vec_chars.clear();
        break;
    case ARG_DATA_TYPE_UCHAR:
        buffer = (void*)new unsigned char[size];
        stringToNumericArray<unsigned char>(arr,vec_uchars);
        memcpy(buffer,vec_uchars.data(),size*sizeof(unsigned char));
        vec_uchars.clear();
        break;
    case ARG_DATA_TYPE_SHORT:
        buffer = (void*)new short[size];
        stringToNumericArray<short>(arr,vec_shorts);
        memcpy(buffer,vec_shorts.data(),size*sizeof(short));
        vec_shorts.clear();
        break;
    case ARG_DATA_TYPE_USHORT:
        buffer = (void*)new unsigned short[size];
        stringToNumericArray<unsigned short>(arr,vec_ushorts);
        memcpy(buffer,vec_ushorts.data(),size*sizeof(unsigned short));
        vec_ushorts.clear();
        break;
    case ARG_DATA_TYPE_INT:
        buffer = (void*)new int[size];
        stringToNumericArray<int>(arr,vec_ints);
        memcpy(buffer,vec_ints.data(),size*sizeof(int));
        vec_ints.clear();
        break;
    case ARG_DATA_TYPE_UINT:
        buffer = (void*)new unsigned int[size];
        stringToNumericArray<unsigned int>(arr,vec_uints);
        memcpy(buffer,vec_uints.data(),size*sizeof(unsigned int));
        vec_uints.clear();
        break;
    case ARG_DATA_TYPE_LONG:
        buffer = (void*)new long[size];
        stringToNumericArray<long>(arr,vec_longs);
        memcpy(buffer,vec_longs.data(),size*sizeof(long));
        vec_longs.clear();
        break;
    case ARG_DATA_TYPE_ULONG:
        buffer = (void*)new unsigned long[size];
        stringToNumericArray<unsigned long>(arr,vec_ulongs);
        memcpy(buffer,vec_ulongs.data(),size*sizeof(unsigned long));
        vec_ulongs.clear();
        break;
    case ARG_DATA_TYPE_FLOAT:
        buffer = (void*)new float[size];
        stringToNumericArray<float>(arr,vec_floats);
        memcpy(buffer,vec_floats.data(),size*sizeof(float));
        vec_floats.clear();
        break;
    case ARG_DATA_TYPE_DOUBLE:
        buffer = (void*)new double[size];
        stringToNumericArray<double>(arr,vec_doubles);
        memcpy(buffer,vec_doubles.data(),size*sizeof(double));
        vec_doubles.clear();
        break;
    }
    return buffer;
}
