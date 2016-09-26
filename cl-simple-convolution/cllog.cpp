#include "cllog.h"

CLLog::CLLog()
{

}

void CLLog::d(const std::string&tag,const std::string & msg){
#if(_DEBUG_LOG_)
    std::cout << tag << ":" << msg << std::endl;
#endif
}
