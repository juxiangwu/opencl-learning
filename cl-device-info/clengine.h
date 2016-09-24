#ifndef CLEngine_H
#define CLEngine_H
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <vector>
#include <map>

class CLEngine
{
public:
    CLEngine();
    bool init();
    bool execute(const std::string & kernel_name, cl_kernel kernel,
                 size_t* global_work_size, size_t* local_work_size);
    bool readBufferResult(cl_mem mem,size_t size,void *host_mem);

    cl_context getContext()const;
    std::vector<cl_device_id> getDevices()const;
    cl_device_id getFirstDevice()const;
    void release();

    std::string getPlatformVendor() const;
    std::string getPlatformName()const;
    std::string getPlatformVersion()const;
    std::string getPlatformProfile()const;
    std::string getPlatformExtensions()const;
    cl_device_type getDeviceType()const;
    cl_uint getDeviceVendorId()const;
    cl_uint getDeviceMaxWorkItemDimesions()const;
    cl_uint getDeviceMaxComputeUnits()const;
    cl_uint getDevicePreferedVectorWidthChar()const;
    cl_uint getDevicePreferedVectorWidthInt()const;
    cl_uint getDevicePreferedVectorWidthShort()const;
    cl_uint getDevicePreferedVectorWidthLong()const;
    cl_uint getDevicePreferedVectorWidthFloat()const;
    cl_uint getDevicePreferedVectorWidthDouble()const;
    cl_uint getDevicePreferedVectorWidthHalf()const;
    cl_uint getDeviceNativeVectorWidthChar() const;
    cl_uint getDeviceNativeVectorWidthShort()const;
    cl_uint getDeviceNativeVectorWidthInt()const;
    cl_uint getDeviceNativeVectorWidthLong()const;
    cl_uint getDeviceNativeVectorWidthFloat()const;
    cl_uint getDeviceNativeVectorWidthDouble()const;
    cl_uint getDeviceNativeVectorWidthHalf()const;
    size_t* getDeviceMaxWorkItemSize() const;
    size_t getDeviceMaxGroupSize() const;
    cl_uint getDeviceMaxClockFrequence()const;
    cl_uint getDeviceAddressBits()const;
    cl_ulong getDeviceMaxMemAllocSize()const;
    cl_bool getDeviceImageSupported()const;
    cl_uint getDeviceMaxReadImageArgs()const;
    cl_uint getDeviceMaxWriteImageArgs()const;
    size_t getDeviceImage2DMaxWidth()const;
    size_t getDeviceImage2DMaxHeight()const;
    size_t getDeviceImage3DMaxWidth()const;
    size_t getDeviceImage3DMaxHeight()const;
    size_t getDeviceImage3DMaxDepth()const;
    cl_uint getDeviceMaxSamplers()const;
    size_t getDeviceMaxParameterSize()const;
    cl_uint getDeviceMemBaseAddrAlign()const;
    cl_uint getDeviceMinDataTypeAlignSize()const;
    cl_device_fp_config getDeviceSingleFpConfig()const;

    cl_uint getDeviceGlobalMemCacheLines()const;
    cl_ulong getDeviceGlobalMemCacheSize()const;
    cl_ulong getDeviceGlobalMemSize()const;
    cl_ulong getDeviceMaxConstantBufferSize()const;
    cl_uint getDeviceMaxConstantArgs()const;
    cl_device_local_mem_type getDeviceLocalMemType()const;
    cl_ulong getDeviceLocalMemSize()const;
    cl_bool getDeviceErrorCorrectionSupport()const;
    cl_bool getDeviceHostUnifiedMemory()const;
    size_t getDeviceProfilingTimerResolution()const;
    cl_bool getDeviceEndianLittle()const;
    cl_bool getDeviceAvailable()const;
    cl_bool getDeviceCompilerAvailable()const;
    cl_device_exec_capabilities getDeviceExecCapabilities()const;
    cl_command_queue_properties getDeviceCommandQueueProperties()const;

    std::string getDeviceProfile()const;
    std::string getDeviceVersion()const;
    std::string getDeviceExtensions()const;

private:
    bool createContext();
    bool createCommandQueue();
    std::string getPlatformInfo(cl_platform_info name) const;
    std::string getDeviceInfoStringValue(cl_device_info name)const;
    template<typename T>
    T getDeviceInfoValue(cl_device_info name);
private:
    cl_context m_context;
    cl_command_queue m_command_queue;
    std::vector<cl_platform_id> m_platforms;
    std::vector<cl_device_id> m_devices;
    std::map<std::string,cl_program> m_programs;
    std::map<std::string,cl_kernel> m_kernels;
    cl_uint m_platform_counts;
    bool m_all_inited;
    size_t * m_device_max_work_item_size;


};

#endif // CLEngine_H
