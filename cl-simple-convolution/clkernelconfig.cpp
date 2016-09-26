
#include "clkernelconfig.h"

CLKernelConfig::CLKernelConfig():m_kernel_name(""),
    m_kernel_path("")
{
    m_kernel_args.clear();
}

CLKernelConfig::~CLKernelConfig(){
//    if(!m_kernel_args.empty()){
//        std::map<const std::string,CLKernelArg*>::iterator it = m_kernel_args.begin();
//        for(;it != m_kernel_args.end();++it){
//            if(it->second){
//                CLKernelArg * arg = it->second;
//                delete arg;
//                m_kernel_args[it->first] = NULL;
//            }
//        }
//        m_kernel_args.clear();
//        std::cout << "release all args.\n";
//    }
     m_kernel_args.clear();
}

void CLKernelConfig::setKernelPath(const std::string & path){
    m_kernel_path = path;
}

std::string CLKernelConfig::getKernelPath()const{
    return m_kernel_path;
}
void CLKernelConfig::setKernelName(const std::string & name){
    m_kernel_name = name;
}

std::string CLKernelConfig::getKernelName()const{
    return m_kernel_name;
}
CLKernelArg CLKernelConfig::getKernelArg(const std::string  name)const{
    return m_kernel_args[name];
}

void CLKernelConfig::addKernelArg(const std::string  name, CLKernelArg arg){
    m_kernel_args[name] = arg;
}

clkernel_args_t CLKernelConfig::getAllKernelArgs()const{
    return m_kernel_args;
}
