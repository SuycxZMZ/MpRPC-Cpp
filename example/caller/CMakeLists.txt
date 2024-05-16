# set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callfriendservice.cc ../friend.pb.cc)
add_executable(caller ${SRC_LIST})
target_link_libraries(caller mprpc protobuf)