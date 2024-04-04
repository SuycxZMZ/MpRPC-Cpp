#ifndef MPRPCCHANNEL_H
#define MPRPCCHANNEL_H

#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <string>
#include "rpcheader.pb.h"

class MpRpcChannel : public google::protobuf::RpcChannel
{
public:
    // 所有使用 stub 代理类调用的rpc方法都走到这里，进行序列化和网络发送
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done) override;
};

#endif