#QT -= gui

QT += network gui

CONFIG += c++11 console exception
CONFIG -= app_bundle

#QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
#QMAKE_CXXFLAGS_STL_ON = /EHa


DEFINES += QT_DEPRECATED_WARNINGS


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cameracontroller.cpp \
        cameramainlooper.cpp \
        main.cpp \
        mytcpserver.cpp \
        requestmapper.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cameracontroller.h \
    cameramainlooper.h \
    mytcpserver.h \
    requestmapper.h


include(QtWebApp/QtWebApp/httpserver/httpserver.pri)
#include(webapp.ini)

DISTFILES += \
    build/webapp.ini


# ////////////////////////////



SOURCES += \
    gphoto2pp_src/camera_abilities_list_wrapper.cpp \
    gphoto2pp_src/camera_file_wrapper.cpp \
    gphoto2pp_src/camera_list_wrapper.cpp \
    gphoto2pp_src/camera_widget_wrapper.cpp \
    gphoto2pp_src/camera_wrapper.cpp \
    gphoto2pp_src/choices_widget.cpp \
    gphoto2pp_src/date_widget.cpp \
    gphoto2pp_src/float_widget.cpp \
    gphoto2pp_src/gp_port_info_list_wrapper.cpp \
    gphoto2pp_src/helper_camera_wrapper.cpp \
    gphoto2pp_src/helper_context.cpp \
    gphoto2pp_src/helper_debugging.cpp \
    gphoto2pp_src/helper_gphoto2.cpp \
    gphoto2pp_src/helper_widgets.cpp \
    gphoto2pp_src/int_widget.cpp \
    gphoto2pp_src/menu_widget.cpp \
    gphoto2pp_src/non_value_widget.cpp \
    gphoto2pp_src/radio_widget.cpp \
    gphoto2pp_src/range_widget.cpp \
    gphoto2pp_src/section_widget.cpp \
    gphoto2pp_src/string_widget.cpp \
    gphoto2pp_src/text_widget.cpp \
    gphoto2pp_src/toggle_widget.cpp \
    gphoto2pp_src/window_widget.cpp



INCLUDEPATH += $${PWD} \
    gphoto2pp/



DEPENDPATH += $${PWD} \
    gphoto2pp/



macx:{
    LIBS += -L/usr/local/Cellar/libgphoto2/2.5.27/lib/ -lgphoto2_port
    LIBS += -L/usr/local/Cellar/libgphoto2/2.5.27/lib/ -lgphoto2

#    INCLUDEPATH += /usr/local/Cellar/libgphoto2/2.5.27/include
#    DEPENDPATH += /usr/local/Cellar/libgphoto2/2.5.27/include

    CONFIG+=sdk_no_version_check

}

