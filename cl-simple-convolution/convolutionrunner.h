#ifndef CONVOLUTIONRUNNER_H
#define CONVOLUTIONRUNNER_H
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <vector>
#include <map>

class ConvolutionRunner
{
public:
    ConvolutionRunner();
};

#endif // CONVOLUTIONRUNNER_H
