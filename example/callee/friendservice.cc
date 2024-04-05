#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t id)
    {
        std::cout << "do local GetFriendList service" << std::endl;
        return {"zhang san", "li si", "zhao jian yong"};
    }
    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        std::vector<std::string> friend_list = GetFriendList(id);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(1);
        
        for (auto & name : friend_list)
        {
            std::string * p = response->add_friend_();
            *p = name;
        }

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