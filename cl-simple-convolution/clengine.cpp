#include "clengine.h"
#include <iostream>
CLEngine::CLEngine():m_all_inited(false),m_context(NULL),
    m_command_queue(NULL),m_platform_counts(0)
{
    m_devices.clear();
    m_platforms.clear();
    m_device_max_work_item_size = new size_t[3];
}

bool CLEngine::init(){
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

bool CLEngine::execute(const std::string & kernel_name,
                         cl_kernel kernel,
                         size_t* global_work_size,size_t* local_work_size){
    if(!m_all_inited){
        std::cerr << "CLEngine::CLEngine not inited yet.\n";
        return false;
    }
    if(kernel_name.empty()){
        std::cerr << "CLEngine::invalid kernel name\n";
        return false;
    }
    if(kernel == NULL){
        std::cerr <<"CLEngine::invalid kenel object\n";
        return false;
    }
    if(global_work_size == NULL){
        std::cerr << "CLEngine::invalid global work size\n";
        return false;
    }

    if(local_work_size == NULL){
        std::cerr << "CLEngine::invalid local work size\n";
        return false;
    }
    cl_int err = CL_SUCCESS;
    err = clEnqueueNDRangeKernel(m_command_queue,kernel,1,NULL,
                                 global_work_size,local_work_size
                                 ,0,NULL,NULL);
    if(err != CL_SUCCESS){
        std::cerr << "CLEngine::canot runn kernel.\n";
        return false;
    }

    return true;
}

bool CLEngine::readBufferResult(cl_mem mem,size_t size,
                                  void *host_mem){
    if(!m_all_inited){
        return false;
    }
    if(mem == NULL){
        std::cerr << "CLEngine::invalid cl_mem object.\n";
        return false;
    }
    if(host_mem == NULL){
        std::cerr << "CLEngine::invalid host object.\n";
        return false;
    }
    cl_int err = CL_SUCCESS;
    err = clEnqueueReadBuffer(m_command_queue,mem,CL_TRUE,0,size,host_mem,0,NULL,NULL);
    if(err != CL_SUCCESS){
        std::cerr << "CLEngine::cannot read buffer from device.Code:" << err << std::endl;
        return false;
    }
    return true;
}

bool CLEngine::readBufferResults(cl_buffer_map_t & buffers){
    cl_buffer_map_t::iterator it = buffers.begin();
    bool has_read = true;
    for(;it != buffers.end();++it){
        cl_buffer_info_t* buffer = it->second;
        if(buffer->m_arg_type == ARG_DATA_OUTPUT){
            bool res = readBufferResult(buffer->m_buffer,
                       buffer->m_data_element_size * buffer->m_data_len,
                                        buffer->m_data);
            if(!res){
                std::cerr << "CLEngine::read output data failed.\n";
                has_read = false;
                break;
            }
        }

    }
    return has_read;
}

cl_context CLEngine::getContext()const{
    return m_context;
}
cl_device_id CLEngine::getFirstDevice()const{
    if(!m_all_inited){
        return NULL;
    }
    if(m_devices.empty()){
        return NULL;
    }
    return m_devices[0];
}
std::vector<cl_device_id> CLEngine::getDevices()const{
    return this->m_devices;
}

bool CLEngine::createContext(){
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

bool CLEngine::createCommandQueue(){
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

void CLEngine::release(){
    if(m_device_max_work_item_size){
        delete [] m_device_max_work_item_size;
    }
    if(!m_all_inited){
        return;
    }
    for(size_t i = 0;i < m_devices.size();i++){
        clReleaseDevice(m_devices[i]);
    }
    clReleaseContext(m_context);
    clReleaseCommandQueue(m_command_queue);
    m_all_inited = false;
    std::cout << "CLEngine:released.\n";
}

std::string CLEngine::getPlatformVendor() const{
    return getPlatformInfo(CL_PLATFORM_VENDOR);
}

std::string CLEngine::getPlatformName()const{
    return getPlatformInfo(CL_PLATFORM_NAME);
}
std::string CLEngine::getPlatformVersion()const{
    return getPlatformInfo(CL_PLATFORM_VERSION);
}
std::string CLEngine::getPlatformProfile()const{
    return getPlatformInfo(CL_PLATFORM_PROFILE);
}
std::string CLEngine::getPlatformExtensions()const{
    return getPlatformInfo(CL_PLATFORM_EXTENSIONS);

}


std::string CLEngine::getPlatformInfo(cl_platform_info name) const{
    if(!m_all_inited){
        std::cerr << "CLEngine not inited yet.\n";
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
        return std::string("none");
    }
    std::string vendor_name(vendor);
    delete [] vendor;
    return vendor_name;
}

cl_device_type CLEngine::getDeviceType()const{
    return getDeviceInfoValue<cl_device_type>(CL_DEVICE_TYPE);
}

cl_uint CLEngine::getDeviceVendorId()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_VENDOR_ID);
}

cl_uint CLEngine::getDeviceMaxWorkItemDimesions()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
}

