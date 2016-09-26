#include "clkernel.h"
#include "clargdataconverter.h"
#include "cllog.h"
#define TAG "CLKernel"

CLKernel::CLKernel():m_kernel(NULL),m_program(NULL),
    m_parser(NULL),m_created(false)
{

}

CLKernel::~CLKernel(){
    if(m_parser){
        delete m_parser;
    }
}

bool CLKernel::createKernel(const std::string & kernel_config_file,
                            cl_context  context, cl_device_id device){

    m_parser = new CLKernelConfigParser;
    m_kernel_cfg = m_parser->parase(kernel_config_file);
    CLLog::d(TAG,"parase kernel arg config");
    if(m_kernel_cfg.getKernelPath().empty()){
        std::cerr << "not defined kernel source.\n";
        return false;
    }

    std::string kernel_src = KernelReader::read(m_kernel_cfg.getKernelPath());

    if(kernel_src.empty()){
        std::cerr << "empty kernel";
        return false;
    }

    CLLog::d(TAG,"kernel source:");
    CLLog::d(TAG,kernel_src);

    cl_int err = CL_SUCCESS;
    const char * src = kernel_src.c_str();
    size_t len = kernel_src.length();
    m_program = clCreateProgramWithSource(context,1,&src,&len,&err);
    if(m_program == NULL || err != CL_SUCCESS){
        std::cerr << "cannot create program\n";
        return false;
    }
    CLLog::d(TAG,"created program");
    err = clBuildProgram(m_program,0,NULL,NULL,NULL,NULL);
    if(err != CL_SUCCESS){
        char buildLog[10240];
        clGetProgramBuildInfo(m_program, device, CL_PROGRAM_BUILD_LOG,
                              sizeof(buildLog), buildLog, NULL);

        std::cerr << "Error in kernel: " << std::endl;
        std::cerr << buildLog << std::endl;
        clReleaseProgram(m_program);
        return false;
    }
    std::cout << "kernel name:" << m_kernel_cfg.getKernelName() << std::endl;
    m_kernel = clCreateKernel(m_program,m_kernel_cfg.getKernelName().c_str(),&err);
    if(m_kernel == NULL || err != CL_SUCCESS){
        std::cerr << "cannot create kernel object:" << err << std::endl;
        clReleaseProgram(m_program);
        clReleaseKernel(m_kernel);
        return false;
    }
    CLLog::d(TAG,"built program");

    clkernel_args_t args = m_kernel_cfg.getAllKernelArgs();
    clkernel_args_t::iterator args_it = args.begin();
    for(;args_it != args.end();++args_it){
        CLKernelArg  arg = args_it->second;
//        if(!arg){
//            std::cerr << "invalid kernel arg\n";
//            release();
//            return false;
//        }
        cl_buffer_info_t * buffer_info = new cl_buffer_info_t;
        size_t elem_size = CLArgDataConverter::getDataElementSizeByType(arg.getDataType());
        int data_type = arg.getDataType();
        buffer_info->m_index = arg.getIndex();
        std::cout << "arg index = " << arg.getIndex() << std::endl;
        buffer_info->m_data_type = data_type;
        buffer_info->m_data_len = arg.getDataLength();
        buffer_info->m_data_element_size = elem_size;
        buffer_info->m_arg_type = arg.getType();
        CLLog::d(TAG,"setted base info");
        switch(arg.getType()){
        case ARG_DATA_INPUT:
            buffer_info->m_mem_flag = CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR;
            break;
        case ARG_DATA_OUTPUT:
            buffer_info->m_mem_flag = CL_MEM_READ_WRITE;
            break;
        }
        CLLog::d(TAG,"set mem object mem flags");
        std::string data = arg.getData();
        CLLog::d(TAG,"arg data:");
        CLLog::d(TAG,data);
        if(!data.empty()){//create and copy

            buffer_info->m_data = CLArgDataConverter::createbufferFromStringWithType(data,data_type,arg.getDataLength());
        }else{//create only
            buffer_info->m_data = CLArgDataConverter::createBufferByDataType(data_type,arg.getDataLength());
        }
        if(!buffer_info->m_data){
            std::cerr << "cannot create data buffer.\n";
            release();
            return false;
        }
        CLLog::d(TAG,"created arg data");
        buffer_info->m_buffer = clCreateBuffer(context,buffer_info->m_mem_flag,elem_size * arg.getDataLength(),buffer_info->m_data,&err);
        if(buffer_info->m_buffer)
            m_buffers[arg.getName()] = buffer_info;
        if(buffer_info == NULL || err != CL_SUCCESS){
            //TODO:release resources
            std::cerr <<"cannot create cl_mem object:" << err << std::endl;
            release();
            return false;
        }
    }
    CLLog::d(TAG,"created mem object");
    cl_buffer_map_t::iterator mem_it = m_buffers.begin();
    err = CL_SUCCESS;
    for(;mem_it != m_buffers.end();++mem_it){
        cl_buffer_info_t* buffer = mem_it->second;
//        std::cout << "m_index:" << buffer->m_index << std::endl;
//        std::cout << "m_elem_size:" << buffer->m_data_element_size << std::endl;
//        std::cout << "m_data_len:" << buffer->m_data_len << std::endl;
//        size_t data_size = buffer->m_data_element_size * buffer->m_data_len;
//        std::cout << "m_data_size:" << data_size << std::endl;
//        std::cout << "m_mem_flag:" << buffer->m_mem_flag << std::endl;
//        std::cout << "m_data:" << &buffer->m_data << std::endl;
//        std::cout << "m_data_type:" << buffer->m_data_type << std::endl;
//        std::cout << "m_arg_type:" << buffer->m_arg_type << std::endl;
//        std::cout <<"-------------------------------\n";
        err |= clSetKernelArg(m_kernel,buffer->m_index,
                              sizeof(cl_mem),
                              (const void*)&buffer->m_buffer);
    }
    if(err != CL_SUCCESS){
        std::cerr << "set kernel arg error:" << err << std::endl;
        release();
        return false;
    }
    CLLog::d(TAG,"setted kernel args");

    m_created = true;
    return true;
}

void CLKernel::release(){
    if(m_program){
        clReleaseProgram(m_program);
    }

    if(m_kernel){
        clReleaseKernel(m_kernel);
    }
    cl_buffer_map_t::iterator mem_it = m_buffers.begin();
    for(;mem_it != m_buffers.end();++mem_it){
        cl_buffer_info_t * buff = mem_it->second;
        if(!buff){
            continue;
        }
        if(buff->m_buffer){
            clReleaseMemObject(buff->m_buffer);
        }
        if(buff->m_data){
            delete [] buff->m_data;
        }

        delete buff;
        m_buffers[mem_it->first] = NULL;
    }
    m_buffers.clear();
}

cl_kernel CLKernel::getKernel()const{
    return m_kernel;
}

cl_buffer_info_t * CLKernel::getBufferInfo(const std::string name)const{
    return m_buffers[name];
}

cl_buffer_map_t CLKernel::getBuffers()const{
    return m_buffers;
}

void CLKernel::getOutputBuffres(std::vector<cl_buffer_info_t*>& buffers){
    cl_buffer_map_t::iterator it = m_buffers.begin();
    for(;it != m_buffers.end();++it){
        cl_buffer_info_t * buffer = it->second;
        if(buffer->m_arg_type == ARG_DATA_OUTPUT){
            buffers.push_back(buffer);
        }
    }
}

std::string CLKernel::getName()const{
    return m_kernel_cfg.getKernelName();
}

bool CLKernel::createProgram(cl_context ctx,cl_device_id device,
                             const std::string & source){


    return true;
}
