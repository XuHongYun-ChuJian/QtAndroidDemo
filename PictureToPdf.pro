QT += core gui
QT += core5compat
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "PictureToPdf"
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {
    QT += core-private
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/res/values/libs.xml
    OTHER_FILES += \
#        android/src/org/qtproject/example/utils/JavaUtils.java

    HEADERS += \
        JavaUtils/javautils.h

    SOURCES += \
        JavaUtils/javautils.cpp
}
