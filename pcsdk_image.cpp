#include "pcsdk_image.h"
#include <opencv2/opencv.hpp>
#include <QImage>

namespace MontBlanc
{

PCSDKImage::PCSDKImage(OpenCVImage *parent) :
    OpenCVImage(parent), minDistance_(0), maxDistance_(1000)
{
    setup();
}

PCSDKImage::~PCSDKImage()
{
    if (pcsdk_->isRunning()) {
        pcsdk_->stop();
    }
}

void PCSDKImage::setup()
{
    pcsdk_ = PCSDK::GetInstance();
    pcsdk_->start();
}


void PCSDKImage::updateColorImage()
{
}


void PCSDKImage::updateDepthImage()
{
    auto depthImage = pcsdk_->getDepthImage();

    const size_t imageSize = PCSDK::DEPTH_WIDTH * PCSDK::DEPTH_HEIGHT;
    cv::Mat colorImage(PCSDK::DEPTH_HEIGHT, PCSDK::DEPTH_WIDTH, CV_8UC3);

    for (int i = 0; i < PCSDK::DEPTH_WIDTH; ++i) {
        for (int j = 0; j < PCSDK::DEPTH_HEIGHT; ++j) {
            const int index = PCSDK::DEPTH_HEIGHT * i + j;
            /*
            const double maxLength = 600; // mm
            const double data = depthImage[index];
            const double base = maxLength / 3;

            double b = 0, g = 0, r = 0;
            if (data < base) {
                 b = data / base * 255;
            } else if (data < 2 * base) {
                g = (data - base) / base * 255;
                b = 255 - g;
            } else if (data < 3 * base) {
                r = (data - 2 * base) / base * 255;
                g = 255 - r;
            } else {
                r = 255;
            }
            colorImage[3*(240*i + j) + 0] = r;
            colorImage[3*(240*i + j) + 1] = g;
            colorImage[3*(240*i + j) + 2] = b;
            */
            const double data = depthImage[index];
            const double depthColor = (data > minDistance_ && data < maxDistance_) ?
                        (data - minDistance_) / (maxDistance_ - minDistance_) * 255 : 0;
            colorImage.data[3*index] = colorImage.data[3*index + 1] = colorImage.data[3*index + 2] = depthColor;
        }
    }

    image_ = colorImage;
    emit imageChanged();
}


void PCSDKImage::paint(QPainter *painter)
{
    updateDepthImage();
    OpenCVImage::paint(painter);
}

}
