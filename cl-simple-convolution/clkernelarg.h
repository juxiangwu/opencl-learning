#ifndef CLKERNELARG_H
#define CLKERNELARG_H

#include <iostream>

class CLKernelArg
{
public:
    CLKernelArg();
    void setIndex(const int &index);
    int getIndex()const;
    void setDataType(int dataType);
    int getDataType()const;
    void setDataLength(int len);
    int getDataLength()const;
    void setType(int type);
    int getType()const;
    void setReadWriteType(int type);
    int getReadWriteType()const;
    void setData(const std::string &data);
    std::string getData()const;
    void setUseOpenCVMatrix(bool use);
    bool getUseOpenCVMatrix()const;
    void setDataPath(const std::string& path);
    std::string getDataPath() const;
    void setDataDim(int dim);
    int getDataDim()const;
    void setDataRows(int rows);
    int getDataRow()const;
    void setDataCols(int cols);
    int getDataCols()const;
    void setName(const std::string&name);
    std::string getName()const;
private:
    int m_index;
    std::string m_name;
    int m_datat_type;
    int m_data_length;
    int m_type;
    int m_read_write_type;
    std::string m_data;
    bool m_use_opencv_matrix;
    std::string m_data_path;
    int m_data_dim;
    int m_data_rows;
    int m_data_cols;

};

#endif // CLKERNELARG_H
