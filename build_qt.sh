#!/bin/bash
set -e

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
PROCESSORS=$(< /proc/cpuinfo grep "processor" | wc -l)
RISCV=/home/qqm/Downloads/work/keystone/riscv64
export PATH=$RISCV/bin:$PATH

QTOUTPUT=$SHELL_FOLDER/build_qt
HOST_OUTPUT=$QTOUTPUT/host_output
TARGET_OUTPUT=$QTOUTPUT/target_output

if [ ! -d "$QTOUTPUT" ]; then
    mkdir $QTOUTPUT
fi
if [ ! -d "$HOST_OUTPUT" ]; then
    mkdir $HOST_OUTPUT
fi
if [ ! -d "$TARGET_OUTPUT" ]; then
    mkdir $TARGET_OUTPUT
fi

build_qt()
{
    # 编译qt
    echo "------------------------------- 编译qt -------------------------------"
    cd $SHELL_FOLDER/qt-everywhere-src-5.12.12

	./configure \
        -opensource \
        -confirm-license \
        -release \
        -optimize-size \
        -strip \
        -ltcg \
        -silent \
        -qpa linuxfb \
        -no-opengl \
        -skip webengine \
        -nomake tools \
        -nomake tests \
        -nomake examples \
        -xplatform linux-riscv64-gnu-g++ \
        -prefix /opt/Qt-5.12.12 \
        -extprefix $HOST_OUTPUT
	make -j$PROCESSORS
	make install
    if [ ! -d "$TARGET_OUTPUT/opt" ]; then
        mkdir $TARGET_OUTPUT/opt
    fi
    if [ ! -d "$TARGET_OUTPUT/opt/Qt-5.12.12" ]; then
        mkdir $TARGET_OUTPUT/opt/Qt-5.12.12
    fi
    cp -r $HOST_OUTPUT/lib $TARGET_OUTPUT/opt/Qt-5.12.12/lib
    cp -r $HOST_OUTPUT/plugins $TARGET_OUTPUT/opt/Qt-5.12.12/plugins
    cp -r $HOST_OUTPUT/translations $TARGET_OUTPUT/opt/Qt-5.12.12/translations
    rm -rf $TARGET_OUTPUT/opt/Qt-5.12.12/lib/cmake
    rm -rf $TARGET_OUTPUT/opt/Qt-5.12.12/lib/pkgconfig
    rm -rf $TARGET_OUTPUT/opt/Qt-5.12.12/lib/*.prl
    rm -rf $TARGET_OUTPUT/opt/Qt-5.12.12/lib/*.a
    rm -rf $TARGET_OUTPUT/opt/Qt-5.12.12/lib/*.la
}

build_qt
 
echo "----------------------------------- 完成 -----------------------------------"
