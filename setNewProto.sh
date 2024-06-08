#!/bin/bash
set -x

# 切换到 src 文件夹
cd src

rm -rf *.pb.h
rm -rf *.pb.h

protoc --cpp_out=./ *.proto


