#!/usr/bin/env bash
# Parameter
# -doc generate README.md
# any other paramters will be passed to make
#set -x

for arg do
  shift
  case $arg in
    (-doc) GENDOC="y" ;;
       (*) set -- "$@" "$arg" ;;
  esac
done

SOURCE_DIR=$(cd `dirname $0`;pwd)
BUILD_DIR=${BUILD_DIR:-"$SOURCE_DIR/build_dir"}

if [ -z "$DEBUG" ];then
    BUILD_TYPE=${BUILD_TYPE:-release}
else
    BUILD_TYPE=${BUILD_TYPE:-debug}
fi

if [ ! -z "$CLANG" ];then
    export CC=clang
    export CXX=clang++
fi

[ ! -z "$CXX" ] && echo "CXX is set as $CXX"

[ -d $BUILD_DIR/$BUILD_TYPE ] || mkdir -p $BUILD_DIR/$BUILD_TYPE

pushd `pwd `> /dev/null

cd $BUILD_DIR/$BUILD_TYPE && \
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $SOURCE_DIR && \
    make $@

if [ $? -eq 0 ];then
    if [ `basename $0` == "t" ];then
        $BUILD_DIR/$BUILD_TYPE/notes $@
    else
        [ ! -z "$GENDOC" ] && cd $SOURCE_DIR && ./gen_readme.py
    fi
fi

popd > /dev/null



