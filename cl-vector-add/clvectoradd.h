#ifndef CLVECTORADD_H
#define CLVECTORADD_H
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <iostream>
#include <map>

class CLVectorAdd
{
public:
    CLVectorAdd();
    /**
     * @brief initKernel init kernel
     * @param kernel_path kernel source file path
     * @param kernel_name kernel name
     * @param vec1 input vector
     * @param vec2 input vector
     * @param output output vector
     * @param size the size of vector.size = (length of vector) * (vector element's size)
     * @param ctx cl context
     * @param device cl device
     * @return if init success,return true;else,return false;
     */
    bool initKernel(const std::string&kernel_path,
                    const std::string &kernel_name,
                    void * vec1, void * vec2, void* output,
                    size_t size,
                    cl_context ctx, cl_device_id device);
    /**
     * @brief release release all resources.
     */
    void release();

    cl_kernel getKernel()const;
    cl_mem getBuffer(const std::string name)const;
    cl_mem getOutputBuffer()const;
private:
    cl_kernel m_kernel;
    cl_program m_program;
    std::map<const std::string,cl_mem> m_args;
    bool m_all_inited;
};

#endif // CLVECTORADD_H
