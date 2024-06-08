# 基于重写muduo网络库和protobuf的rpc框架

## [整体设计和流程](docs/整体设计和流程.md)

## 编译 && 使用

[tinymuduo项目仓库](https://github.com/SuycxZMZ/tiny-muduo)

```shell
git clone https://github.com/SuycxZMZ/tiny-muduo.git
cd tiny-muduo
mkdir build
cd build
cmake ..
make 
sudo make install
```

[protobuf和zookeeper安装](tools-package/README.md)

```shell
#本项目编译
git clone https://github.com/SuycxZMZ/MpRPC-Cpp.git
cd MpRPC-Cpp
mkdir build
cd build
cmake ..
make -j4

## 运行示例
cd ../bin
# 运行服务提供方
./provider -i ../rpc.cfg
# 运行服务调用方
./caller -i ../rpc.cfg
```
