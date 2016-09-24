#include <iostream>
#include "clengine.h"
using namespace std;

int main(int argc, char *argv[])
{
    bool res  = false;
    CLEngine engine;
    res = engine.init();
    if(!res){
        std::cerr << "cannot init CLEngine.\n";
        return -1;
    }
    std::cout << "Device Info:\n";
    std::cout << "type:" << engine.getDeviceType() << std::endl;
    std::cout << "vendor id:"<<engine.getDeviceVendorId() << std::endl;
    std::cout << "max work item dimensions:" << engine.getDeviceMaxWorkItemDimesions() << std::endl;
    size_t * info = engine.getDeviceMaxWorkItemSize();
    std::cout << "max work item size:[" << info[0] << "," << info[1] << "," << info[2] << "]" << std::endl;
    std::cout << "max work group size:" << engine.getDeviceMaxGroupSize() << std::endl;
    std::cout << "max compute units:" << engine.getDeviceMaxWorkItemDimesions() << std::endl;

    std::cout << "prefered vector width char:" << engine.getDevicePreferedVectorWidthChar() << std::endl;
    std::cout << "prefered vector width short:" << engine.getDevicePreferedVectorWidthShort() << std::endl;
    std::cout << "prefered vector width int:" << engine.getDevicePreferedVectorWidthInt() << std::endl;
    std::cout << "prefered vector width long:" << engine.getDevicePreferedVectorWidthLong() << std::endl;
    std::cout << "prefered vector width float:" << engine.getDevicePreferedVectorWidthFloat() << std::endl;
    std::cout << "prefered vector width double:" << engine.getDevicePreferedVectorWidthDouble() << std::endl;
    std::cout << "prefered vector width half:" << engine.getDevicePreferedVectorWidthHalf() << std::endl;

    std::cout << "native vector width char:" << engine.getDeviceNativeVectorWidthChar() << std::endl;
    std::cout << "native vector width short:" << engine.getDeviceNativeVectorWidthShort() << std::endl;
    std::cout << "native vector width int:" << engine.getDeviceNativeVectorWidthInt() << std::endl;
    std::cout << "native vector width long:" << engine.getDeviceNativeVectorWidthLong() << std::endl;
    std::cout << "native vector width float:" << engine.getDeviceNativeVectorWidthFloat() << std::endl;
    std::cout << "native vector width double:" << engine.getDeviceNativeVectorWidthDouble() << std::endl;
    std::cout << "native vector width half:" << engine.getDeviceNativeVectorWidthHalf() << std::endl;


    std::cout << "max clock frequence:" << engine.getDeviceMaxClockFrequence() <<"MHz"<< std::endl;
    std::cout << "addr bits:" << engine.getDeviceAddressBits() << std::endl;
    std::cout << "max mem alloc size:" << engine.getDeviceMaxMemAllocSize() << std::endl;
    std::cout << "image supported:" << engine.getDeviceImageSupported() << std::endl;
    std::cout << "max read image args:" << engine.getDeviceMaxReadImageArgs() << std::endl;
    std::cout << "max write image args:" << engine.getDeviceMaxWriteImageArgs() << std::endl;
    std::cout << "image2d max width:" << engine.getDeviceImage2DMaxWidth() << std::endl;
    std::cout << "image2d max height:" << engine.getDeviceImage2DMaxHeight() << std::endl;
    std::cout << "image3d max width:" << engine.getDeviceImage3DMaxWidth() << std::endl;
    std::cout << "image3d max height:" << engine.getDeviceImage3DMaxHeight() << std::endl;
    std::cout << "image3d max depth:" << engine.getDeviceImage3DMaxDepth() << std::endl;
    std::cout << "max samplers:" << engine.getDeviceMaxSamplers() << std::endl;
    std::cout << "max parameter size:" << engine.getDeviceMaxParameterSize() << std::endl;
    std::cout << "mem base addr align:" << engine.getDeviceMemBaseAddrAlign() << std::endl;
    std::cout << "min data type align size:" << engine.getDeviceMinDataTypeAlignSize() << std::endl;
    std::cout << "single fp config:" << engine.getDeviceSingleFpConfig() << std::endl;


    std::cout << "global mem cache lines:" << engine.getDeviceGlobalMemCacheLines() <<std::endl;
    std::cout << "global mem cache size:" << engine.getDeviceGlobalMemCacheSize() << std::endl;
    std::cout << "global mem size:" << engine.getDeviceGlobalMemSize() << std::endl;
    std::cout << "max constant buffer size:" << engine.getDeviceMaxConstantBufferSize() << std::endl;
    std::cout << "max constant args:" << engine.getDeviceMaxConstantArgs() << std::endl;
    std::cout << "local mem type:" << engine.getDeviceLocalMemType() << std::endl;
    std::cout << "local mem size:" << engine.getDeviceLocalMemSize() << std::endl;
    std::cout << "error correction support:" << engine.getDeviceErrorCorrectionSupport() << std::endl;
    std::cout << "host unified memory:" << engine.getDeviceHostUnifiedMemory() << std::endl;
    std::cout << "profiling time resolution:" << engine.getDeviceProfilingTimerResolution() << std::endl;
    std::cout << "endian little:" << engine.getDeviceEndianLittle() << std::endl;
    std::cout << "device avaliable:" << engine.getDeviceAvailable() << std::endl;
    std::cout << "compiler availabel:" << engine.getDeviceCompilerAvailable() << std::endl;
    std::cout << "device exec capabilites:" << engine.getDeviceExecCapabilities() << std::endl;
    std::cout << "device command queue properites:" << engine.getDeviceCommandQueueProperties() << std::endl;
    std::cout << "profile:" << engine.getDeviceProfile() << std::endl;
    std::cout << "version:" << engine.getDeviceVersion() << std::endl;
    std::cout << "extensions:\n" << engine.getDeviceExtensions() << std::endl;


    engine.release();
    return 0;
}
