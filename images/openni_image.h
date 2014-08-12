#ifndef OPENNI_IMAGE_H
#define OPENNI_IMAGE_H

#include "opencv_image.h"
#include <OpenNI.h>
#include <memory>

namespace MontBlanc
{

class IRImageListener : public openni::VideoStream::NewFrameListener
{
public:
    const cv::Mat& getImage() const;

private:
    void onNewFrame(openni::VideoStream& stream) override;
    cv::Mat image_;
    mutable std::mutex mutex_;
};

class OpenNIImage : public OpenCVImage
{
    Q_OBJECT

public:
    explicit OpenNIImage(OpenCVImage *parent = nullptr);
    ~OpenNIImage();

    void initialize();
    void shutdown();
    Q_INVOKABLE bool isOpen() const;

    void paint(QPainter *painter) override;

private:
    static int instanceNum;
    static std::shared_ptr<openni::Device> Device;
    static std::shared_ptr<openni::VideoStream> Stream;
    static std::shared_ptr<IRImageListener> Listener;
    static bool IsOpen;

signals:
    void open() const;
    void close() const;
};

}

#endif // OPENNI_IMAGE_H
