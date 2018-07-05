#-------------------------------------------------
# Library Druai
#   Created 05/20/2014
#   Tim Craig - Druai Consulting
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = Druai
TEMPLATE = lib

debug {
   DESTDIR = debug
   OBJECTS_DIR = debug/obj
   UI_DIR = ui
   MOC_DIR = moc
   }
else {
   DESTDIR = release
   OBJECTS_DIR = release/obj
   UI_DIR = ui
   MOC_DIR = moc
   }

QT += multimedia multimediawidgets

include(D:/Projects/Workspace/ProjectsCommon/Boost.pri)
include(D:/Projects/Workspace/ProjectsCommon/OpenCV.pri)

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
      DCameraMainWindow.cpp \
      DCVCameraMainWindow.cpp \
      ImageWidget.cpp \
      DQImage.cpp \
      DQCameraHandler.cpp \
      DCVCameraHandler.cpp \
      DHistogram.cpp \
      DQHistogramWidget.cpp \
      DQRubberBand.cpp \
      DQCVImageUtils.cpp \
      CVImage.cpp \
      DQOpenCV.cpp \
      CameraCalibration.cpp \
      DPersistentMainWindow.cpp

HEADERS += \
      DCameraMainWindow.h \
      DCVCameraMainWindow.h \
      ImageWidget.h \
      DQImage.h \
      DQCameraHandler.h \
      DCVCameraHandler.h \
      DHistogram.h \
      DQHistogramWidget.h \
      DQRubberBand.h \
      DQCVImageUtils.h \
      CVImage.h \
      DQOpenCV.h \
      CameraCalibration.h \
      DPersistentMainWindow.h


FORMS += \
      DCameraMainWindow.ui \
      DCVCameraMainWindow.ui

CONFIG += c++11 staticlib

RESOURCES += \
    Druai.qrc

win32 {
CONFIG(debug, debug|release) {
   target.path = D:/Projects/Workspace/Lib/debug
   }
CONFIG(release, debug|release) {
   target.path = D:/Projects/Workspace/Lib/release
   }
}

unix {
    target.path = /usr/lib
}
INSTALLS += target
