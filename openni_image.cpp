#include "openni_image.h"

namespace MontBlanc
{

/* --------------------------------------------------------------------------------
 * IRImageListener
 * -------------------------------------------------------------------------------- */
void IRImageListener::onNewFrame(openni::VideoStream &stream)
{
    openni::VideoFrameRef frame;
    stream.readFrame(&frame);

    if (!frame.isValid()) {
        qDebug() << "Frame is invalid.";
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
    return image_;
}

/* --------------------------------------------------------------------------------
 * OpenNIImage
 * -------------------------------------------------------------------------------- */
std::shared_ptr<IRImageListener> OpenNIImage::Listener = nullptr;
std::shared_ptr<openni::Device> OpenNIImage::Device = nullptr;
std::shared_ptr<openni::VideoStream> OpenNIImage::Stream = nullptr;
int OpenNIImage::instanceNum = 0;
bool OpenNIImage::IsOpen = false;

OpenNIImage::OpenNIImage(OpenCVImage* parent)
    : OpenCVImage(parent)
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

    Listener = std::make_shared<IRImageListener>();
    Stream = std::make_shared<openni::VideoStream>();
    Stream->create(*Device, openni::SensorType::SENSOR_IR);
    Stream->addNewFrameListener(Listener.get());
    Stream->start();

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
    if (isOpen()) {
        auto image = Listener->getImage();
        cv::cvtColor(image, image_, CV_GRAY2BGR);
        emit imageChanged();
    }
    OpenCVImage::paint(painter);
}

}
