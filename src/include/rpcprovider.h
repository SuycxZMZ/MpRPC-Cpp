#ifndef PROVIDER_H
#define PROVIDER_H

#include "google/protobuf/service.h"

class RpcProvider
{
public:
    void NotifyService(google::protobuf::Service * service);

    void Run();
private:

};


#endif