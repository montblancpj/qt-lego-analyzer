SOURCES += \
	homography_image.cpp \
	opencv_image.cpp \
	openni_image.cpp \
	level_analyzed_image.cpp

HEADERS += \
	homography_image.h \
	opencv_image.h \
	openni_image.h \
	level_analyzed_image.h

win32 {

	SOURCES += pcsdk_image.cpp
	HEADERS += pcsdk_image.h

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

	QMAKE_CFLAGS += -stdlib=libstdc++
	QMAKE_CFLAGS_WARN_OFF += -Woverloaded-virtual

}

OTHER_FILES = qmldir

