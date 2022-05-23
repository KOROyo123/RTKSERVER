QT -= gui
QT += core sql

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../../RTKLib.pri)

TARGET = RTKSERVER_v2.0
TEMPLATE = app

INCLUDEPATH += ../../src
INCLUDEPATH += ../../koro


linux{
    RTKLIB =../../src/libRTKLib.a
    LIBS +=  $${RTKLIB}
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
    interface.cpp \
        main.cpp \
    navi.cpp \
    sql_trans.cpp \
    svr_core.cpp \
    sys_ctrl.cpp



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    interface.h \
    navi.h \
    sql_trans.h \
    svr_core.h \
    sys_ctrl.h


