INCLUDEPATH += $$PWD

SOURCES += \
	$$PWD/homography_image.cpp \
	$$PWD/opencv_image.cpp \
	$$PWD/openni_image.cpp \
	$$PWD/level_analyzed_image.cpp

HEADERS += \
	$$PWD/homography_image.h \
	$$PWD/opencv_image.h \
	$$PWD/openni_image.h \
	$$PWD/level_analyzed_image.h

win32 {

	SOURCES += $$PWD/pcsdk_image.cpp
	HEADERS += $$PWD/pcsdk_image.h

	QMAKE_INCDIR += \
		$$PWD/include \
		$$(PCSDK_DIR)/include \
		$$(PCSDK_DIR)/sample/common/include \
		$$(OPENCV_DIR)/include \
		$$(OPENNI2_INCLUDE)

	QMAKE_LIBDIR += \
		$$PWD/lib \
		$$(OPENCV_DIR)/x86/vc11/lib \
		$$(OPENNI2_LIB)

	Release:QMAKE_LIBS += \
		-lopencv_core249 -lopencv_highgui249 -lopencv_imgproc249 -lopencv_calib3d249 -lPCSDK -lOpenNI2
	Debug:QMAKE_LIBS += \
		-lopencv_core249d -lopencv_highgui249d -lopencv_imgproc249d -lopencv_calib3d249d -lOpenNI2 -lPCSDK

} macx {

	QMAKE_INCDIR += \
		$$PWD/include \
		/usr/local/include \
		/usr/local/include/ni2

	QMAKE_LIBDIR += \
		/usr/local/lib \
		/usr/local/lib/ni2

	QMAKE_LIBS += \
		-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lOpenNI2
}

QMAKE_CXXFLAGS_WARN_OFF += -Wno-overloaded-virtual

