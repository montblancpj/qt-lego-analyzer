#include "opencv_image.h"
#include <array>

namespace MontBlanc
{

OpenCVImage::OpenCVImage(QQuickItem *parent) :
    QQuickPaintedItem(parent), gamma_(1.0)
{
}

QVariant OpenCVImage::image() const
{
    return QVariant::fromValue(image_);
}

void OpenCVImage::setImage(const QVariant &image)
{
    image_ = image.value<cv::Mat>();
    applyGammeCorrection();

    emit imageChanged();
    emit update();
}

void OpenCVImage::applyGammeCorrection()
{
    // 修正する...
    if (gamma_ == 1.0 || image_.empty()) {
        return;
    }

    std::array<int, 256> lut;
    for (int i = 0; i < 256; ++i) {
        lut[i] = i * 2;
    }

    cv::Mat hsvImage;
    //cv::cvtColor(image_, hsvImage, CV_BGR2HSV);
    hsvImage = image_.clone();
    std::vector<cv::Mat> hsvVec;
    cv::split(hsvImage, hsvVec);
    cv::LUT(hsvVec[0], cv::Mat(1, lut.size(), CV_8U, lut.data()), hsvVec[0]);
    cv::LUT(hsvVec[1], cv::Mat(1, lut.size(), CV_8U, lut.data()), hsvVec[1]);

    cv::merge(hsvVec, hsvImage);

    //cv::cvtColor(hsvImage, image_, CV_HSV2BGR);
    image_ = hsvImage;
}

void OpenCVImage::paint(QPainter *painter)
{
    if (image_.empty()) return;

    // Scaling to QML Element size
    /*
    if (image_.cols != width() || image_.rows != height()) {
        cv::Mat scaledImg(height(), width(), image_.type());
        cv::resize(image_, scaledImg, scaledImg.size(), cv::INTER_CUBIC);
        image_ = scaledImg;
    }
    */
    cv::Mat scaledImage(height(), width(), image_.type());
    cv::resize(image_, scaledImage, scaledImage.size(), cv::INTER_CUBIC);

    // BGR -> ARGB
    cv::cvtColor(scaledImage, scaledImage, CV_BGR2BGRA);
    std::vector<cv::Mat> bgra;
    cv::split(scaledImage, bgra);
    std::swap(bgra[0], bgra[3]);
    std::swap(bgra[1], bgra[2]);

    QImage outputImage(scaledImage.data, scaledImage.cols, scaledImage.rows, QImage::Format_ARGB32);
    painter->drawImage(0, 0, outputImage);
}

}
