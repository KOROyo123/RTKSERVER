#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T08:58:44
#
# src.pro工程文件 Rtklib2.4.3 b33 Emlid
# src的文件夹的源码部分替换为2.4.3 b34版本源码
# 新加了部分 -Wno-error= 使得程序能够在linux和win平台下的qt运行并编译生成可执行程序
# 在编译器版本Qt 6.1.2 GCC 64bit 和 Qt 6.2.1 MinGW 64bit 下可以正常运行
#
#-------------------------------------------------

QT       -= core gui

TARGET = RTKLib
TEMPLATE = lib
CONFIG += staticlib

include(../RTKLib.pri)
INCLUDEPATH +=

DESTDIR = $${OUT_PWD}

QMAKE_CFLAGS += -std=c99 -pedantic -Wall -Werror -fpic -fno-strict-overflow \
    -Wno-error=unused-but-set-variable -Wno-error=unused-function \
    -Wno-error=unused-result -Wno-error=pointer-to-int-cast \
    -Wno-error=unused-variable -Wno-error=int-conversion \
    -Wno-error=stringop-truncation -Wno-error=format-overflow \
    -Wno-error=maybe-uninitialized -Wno-error=array-bounds
QMAKE_CFLAGS_DEBUG = -O0 -g
QMAKE_CFLAGS_RELEASE = -O3
QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO = -O3 -g
CONFIG -= warn_on

#CONFIG(release, debug|release) {
#    DEFINES += NDEBUG
#}

DEFINES -= UNICODE

SOURCES += rtkcmn.c \
    convkml.c \
    convrnx.c \
    convgpx.c \
    datum.c \
    download.c \
    ephemeris.c \
    geoid.c \
    gis.c \
    ionex.c \
    lambda.c \
    options.c \
    pntpos.c \
    postpos.c \
    ppp.c \
    ppp_ar.c \
    preceph.c \
    rcvraw.c \
    rinex.c \
    rtcm.c \
    rtcm2.c \
    rtcm3.c \
    rtcm3e.c \
    rtkpos.c \
    rtksvr.c \
    sbas.c \
    solution.c \
    stream.c \
    streamsvr.c \
    tides.c \
    tle.c \
    rcv/binex.c \
    rcv/crescent.c \
    rcv/javad.c \
    rcv/novatel.c \
    rcv/nvs.c \
    rcv/rt17.c \
    rcv/septentrio.c \
    rcv/skytraq.c \
    rcv/ss2.c \
    rcv/ublox.c \


HEADERS += rtklib.h \


unix {
    target.path = /usr/lib
    INSTALLS += target
}
