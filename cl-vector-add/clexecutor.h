#ifndef CLEXECUTOR_H
#define CLEXECUTOR_H
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <vector>
#include <map>

class CLExecutor
{
public:
    CLExecutor();
    bool init();
    bool execute(const std::string & kernel_name, cl_kernel kernel,
                 size_t* global_work_size, size_t* local_work_size);
    bool readBufferResult(cl_mem mem,size_t size,void *host_mem);

    cl_context getContext()const;
    std::vector<cl_device_id> getDevices()const;
    cl_device_id getFirstDevice()const;
    void release();
private:
    bool createContext();
    bool createCommandQueue();

private:
    cl_context m_context;
    cl_command_queue m_command_queue;
    std::vector<cl_platform_id> m_platforms;
    std::vector<cl_device_id> m_devices;
    std::map<std::string,cl_program> m_programs;
    std::map<std::string,cl_kernel> m_kernels;
    cl_uint m_platform_counts;
    bool m_all_inited;
    //size_t* m_global_work_size;
    //size_t* m_local_work_size;

};

#endif // CLEXECUTOR_H
