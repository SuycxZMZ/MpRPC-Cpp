#ifndef MPRPCCONtROLLER_H
#define MPRPCCONtROLLER_H

#include <google/protobuf/service.h>
#include <string>

class MprpcController : public google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string & reason);

    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure * callback);
private:
    // rpc方法执行过程中的状态
    bool m_failed;

    // 方法执行过程中的错误信息
    std::string m_errText;
};


#endif