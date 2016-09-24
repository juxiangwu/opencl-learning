#include "clvectoradd.h"
#include "kernelreader.h"

CLVectorAdd::CLVectorAdd():m_all_inited(false),m_kernel(NULL),m_program(NULL)
{
    m_args.clear();
}

bool CLVectorAdd::initKernel(const std::string&kernel_path,const std::string & kernel_name,
                             void *vec1, void *vec2, void *output, size_t size,
                             cl_context ctx, cl_device_id device){
    if(kernel_path.empty()){
        std::cerr << "invalid kernel path\n";
        return false;
    }
    if(kernel_name.empty()){
        std::cerr << "invalid kernel name\n";
        return false;
    }
    std::string kernel_src = KernelReader::read(kernel_path);
    if(kernel_src.empty()){
        std::cerr << "cannot read kernel source\n";
        return false;
    }

    cl_int err = CL_SUCCESS;
    const char * kernel_data = kernel_src.c_str();
    size_t kernel_len = kernel_src.length();
    //create program from source kernel
    m_program = clCreateProgramWithSource(ctx,1,&kernel_data,&kernel_len,&err);
    if(!m_program){
        std::cerr << "cannot create program from source\n";
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

    cl_mem vec1_mem = clCreateBuffer(ctx,CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,size,vec1,&err);
    cl_mem vec2_mem = clCreateBuffer(ctx,CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,size,vec2,&err);
    cl_mem output_mem = clCreateBuffer(ctx,CL_MEM_READ_WRITE,size,output,&err);

    if(vec1_mem == NULL || vec2_mem == NULL || output_mem == NULL){
        std::cerr << "cannot create cl buffer.\n";
        clReleaseProgram(m_program);
        return false;
    }

    m_args["vec1_mem"] = vec1_mem;
    m_args["vec2_mem"] = vec2_mem;
    m_args["output_mem"] = output_mem;

    m_kernel = clCreateKernel(m_program,kernel_name.c_str(),&err);
    if(m_kernel == NULL || err != CL_SUCCESS){
        std::cerr << "cannot creat kernel,code:" << err << std::endl;
        clReleaseProgram(m_program);
        clReleaseMemObject(vec1_mem);
        clReleaseMemObject(vec2_mem);
        clReleaseMemObject(output_mem);
        clReleaseKernel(m_kernel);
        m_args.clear();
        return false;
    }
    err = clSetKernelArg(m_kernel,0,sizeof(cl_mem),(const void*)&vec1_mem);
    err |= clSetKernelArg(m_kernel,1,sizeof(cl_mem),(const void*)&vec2_mem);
    err |= clSetKernelArg(m_kernel,2,sizeof(cl_mem),(void*)&output_mem);

    if(err != CL_SUCCESS){
        std::cerr << "cannot set kernel args.Error code:" << err << std::endl;
        clReleaseProgram(m_program);
        clReleaseMemObject(vec1_mem);
        clReleaseMemObject(vec2_mem);
        clReleaseMemObject(output_mem);
        clReleaseKernel(m_kernel);
        m_args.clear();
        return false;
    }
    m_all_inited = true;
    return true;
}

void CLVectorAdd::release(){
    if(!m_all_inited){
        return;
    }
    clReleaseProgram(m_program);
    clReleaseMemObject(m_args["vec1_mem"]);
    clReleaseMemObject(m_args["vec2_mem"]);
    clReleaseMemObject(m_args["output_mem"]);
    clReleaseKernel(m_kernel);
    m_args.clear();
    m_all_inited = false;
}

cl_kernel CLVectorAdd::getKernel()const{
    return m_kernel;
}

cl_mem CLVectorAdd::getBuffer(const std::string  name)const{
     return m_args[name];
}

cl_mem CLVectorAdd::getOutputBuffer()const{
    return m_args["output_mem"];
}