cl_uint CLEngine::getDeviceMaxComputeUnits()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS);
}

size_t *CLEngine::getDeviceMaxWorkItemSize() const{
    cl_int err = CL_SUCCESS;

    err = clGetDeviceInfo(m_devices[0],CL_DEVICE_MAX_WORK_ITEM_SIZES,
            3 * sizeof(size_t),m_device_max_work_item_size,NULL);
    if(err != CL_SUCCESS){
        std::cerr << "cannot get device max work item size\n";
        return NULL;
    }
    return m_device_max_work_item_size;
}

size_t CLEngine::getDeviceMaxGroupSize() const{
   return getDeviceInfoValue<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);

}

cl_uint CLEngine::getDevicePreferedVectorWidthChar()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR);
}
cl_uint CLEngine::getDevicePreferedVectorWidthInt()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT);
}
cl_uint CLEngine::getDevicePreferedVectorWidthShort()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT);

}
cl_uint CLEngine::getDevicePreferedVectorWidthLong()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG);

}
cl_uint CLEngine::getDevicePreferedVectorWidthFloat()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT);

}
cl_uint CLEngine::getDevicePreferedVectorWidthDouble()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE);

}
cl_uint CLEngine::getDevicePreferedVectorWidthHalf()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF);

}

cl_uint CLEngine::getDeviceNativeVectorWidthChar() const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR);

}
cl_uint CLEngine::getDeviceNativeVectorWidthShort()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT);

}
cl_uint CLEngine::getDeviceNativeVectorWidthInt()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT);

}
cl_uint CLEngine::getDeviceNativeVectorWidthLong()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG);

}
cl_uint CLEngine::getDeviceNativeVectorWidthFloat()const{
    //return getDeviceInfoCLUIntValue(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT);
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT);
}
cl_uint CLEngine::CLEngine::getDeviceNativeVectorWidthDouble()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE);

}
cl_uint CLEngine::getDeviceNativeVectorWidthHalf()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF);

}

cl_uint CLEngine::getDeviceMaxClockFrequence()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY);
}
cl_uint CLEngine::getDeviceAddressBits()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_ADDRESS_BITS);
}
cl_ulong CLEngine::getDeviceMaxMemAllocSize()const{
    return getDeviceInfoValue<cl_ulong>(CL_DEVICE_MAX_MEM_ALLOC_SIZE);
}
cl_bool CLEngine::getDeviceImageSupported()const{
    return getDeviceInfoValue<cl_bool>(CL_DEVICE_IMAGE_SUPPORT);
}
cl_uint CLEngine::getDeviceMaxReadImageArgs()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MAX_READ_IMAGE_ARGS);
}
cl_uint CLEngine::getDeviceMaxWriteImageArgs()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS);
}

size_t CLEngine::getDeviceImage2DMaxWidth()const{
    return getDeviceInfoValue<size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH);
}
size_t CLEngine::getDeviceImage2DMaxHeight()const{
    return getDeviceInfoValue<size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT);
}
size_t CLEngine::getDeviceImage3DMaxWidth()const{
    return getDeviceInfoValue<size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH);
}
size_t CLEngine::getDeviceImage3DMaxHeight()const{
    return getDeviceInfoValue<size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT);
}
size_t CLEngine::getDeviceImage3DMaxDepth()const{
    return getDeviceInfoValue<size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH);
}
cl_uint CLEngine::getDeviceMaxSamplers()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MAX_SAMPLERS);
}
size_t CLEngine::getDeviceMaxParameterSize()const{
    return getDeviceInfoValue<size_t>(CL_DEVICE_MAX_PARAMETER_SIZE);
}
cl_uint CLEngine::getDeviceMemBaseAddrAlign()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MEM_BASE_ADDR_ALIGN);
}
cl_uint CLEngine::getDeviceMinDataTypeAlignSize()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE);
}
cl_device_fp_config CLEngine::getDeviceSingleFpConfig() const{
    return getDeviceInfoValue<cl_device_fp_config>(CL_DEVICE_SINGLE_FP_CONFIG);
}


