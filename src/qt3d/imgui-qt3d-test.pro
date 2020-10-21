TEMPLATE = app
TARGET = imgui-qt3d-test
BINDIR = $$OUT_PWD/../../bin
DESTDIR = $$BINDIR
LIBS += -L"$$BINDIR" -limgui-qt3d
PRE_TARGETDEPS += $$BINDIR/$${QMAKE_PREFIX_STATICLIB}imgui-qt3d.$${QMAKE_EXTENSION_STATICLIB}

QT += 3dcore 3drender 3dinput 3dlogic 3danimation 3dextras widgets

INCLUDEPATH += $$PWD/../imgui-qt3d $$PWD/../3rdparty/imgui

SOURCES += \
    main.cpp \
    gui.cpp \
    mainwindow.cpp

HEADERS += \
    gui.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
