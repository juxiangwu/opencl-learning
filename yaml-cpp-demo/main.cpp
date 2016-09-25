#include <iostream>
#include <yaml-cpp/dll.h>
#include <yaml-cpp/yaml.h>
#include <opencv2/opencv.hpp>
#include <boost/property_tree/ptree.hpp>
#include <tinyxml2.h>
using namespace tinyxml2;
using namespace std;

int main(int argc, char *argv[])
{
    XMLDocument doc;

    doc.LoadFile("/home/jenson/Develop/QtCreator/opencl-cpp/datas/cl-config-template.xml");
    XMLElement *root=doc.RootElement();
    std::cout << root->Name() << std::endl;
    XMLElement *firstNode=root->FirstChildElement();
    while(firstNode){
        std::string nodeName(firstNode->Name());
        if(nodeName == "kernel-name"){
            std::cout << firstNode->Name() << ":" << firstNode->GetText() << std::endl;
        }else if(nodeName == "kernel-path"){
            std::cout << firstNode->Name() << ":" << firstNode->GetText() << std::endl;
        }else if(nodeName == "kernel-args"){
            XMLElement * kernelArgNode = firstNode->FirstChildElement();
           do{
               // std::cout << "" << kernelArgNode->Name() << std::endl;

                XMLElement * kernelArgSubNode = kernelArgNode->FirstChildElement();

                do{
                    std::string kernelArgSubNodeName(kernelArgSubNode->Name());
                    if(kernelArgSubNodeName == "arg-index"){
                        std::cout << "\t" << kernelArgSubNodeName << ":" << kernelArgSubNode->GetText() << std::endl;
                    }else if(kernelArgSubNodeName == "arg-name"){
                        std::cout << "\t" << kernelArgSubNodeName << ":" << kernelArgSubNode->GetText() << std::endl;
                    }else if(kernelArgSubNodeName == "data-type"){
                        std::cout << "\t" << kernelArgSubNodeName << ":" << kernelArgSubNode->GetText() << std::endl;
                    }else if(kernelArgSubNodeName == "data-length"){
                        std::cout << "\t" << kernelArgSubNodeName << ":" << kernelArgSubNode->GetText() << std::endl;
                    }else if(kernelArgSubNodeName == "arg-type"){
                        std::cout << "\t" << kernelArgSubNodeName << ":" << kernelArgSubNode->GetText() << std::endl;
                    }else if(kernelArgSubNodeName == "read-write-type"){
                        std::cout << "\t" << kernelArgSubNodeName << ":" << kernelArgSubNode->GetText() << std::endl;
                    }else if(kernelArgSubNodeName == "data"){

                        std::string useOpenCVMatrix = kernelArgSubNode->Attribute("use-opencv-matrix");
                        std::cout <<"\t\tuse-opencv-matrix:"<< useOpenCVMatrix << std::endl;
                        char* datapath = kernelArgSubNode->Attribute("data-path");
                        if(datapath){
                            std::cout << "\t\tdata-path:" << datapath << std::endl;
                        }
                        //std::cout << "\t" << kernelArgSubNodeName << ":" << kernelArgSubNode->GetText() << std::endl;
                        char * data = kernelArgSubNode->GetText();
                        if(data){
                            std::cout << "\tdata:"<<data<<std::endl;
                        }
                    }

                    kernelArgSubNode = kernelArgSubNode->NextSiblingElement();
                }while(kernelArgSubNode);
                kernelArgNode = kernelArgNode->NextSiblingElement();
            } while(kernelArgNode);
        }
        firstNode = firstNode->NextSiblingElement();
    }
    std::cout << std::endl;
    return 0;
}
