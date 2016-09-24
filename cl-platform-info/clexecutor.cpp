#include "clexecutor.h"
#include <iostream>
CLExecutor::CLExecutor():m_all_inited(false),m_context(NULL),m_command_queue(NULL),m_platform_counts(0)
{
    m_devices.clear();
    m_platforms.clear();
}

bool CLExecutor::init(){
    bool res = false;
    res = createContext();
    if(!res){
        std::cerr << "cannot create context\n";
        return false;
    }
    res = createCommandQueue();
    if(!res){
        std::cerr << "cannot create command queue\n";
        return false;
    }
    m_all_inited = true;
    return true;
}

bool CLExecutor::execute(const std::string & kernel_name,
                         cl_kernel kernel,
                         size_t* global_work_size,size_t* local_work_size){
    if(!m_all_inited){
        std::cerr << "CLExecutor::CLExecutor not inited yet.\n";
        return false;
    }
    if(kernel_name.empty()){
        std::cerr << "CLExecutor::invalid kernel name\n";
        return false;
    }
    if(kernel == NULL){
        std::cerr <<"CLExecutor::invalid kenel object\n";
        return false;
    }
    if(global_work_size == NULL){
        std::cerr << "CLExecutor::invalid global work size\n";
        return false;
    }

    if(local_work_size == NULL){
        std::cerr << "CLExecutor::invalid local work size\n";
        return false;
    }
    cl_int err = CL_SUCCESS;
    err = clEnqueueNDRangeKernel(m_command_queue,kernel,1,NULL,
                           global_work_size,local_work_size
                           ,0,NULL,NULL);
    if(err != CL_SUCCESS){
        std::cerr << "CLExecutor::canot runn kernel.\n";
        return false;
    }

    return true;
}

bool CLExecutor::readBufferResult(cl_mem mem,size_t size,
                                 void *host_mem){
    if(!m_all_inited){
        return false;
    }
    if(mem == NULL){
        std::cerr << "CLExecutor::invalid cl_mem object.\n";
        return false;
    }
    if(host_mem == NULL){
        std::cerr << "CLExecutor::invalid host object.\n";
        return false;
    }
    cl_int err = CL_SUCCESS;
    err = clEnqueueReadBuffer(m_command_queue,mem,CL_TRUE,0,size,host_mem,0,NULL,NULL);
    if(err != CL_SUCCESS){
        std::cerr << "CLExecutor::cannot read buffer from device.Code:" << err << std::endl;
        return false;
    }
    return true;
}

cl_context CLExecutor::getContext()const{
    return m_context;
}
cl_device_id CLExecutor::getFirstDevice()const{
    if(!m_all_inited){
        return NULL;
    }
    if(m_devices.empty()){
        return NULL;
    }
    return m_devices[0];
}
std::vector<cl_device_id> CLExecutor::getDevices()const{
    return this->m_devices;
}

bool CLExecutor::createContext(){
    cl_int err = CL_SUCCESS;
    cl_uint num_platform = 0;
    cl_platform_id platform = NULL;
    err = clGetPlatformIDs(1, &platform, &num_platform);
    if(err != CL_SUCCESS || num_platform <= 0){
        std::cerr << "cannot found any CL platform.\n";
        return false;
    }
    m_platform_counts = num_platform;
    m_platforms.push_back(platform);
    cl_context_properties context_properties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platform,
        0
    };

    m_context = clCreateContextFromType(context_properties, CL_DEVICE_TYPE_GPU,
                                        NULL, NULL, &err);
    if(err != CL_SUCCESS){
        std::cerr << "cannot create CL context.\n";
        return false;
    }



    return true;
}

bool CLExecutor::createCommandQueue(){
    cl_int err;
    cl_device_id * devices = NULL;
    size_t device_counts = 0;

    err = clGetContextInfo(m_context, CL_CONTEXT_DEVICES, 0, NULL, &device_counts);
    if (err != CL_SUCCESS)
    {
        std::cerr << "cannot query device counts.\n";
        return false;
    }

    if(device_counts <=0){
        std::cerr << "cannot found any device\n";
        return false;
    }
    size_t size = device_counts / sizeof(cl_device_id);
    std::cout << "found:" << size << " deviecs\n";
    devices = new cl_device_id[size];
    err = clGetContextInfo(m_context, CL_CONTEXT_DEVICES, device_counts, devices, NULL);

    if(err != CL_SUCCESS){
        std::cerr << "cannot get devices\n";
        delete [] devices;
        return false;
    }


    m_command_queue = clCreateCommandQueue(m_context, devices[0], 0, NULL);
    if(m_command_queue == NULL){
        std::cerr << "cannot create command queue\n";
        for(size_t i = 0;i < size;i++){
            clReleaseDevice(devices[i]);
        }
        delete [] devices;
        return false;
    }
    for(size_t i = 0;i < size;i++){
        m_devices.push_back(devices[i]);
    }
    delete [] devices;
    return true;
}

void CLExecutor::release(){
    if(!m_all_inited){
        return;
    }
    for(size_t i = 0;i < m_devices.size();i++){
        clReleaseDevice(m_devices[i]);
    }
    clReleaseContext(m_context);
    clReleaseCommandQueue(m_command_queue);
    m_all_inited = false;
    std::cout << "CLExecutor:released.\n";
}

std::string CLExecutor::getPlatformVendor() const{
    return getPlatformInfo(CL_PLATFORM_VENDOR);
}

std::string CLExecutor::getPlatformName()const{
    return getPlatformInfo(CL_PLATFORM_NAME);
}
std::string CLExecutor::getPlatformVersion()const{
    return getPlatformInfo(CL_PLATFORM_VERSION);
}
std::string CLExecutor::getPlatformProfile()const{
    return getPlatformInfo(CL_PLATFORM_PROFILE);
}
std::string CLExecutor::getPlatformExtensions()const{
    return getPlatformInfo(CL_PLATFORM_EXTENSIONS);
}


std::string CLExecutor::getPlatformInfo(cl_platform_info name) const{
    if(!m_all_inited){
        std::cerr << "CLExecutor not inited yet.\n";
        return std::string("none");
    }
    int err = CL_SUCCESS;
    size_t platform_vendor_name_size = 0;
    err = clGetPlatformInfo(m_platforms[0],name,0,NULL,&platform_vendor_name_size);
    if(platform_vendor_name_size == 0 || err != CL_SUCCESS){
        std::cerr << "cannot get platform info name size.\n";
        return std::string("none");
    }
    char * vendor = new char[platform_vendor_name_size];
    err = clGetPlatformInfo(m_platforms[0],name,platform_vendor_name_size,vendor,NULL);
    if(err != CL_SUCCESS){
        std::cerr << "cannot get platform info name\n";
        delete [] vendor;
        return std::string("none");
    }
    std::string vendor_name(vendor);
    delete [] vendor;
    return vendor_name;
}
