set(SRC_LIST userservice.cc ../user.pb.cc)
# set(SRC_LIST userservice.cc ../friend.pb.cc)

add_executable(provider ${SRC_LIST})
target_link_libraries(provider mprpc protobuf)