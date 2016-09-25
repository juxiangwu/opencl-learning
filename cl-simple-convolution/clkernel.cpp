#include "clkernel.h"
#include "clargdataconverter.h"

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

    if(m_kernel_cfg.getKernelPath().empty()){
        std::cerr << "not defined kernel source.\n";
        return false;
    }
    std::string kernel_src = KernelReader::read(m_kernel_cfg.getKernelPath());

    if(kernel_src.empty()){
        std::cerr << "empty kernel";
        return false;
    }


}

void CLKernel::release(){

}

cl_kernel CLKernel::getKernel()const{
    return m_kernel;
}

bool CLKernel::createProgram(cl_context ctx,cl_device_id device,
                             const std::string & source){
    if(source.empty()){
        std::cerr << "empty source\n";
        return false;
    }
    cl_int err = CL_SUCCESS;
    const char * src = source.c_str();
    size_t len = source.length();
    m_program = clCreateProgramWithSource(ctx,1,&src,&len,&err);
    if(m_program == NULL || err != CL_SUCCESS){
        std::cerr << "cannot create program\n";
        return false;
    }

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

    m_kernel = clCreateKernel(m_program,m_kernel_cfg.getKernelName().c_str(),&err);
    if(m_kernel == NULL || err != CL_SUCCESS){
       std::cerr << "cannot create kernel object:" << err << std::endl;
       clReleaseProgram(m_program);
       clReleaseKernel(m_kernel);
       return false;
    }
    clkernel_args_t args = m_kernel_cfg.getAllKernelArgs();
    clkernel_args_t::iterator args_it = args.begin();
    for(;args_it != args.end();++args_it){
        CLKernelArg * arg = args_it->second;
        cl_buffer_info_t * buffer_info = new cl_buffer_info_t;
        size_t elem_size = CLArgDataConverter::getDataElementSizeByType(arg->getDataType());
        int data_type = arg->getDataType();
        buffer_info->m_index = arg->getIndex();
        buffer_info->m_data_type = data_type;
        buffer_info->m_data_len = arg->getDataLength();
        buffer_info->m_data_element_size = elem_size;
        buffer_info->m_arg_type = arg->getType();
        switch(arg->getType()){
        case ARG_DATA_INPUT:
            buffer_info->m_mem_flag = CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR;
            break;
        case ARG_DATA_OUTPUT:
            buffer_info->m_mem_flag = CL_MEM_READ_WRITE;
            break;
        }

        const std::string data = arg->getData();
        if(!data.empty()){//create and copy
           buffer_info->m_data = CLArgDataConverter::createbufferFromStringWithType(data,data_type,arg->getDataLength());
        }else{//create only
            buffer_info->m_data = CLArgDataConverter::createBufferByDataType(data_type,arg->getDataLength());
        }
        buffer_info->m_buffer = clCreateBuffer(context,buffer_info->m_mem_flag,elem_size * arg->getDataLength(),buffer_info->m_data,&err);
        if(buffer_info->m_buffer)
        m_buffers[arg->getName()] = buffer_info;
        if(buffer_info == NULL || err != CL_SUCCESS){
            //TODO:release resources
            std::cerr <<"cannot create cl_mem object:" << err << std::endl;
            clReleaseProgram(m_program);
            clReleaseKernel(m_kernel);
            return false;
        }
    }

    cl_buffer_map_t::iterator mem_it = m_buffers.begin();
    for(;mem_it != m_buffers.end();++mem_it){

    }

    return true;
}
