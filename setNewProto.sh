#!/bin/bash
set -x

# 切换到 src 文件夹
cd src

rm -rf *.pb.cc *.pb.h
rm -rf /include/*.pb.h

protoc --cpp_out=./ *.proto
mv *.pb.h /include/

# 切换到 example 文件夹
cd example
rm -rf *.pb.cc *.pb.h
protoc --cpp_out=./ *.proto

