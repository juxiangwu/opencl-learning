#ifndef CLKERNEL_H
#define CLKERNEL_H

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <vector>
#include <map>
#include "clkernelconfig.h"
#include "clkernelconfigparser.h"
#include "kernelreader.h"


struct cl_buffer_info_t{
    int m_index;
    cl_mem m_buffer;
    void * m_data;
    int m_data_type;
    size_t m_data_element_size;
    size_t m_data_len;
    int m_arg_type;
    int m_mem_flag;

};

typedef std::map<const std::string,cl_buffer_info_t*> cl_buffer_map_t;

class CLKernel
{
public:
    CLKernel();
    ~CLKernel();
    bool createKernel(const std::string & kernel_config_file,cl_context  context,cl_device_id  device);
    void release();
    cl_kernel getKernel()const;
private:
    bool createProgram(cl_context ctx, cl_device_id device, const std::string & source);
private:
    cl_kernel m_kernel;
    cl_program m_program;
    CLKernelConfig m_kernel_cfg;
    CLKernelConfigParser* m_parser;
    bool m_created;
    cl_buffer_map_t m_buffers;

};

#endif // CLKERNEL_H
