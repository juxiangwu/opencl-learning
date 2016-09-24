#include <iostream>
#include <CL/cl.hpp>
#include <vector>
#include <boost/compute.hpp>

using namespace std;

std::string kernel_hello_src = BOOST_COMPUTE_STRINGIZE_SOURCE(
            __kernel void hello(){

            }
);

int main(int argc, char *argv[])
{
    try{
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if(platforms.empty()){
            std::cerr << "cannot query any CL platform.\n";
            return -1;
        }

        cl_context_properties ctx_props[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
        cl::Context context(CL_DEVICE_TYPE_GPU,ctx_props);
        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        if(devices.empty()){
            std::cerr << "cannot get any context device.\n";
            return -1;
        }

        cl::Program::Sources kernel_sources(1,std::make_pair(kernel_hello_src.c_str(),kernel_hello_src.length()));
        cl::Program program = cl::Program(context,kernel_sources);
        program.build(devices);

        cl_int err = CL_SUCCESS;

        cl::Kernel kernel_hello(program,"hello",&err);
        cl::Event event;
        cl::CommandQueue queue(context, devices[0], 0, &err);

        queue.enqueueNDRangeKernel(
                    kernel_hello,
                    cl::NullRange,
                    cl::NDRange(4,4),
                    cl::NullRange,
                    NULL,
                    &event);


        event.wait();

        std::cout << "cl execute finished\n";

    }catch(cl::Error ex){
        std::cerr << "Error:" << ex.what() << std::endl;
    }

    return 0;
}
