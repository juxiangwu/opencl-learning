
#include "clkernelconfigparser.h"
#include "tinyxml2.h"
#include <boost/lexical_cast.hpp>
#include "clargdataconverter.h"

CLKernelConfigParser::CLKernelConfigParser()
{

}

CLKernelConfig CLKernelConfigParser::parase(const std::string & configpath){
    CLKernelConfig config;
    XMLDocument doc;
    doc.LoadFile(configpath.c_str());
    XMLElement *root=doc.RootElement();
    XMLElement *first_node=root->FirstChildElement();
    do{
        std::string node_name = first_node->Name();
        if(node_name == "kernel-name"){
            std::string kernel_name = first_node->GetText();
            config.setKernelName(kernel_name);
        }

        if(node_name == "kernel-path"){
            std::string kernel_path = first_node->GetText();
            config.setKernelPath(kernel_path);
        }

        if(node_name == "kernel-args"){
            XMLElement * kernel_arg_nodes = first_node->FirstChildElement();
            do{
                parseKernelArg(kernel_arg_nodes,config);
                kernel_arg_nodes = kernel_arg_nodes->NextSiblingElement();
            }while(kernel_arg_nodes);
        }

        first_node = first_node->NextSiblingElement();
    }while(first_node);
    return config;
}

void CLKernelConfigParser::parseKernelArg(XMLElement * root,CLKernelConfig& config){
    XMLElement * sub_node = root->FirstChildElement();
    CLKernelArg * arg = new CLKernelArg;
    do{
        std::string node_name = sub_node->Name();
        if(node_name == "arg-index"){
            const char * idx_str = sub_node->GetText();
            if(idx_str){
                int index = boost::lexical_cast<int>(sub_node->GetText());
                arg->setIndex(index);
            }
        }else if(node_name == "arg-name"){
            const char * name_str = sub_node->GetText();
            if(name_str){
                arg->setName(std::string(name_str));
            }else{
                arg->setName(std::string(""));
            }
        }else if(node_name == "data-type"){
            const char * data_type_str = sub_node->GetText();
            if(data_type_str){
                int data_type = CLArgDataConverter::getDataType(std::string(data_type_str));
                arg->setDataType(data_type);
            }else{
                arg->setDataType(ARG_DATA_TYPE_NONE);
            }
        }else if(node_name == "data-length"){
            const char * data_len_str = sub_node->GetText();
            if(data_len_str){
                int len = boost::lexical_cast<int>(data_len_str);
                arg->setDataLength(len);
            }else{
                arg->setDataLength(0);
            }
        }else if(node_name == "arg-type"){
            const char * arg_type_str = sub_node->GetText();
            if(arg_type_str){
                int type = CLArgDataConverter::getInputType(std::string(arg_type_str));
                arg->setType(type);
            }else{
                arg->setType(ARG_DATA_INPUT_OUTPUT_NONE);
            }
        }else if(node_name == "read-write-type"){
            const char * read_write_str = sub_node->GetText();
            if(read_write_str){
                int type = CLArgDataConverter::getReadWriteType(std::string(read_write_str));
                arg->setReadWriteType(type);
            }else{
                arg->setReadWriteType(ARG_READ_WRITE_NONE);
            }
        }else if(node_name == "data"){
            const char * attr_use_opencv_matrix = sub_node->Attribute("use-opencv-matrix");
            if(attr_use_opencv_matrix){
                bool used = CLArgDataConverter::getBooleanType(std::string(attr_use_opencv_matrix));
                arg->setUseOpenCVMatrix(used);
            }else{
                arg->setUseOpenCVMatrix(false);
            }

            const char * attr_data_path = sub_node->Attribute("data-path");
            if(attr_data_path){
                arg->setDataPath(std::string(attr_data_path));
            }else{
                arg->setDataPath("");
            }

            const char * attr_data_dim = sub_node->Attribute("data-dim");
            if(attr_data_dim){
                int dim = boost::lexical_cast<int>(attr_data_dim);
                arg->setDataDim(dim);
            }else{
                arg->setDataDim(0);
            }

            const char * attr_data_cols = sub_node->Attribute("data-cols");
            if(attr_data_cols){
                int cols = boost::lexical_cast<int>(attr_data_cols);
                arg->setDataCols(cols);
            }else{
                arg->setDataCols(0);
            }

            const char * attr_data_rows = sub_node->Attribute("data-rows");
            if(attr_data_rows){
                int rows = boost::lexical_cast<int>(attr_data_rows);
                arg->setDataRows(rows);
            }else{
                arg->setDataRows(0);
            }

            const char * data_str = sub_node->GetText();
            if(data_str){
                arg->setData(std::string(data_str));
            }else{
                arg->setData(std::string(""));
            }

        }
        sub_node = sub_node->NextSiblingElement();
    }while(sub_node);
    config.addKernelArg(arg->getName(),arg);
}
