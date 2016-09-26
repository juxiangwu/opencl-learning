#include <iostream>
#include "clkernelconfigparser.h"
#include "clengine.h"
#include <assert.h>

using namespace std;

int main(int argc, char *argv[])
{
//    CLKernelConfigParser parser;
//    CLKernelConfig cfg =parser.parase("/home/jenson/Develop/QtCreator/opencl-cpp/datas/cl-config-template.xml");
//    //assert(cfg.setKernelPath() != "",true);
//    std::cout << "kernel path:" << cfg.getKernelPath() << std::endl;
//    std::cout << "kernel name:" << cfg.getKernelName() << std::endl;
//    CLKernelArg * arg1 = cfg.getKernelArg("vec2_mem");
//    std::cout << "arg index:" << arg1->getIndex() << std::endl;
//    std::cout << "arg name:" << arg1->getName() << std::endl;
//    std::cout << "arg data:" << arg1->getData() << std::endl;
//    std::cout << "arg data type:" << arg1->getDataType() << std::endl;

    CLEngine engine;
    CLKernel kernel;
    bool res = engine.init();
    if(!res){
        std::cerr << "cannot init CLEngine.\n";
        return -1;
    }
    std::cout << "created engine";
    res = kernel.createKernel("/home/jenson/Develop/QtCreator/opencl-cpp/datas/cl-config-template.xml",
                        engine.getContext(),
                        engine.getDevices()[0]);
    if(!res){
        std::cerr << "cannot create kernel.\n";
        return -1;
    }
    std::cout << "created kernel\n";
    size_t global_work_size[1] = { 9 };
    size_t local_work_size[1] = { 1 };
    res = engine.execute(kernel.getName(),kernel.getKernel(),global_work_size,local_work_size);
    if(!res){
        std::cerr << "cannot execute kernel\n";
        return -1;
    }
    cl_buffer_map_t buffers = kernel.getBuffers();
    res = engine.readBufferResults(buffers);
    if(!res){
        std::cerr << "cannot read datas\n";
        return -1;
    }
    std::vector<cl_buffer_info_t*> outputs;
    kernel.getOutputBuffres(outputs);
    std::vector<cl_buffer_info_t*>::iterator buffer_it = outputs.begin();
    for(;buffer_it != outputs.end();++buffer_it){
        cl_buffer_info_t * buffer = *buffer_it;
        std::cout <<"bufer:" <<buffer->m_index<<"'s data:\n";
        if(buffer->m_data){
            for(int i = 0;i < 9;i++){
                std::cout << /*((int*)buffer->m_data)[i]*/ buffer->m_data[i] << ",";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
