#include <iostream>
#include "kernelreader.h"
#include "clexecutor.h"
#include "clvectoradd.h"

#include <CL/cl.h>

using namespace std;

int main(int argc, char *argv[])
{
    //std::string kernel_src = KernelReader::read("../../datas/kernels/kernel_vector_add.cl");

    CLExecutor executor;

    bool res = executor.init();
    if(!res){
        std::cerr << "cannot init clexecutor.\n";
        return -1;
    }
    const std::string file("../../datas/kernels/kernel_vector_add.cl");

    int* A = new int[1000];
    int* B = new int[1000];
    int* C = new int[1000];

    for(size_t i = 0;i < 1000;i++){
        A[i] = B[i] = i;
        C[i] = 0;
    }
    size_t array_size = 1000 * sizeof(int);

    CLVectorAdd vector_add;
    res = vector_add.initKernel(file,"vector_add",A,B,C,array_size,
                          executor.getContext(),executor.getFirstDevice());

    if(!res){
        std::cerr << "init kernel failed.\n";
        executor.release();
        vector_add.release();
        return -1;
    }

    size_t global_work_size[1] = { 1000 };
    size_t local_work_size[1] = { 1 };

    res = executor.execute("vector_add",vector_add.getKernel(),global_work_size,local_work_size);
    if(!res){
        std:cerr << "cannot execute kernel\n";
        vector_add.release();
        executor.release();
        return -1;
    }

    res = executor.readBufferResult(vector_add.getOutputBuffer(),
                                    array_size,(void*)C);
    if(!res){
        std::cerr << "cannot read buffer result.\n";
        vector_add.release();
        executor.release();
        return -1;
    }

    for(int i = 0;i < 1000;i++){
        if((i + 1) % 50 == 0){
            std::cerr << std::endl;
        }
        std::cerr << C[i] << ",";
    }
    std::cout << std::endl;

    vector_add.release();
    executor.release();
    delete[]A;
    delete[]B;
    delete[]C;
    return 0;
}
