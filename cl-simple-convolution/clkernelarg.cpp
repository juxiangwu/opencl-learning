#include "clkernelarg.h"

CLKernelArg::CLKernelArg():m_index(-1),m_datat_type(-1),m_data_length(0),
                           m_type(0),m_read_write_type(0),
                           m_data(""),m_use_opencv_matrix(false),
                           m_data_path(""),
                           m_data_dim(0),m_name(""),
                           m_data_cols(0),m_data_rows(0)
{

}

void CLKernelArg::setIndex(const int& index){
    m_index = index;
}

int CLKernelArg::getIndex()const{
    return m_index;
}
void CLKernelArg::setDataType(int dataType){
    m_datat_type = dataType;
}

int CLKernelArg::getDataType()const{
    return m_datat_type;
}
void CLKernelArg::setDataLength(int len){
    m_data_length = len;
}

int CLKernelArg::getDataLength()const{
    return m_data_length;
}
void CLKernelArg::setType(int type){
    m_type = type;
}

int CLKernelArg::getType()const{
    return m_type;
}
void CLKernelArg::setReadWriteType(int type){
    m_read_write_type = type;
}

int CLKernelArg::getReadWriteType()const{
    return m_read_write_type;
}
void CLKernelArg::setData(const std::string&  data){
    m_data = data;
}

std::string CLKernelArg::getData()const{
    return m_data;
}
void CLKernelArg::setUseOpenCVMatrix(bool use){
    m_use_opencv_matrix = use;
}

bool CLKernelArg::getUseOpenCVMatrix()const{
    return m_use_opencv_matrix;
}

void CLKernelArg::setDataPath(const std::string& path){
    m_data_path = path;
}

std::string CLKernelArg::getDataPath() const{
    return m_data_path;
}

void CLKernelArg::setDataDim(int dim){
    m_data_dim = dim;
}

int CLKernelArg::getDataDim()const{
    return m_data_dim;
}
void CLKernelArg::setDataRows(int rows){
    m_data_rows = rows;
}

int CLKernelArg::getDataRow()const{
    return m_data_rows;
}
void CLKernelArg::setDataCols(int cols){
    m_data_cols = cols;
}

int CLKernelArg::getDataCols()const{
    return m_data_cols;
}

void CLKernelArg::setName(const std::string&name){
    m_name = name;
}

std::string CLKernelArg::getName()const{
    return m_name;
}
