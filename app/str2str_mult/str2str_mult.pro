TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include(../../RTKLib.pri)

TARGET = str2str_mult_v1.6.4
TEMPLATE = app

INCLUDEPATH += ../../src


linux{
    RTKLIB =../../src/libRTKLib.a
    LIBS +=  $${RTKLIB}
# 因为pthread的库不是Linux系统的库，所以在进行编译的时候要加上：-lpthread
    LIBS += -lpthread
}
macx{
    RTKLIB =../../src/libRTKLib.a
    LIBS += /usr/local/lib/libpng.a $${RTKLIB}
}
win32 {
    RTKLIB =../../src/libRTKLib.a
    LIBS+= $${RTKLIB} -lWs2_32 -lwinmm
}

PRE_TARGETDEPS = $${RTKLIB}


SOURCES += \
        main.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



