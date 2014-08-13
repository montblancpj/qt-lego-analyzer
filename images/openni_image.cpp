#include "openni_image.h"

namespace MontBlanc
{


/* --------------------------------------------------------------------------------
 * IRImageListener
 * -------------------------------------------------------------------------------- */
void IRImageListener::onNewFrame(openni::VideoStream &stream)
{
    std::lock_guard<std::mutex> lock(mutex_);

    openni::VideoFrameRef frame;
    stream.readFrame(&frame);

    if (!frame.isValid()) {
        qDebug() << "IR frame is invalid.";
        return;
    }

    cv::Mat image(frame.getHeight(), frame.getWidth(),
                  CV_16U, static_cast<char*>( const_cast<void*>(frame.getData())) );
    image.convertTo(image, CV_8U);
    cv::flip(image, image, 1);

    image_ = image;
}


const cv::Mat& IRImageListener::getImage() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return image_;
}


/* --------------------------------------------------------------------------------
 * ColorImageListener
 * -------------------------------------------------------------------------------- */
void ColorImageListener::onNewFrame(openni::VideoStream &stream)
{
    std::lock_guard<std::mutex> lock(mutex_);

    openni::VideoFrameRef frame;
    stream.readFrame(&frame);

    if (!frame.isValid()) {
        qDebug() << "Color frame is invalid.";
        return;
    }

    cv::Mat image(frame.getHeight(), frame.getWidth(),
                  CV_8UC3, static_cast<char*>( const_cast<void*>(frame.getData())) );
    cv::flip(image, image, 1);

    image_ = image;
}


const cv::Mat& ColorImageListener::getImage() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return image_;
}


/* --------------------------------------------------------------------------------
 * OpenNIImage
 * -------------------------------------------------------------------------------- */
std::shared_ptr<openni::Device>      OpenNIImage::Device        = nullptr;
std::shared_ptr<IRImageListener>     OpenNIImage::IrListener    = nullptr;
std::shared_ptr<openni::VideoStream> OpenNIImage::IrStream      = nullptr;
std::shared_ptr<ColorImageListener>  OpenNIImage::ColorListener = nullptr;
std::shared_ptr<openni::VideoStream> OpenNIImage::ColorStream   = nullptr;
int  OpenNIImage::instanceNum = 0;
bool OpenNIImage::IsOpen      = false;


OpenNIImage::OpenNIImage(OpenCVImage* parent)
    : OpenCVImage(parent), isColor_(false)
{
    if (instanceNum == 0) {
        initialize();
    }
    ++instanceNum;
}


OpenNIImage::~OpenNIImage()
{
    --instanceNum;
    if (instanceNum == 0) {
        shutdown();
    }
}


bool OpenNIImage::isOpen() const
{
    return IsOpen;
}


void OpenNIImage::initialize()
{
    openni::OpenNI::initialize();

    Device = std::make_shared<openni::Device>();
    if (Device->open(openni::ANY_DEVICE) != openni::STATUS_OK) {
        emit error(openni::OpenNI::getExtendedError());
        return;
    }

    IrListener = std::make_shared<IRImageListener>();
    IrStream = std::make_shared<openni::VideoStream>();
    IrStream->create(*Device, openni::SENSOR_IR);
    IrStream->addNewFrameListener(IrListener.get());
    auto irMode = IrStream->getVideoMode();
    irMode.setResolution(640, 480);
    irMode.setFps(30);
    IrStream->setVideoMode(irMode);
    IrStream->start();

    // Color Streams DOES NOT work with IR cam. ...x(
    ColorListener = std::make_shared<ColorImageListener>();
    ColorStream = std::make_shared<openni::VideoStream>();
    ColorStream->create(*Device, openni::SENSOR_COLOR);
    ColorStream->addNewFrameListener(ColorListener.get());
    auto colorMode = IrStream->getVideoMode();
    colorMode.setResolution(640, 480);
    colorMode.setFps(30);
    ColorStream->setVideoMode(colorMode);
    ColorStream->start();

    IsOpen = true;
    emit open();
}


void OpenNIImage::shutdown()
{
    openni::OpenNI::shutdown();
    IsOpen = false;
    emit close();
}


void OpenNIImage::paint(QPainter *painter){
    if ( isOpen() ) {
        if (!isColor_) {
            auto image = IrListener->getImage();
            cv::cvtColor(image, image_, CV_GRAY2BGR);
            emit imageChanged();
        } else { // color stream does not work...
            image_ = ColorListener->getImage();
            emit imageChanged();
        }
    }
    OpenCVImage::paint(painter);
}


}
