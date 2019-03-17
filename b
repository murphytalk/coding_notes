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
    SANIZIZER='-DCMAKE_CXX_FLAGS="-fsanitize=address  -fsanitize=leak -g" -DCMAKE_C_FLAGS="-fsanitize=address  -fsanitize=leak -g" -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address  -fsanitize=leak" -DCMAKE_MODULE_LINKER_FLAGS="-fsanitize=address  -fsanitize=leak"'
else
    BUILD_TYPE=${BUILD_TYPE:-debug}
fi

if [ ! -z "$CLANG" ];then
    export CC=clang
    export CXX=clang++
    unset SANIZIZER
fi


[ -d $BUILD_DIR/$BUILD_TYPE ] || mkdir -p $BUILD_DIR/$BUILD_TYPE


if [ `basename $0` == "t" ];then
    $BUILD_DIR/$BUILD_TYPE/notes $@
else
    pushd `pwd `> /dev/null
    [ ! -z "$CXX" ] && echo "CXX is set as $CXX"
    # TODO only enable addr santizier in DEBUG
    cd $BUILD_DIR/$BUILD_TYPE && \
        cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $SANIZIZER $SOURCE_DIR && \
        make $@

    [ $? -eq 0 ] && [ ! -z "$GENDOC" ] && cd $SOURCE_DIR && ./gen_readme.py
    popd > /dev/null
fi