cl_uint CLEngine::getDeviceGlobalMemCacheLines()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE);
}
cl_ulong CLEngine::getDeviceGlobalMemCacheSize()const{
    return getDeviceInfoValue<cl_ulong>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
}
cl_ulong CLEngine::getDeviceGlobalMemSize()const{
    return getDeviceInfoValue<cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE);
}
cl_ulong CLEngine::getDeviceMaxConstantBufferSize()const{
    return getDeviceInfoValue<cl_ulong>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE);
}
cl_uint CLEngine::getDeviceMaxConstantArgs()const{
    return getDeviceInfoValue<cl_uint>(CL_DEVICE_MAX_CONSTANT_ARGS);
}
cl_device_local_mem_type CLEngine::getDeviceLocalMemType()const{
    return getDeviceInfoValue<cl_device_local_mem_type>(CL_DEVICE_LOCAL_MEM_TYPE);
}
cl_ulong CLEngine::getDeviceLocalMemSize()const{
    return getDeviceInfoValue<cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE);
}
cl_bool CLEngine::getDeviceErrorCorrectionSupport()const{
    return getDeviceInfoValue<cl_bool>(CL_DEVICE_ERROR_CORRECTION_SUPPORT);
}
cl_bool CLEngine::getDeviceHostUnifiedMemory()const{
    return getDeviceInfoValue<cl_bool>(CL_DEVICE_HOST_UNIFIED_MEMORY);
}
size_t CLEngine::getDeviceProfilingTimerResolution()const{
    return getDeviceInfoValue<size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION);
}
cl_bool CLEngine::getDeviceEndianLittle()const{
    return getDeviceInfoValue<cl_bool>(CL_DEVICE_ENDIAN_LITTLE);
}
cl_bool CLEngine::getDeviceAvailable()const{
    return getDeviceInfoValue<cl_bool>(CL_DEVICE_AVAILABLE);
}
cl_bool CLEngine::getDeviceCompilerAvailable()const{
    return getDeviceInfoValue<cl_bool>(CL_DEVICE_COMPILER_AVAILABLE);
}
cl_device_exec_capabilities CLEngine::getDeviceExecCapabilities()const{
    return getDeviceInfoValue<cl_device_exec_capabilities>(CL_DEVICE_EXECUTION_CAPABILITIES);
}
cl_command_queue_properties CLEngine::getDeviceCommandQueueProperties()const{
    return getDeviceInfoValue<cl_command_queue_properties>(CL_DEVICE_COMPILER_AVAILABLE);
}

std::string CLEngine::getDeviceProfile()const{
    return getDeviceInfoStringValue(CL_DEVICE_PROFILE);
}
std::string CLEngine::getDeviceVersion()const{
    return getDeviceInfoStringValue(CL_DEVICE_VERSION);
}
std::string CLEngine::getDeviceExtensions()const{
    return getDeviceInfoStringValue(CL_DEVICE_EXTENSIONS);
}


template<typename T>
T CLEngine::getDeviceInfoValue(cl_device_info name){
    cl_int err = CL_SUCCESS;
    T value = 0;
    err = clGetDeviceInfo(
                m_devices[0],
                name,
                sizeof(T),
                &value,
                NULL);
    if(err != CL_SUCCESS){
        value = 0;
        std::cerr << "cannot get device int value.error code:" << err << std::endl;
    }
    return value;
}


std::string CLEngine::getDeviceInfoStringValue(cl_device_info name)const{
    if(!m_all_inited){
        std::cerr << "CLEngine not inited yet.\n";
        return std::string("none");
    }
    int err = CL_SUCCESS;
    size_t info_size = 0;
    err = clGetDeviceInfo(m_devices[0],name,0,NULL,&info_size);
    if(info_size == 0 || err != CL_SUCCESS){
        std::cerr << "cannot get device info size.\n";
        return std::string("none");
    }
    char * info = new char[info_size];
    err = clGetDeviceInfo(m_devices[0],name,info_size,info,NULL);
    if(err != CL_SUCCESS){
        std::cerr << "cannot get device info\n";
        return std::string("none");
    }
    std::string info_str(info);
    delete [] info;
    return info_str;

}
