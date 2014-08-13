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


class ColorImageListener : public openni::VideoStream::NewFrameListener
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
    Q_PROPERTY(bool isColor MEMBER isColor_)

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
    static std::shared_ptr<openni::VideoStream> IrStream;
    static std::shared_ptr<IRImageListener> IrListener;
    static std::shared_ptr<openni::VideoStream> ColorStream;
    static std::shared_ptr<ColorImageListener> ColorListener;
    static bool IsOpen;
    bool isColor_;

signals:
    void open() const;
    void close() const;
};


}

#endif // OPENNI_IMAGE_H
