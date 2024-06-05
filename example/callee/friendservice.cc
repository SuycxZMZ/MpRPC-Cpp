#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    // 实际要执行的业务
    std::vector<std::string> GetFriendList(uint32_t id)
    {
        std::cout << "do local GetFriendList service" << std::endl;
        return {"zhang san", "li si", "zhao jian yong"};
    }
    // 给框架调用的函数
    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done) override
    {
        uint32_t id = request->id();
        // 调用本地实际业务
        std::vector<std::string> friend_list = GetFriendList(id);

        // 写响应
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(1);
        for (auto & name : friend_list)
        {
            std::string * p = response->add_friend_();
            *p = name;
        }

        // 回调 --> 将响应序列化并通过网络发送 --> 走muduo
        done->Run();
    }
};

int main(int argc, char ** argv)
{
    // 框架初始化
    MprpcApplication::Init(argc, argv);

    // 把 UserService 发布到节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // run 之后进程阻塞，等待远程rpc调用
    provider.Run();

    return 0;
}