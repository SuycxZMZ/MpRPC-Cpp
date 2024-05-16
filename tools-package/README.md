## 1. zookeeper 
```shell
# 解压

# 配置 修改
cd /conf
cp zoo_sample.cfg zoo.cfg
vim zoo.cfg
# 将数据目录改为自己的路径 这一句
dataDir=/data/zookeeper

# 启动命令 在bin目录下
./zkServer.sh start

# 查看开启状态
./zkServer.sh status

# 如果没有开启查看 本机是否安装java环境
java

# 如果没有安装java环境
sudo apt-get install -y openjdk-8-jdk
sudo vim .bashrc
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
source .bashrc

# 关闭
./zkServer.sh stop

```

### 安装c接口
在工程文件夹下
```shell
cd src/c
sudo ./configure
sudo make
# 如果报错，数据类型溢出之类的修改一下Makefile
AM_CFLAGS = -Wall -Werror # 去掉-Werror

# 然后
sudo make
sudo make install

```

