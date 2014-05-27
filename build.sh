#!/bin/sh

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-build}
if [ -z "$DEBUG" ];then
    BUILD_TYPE=${BUILD_TYPE:-release}
else
    BUILD_TYPE=${BUILD_TYPE:-debug}
fi

mkdir -p $BUILD_DIR/$BUILD_TYPE \
  && cd $BUILD_DIR/$BUILD_TYPE \
  && cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           $SOURCE_DIR \
  && make $*


# cd $SOURCE_DIR && doxygen

