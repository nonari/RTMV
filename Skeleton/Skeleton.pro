

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Example linking with Basler and Opencv under Windows
##########################################################
#
#INCLUDEPATH +=  J:/opencv/build/include/opencv2 \
#                J:/opencv/build/include \
#                J:/Basler505/Development/include/genapi \
#                J:/Basler505/Development/include
#
#LIBS+=  -LJ:/Basler505/Development/lib/x64 \ #Basler
#        -LJ:/opencv/build/x64/vc15/lib \
#        -lWinmm
## Add J:\opencv\build\x64\vc15\bin to PATH in Windows's Control Panel
#
#CONFIG(debug, debug|release) {
#
#    LIBS+=  -lopencv_world430d
#}
#
#
#CONFIG(release, debug|release) {
#
#    LIBS+=  -lopencv_world430
#}
############################################################



# Example linking with Basler and OpenCV (static) under Linux
#################################################################
#
QMAKE_CXXFLAGS += -D WITH_TIFF=ON -D BUILD_TIFF=ON
#QMAKE_LFLAGS += -no-pie -static-libgcc
#
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas # Avoids zillions of "Unkown pragma warning" from Pylon
#
#QMAKE_CXXFLAGS += -std=c++11
#
INCLUDEPATH +=  /usr/local/include/opencv4 \
                /opt/pylon/include
#
#QMAKE_LFLAGS += -Wl,--enable-new-dtags -Wl,-rpath,/opt/pylon5/lib64
#
LIBS+=  -L/opt/pylon/lib \
        -Wl,-E -lpylonbase -lpylonutility -lGenApi_gcc_v3_1_Basler_pylon -lGCBase_gcc_v3_1_Basler_pylon \
        -L/usr/local/lib \
         -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco \
         -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_datasets -lopencv_dpm -lopencv_face -lopencv_freetype \
         -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_video -lopencv_plot -lopencv_reg \
         -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd  \
         -lopencv_surface_matching -lopencv_text -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann \
         -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs \
         -lopencv_photo -lopencv_imgproc -lopencv_core \
#        -L/home/calderon/OpenCVStatic/share/OpenCV/3rdparty/lib \
#        -lIlmImf -llibjasper \
#        -lz -lippiw -lippicv -littnotify
#################################################################


TARGET = Skeleton
TEMPLATE = app

# emits warnings if code uses any Qt feature marked as deprecated
DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

SOURCES += \
        configuration.cpp \
        globals.cpp \
        main.cpp \
        mainwindow.cpp \
        worker.cpp

HEADERS += \
        configuration.h \
        globals.h \
        mainwindow.h \
        worker.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
