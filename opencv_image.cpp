#include "opencv_image.h"
#include <array>

namespace MontBlanc
{


OpenCVImage::OpenCVImage(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}


QVariant OpenCVImage::image() const
{
    return QVariant::fromValue(image_);
}


void OpenCVImage::setImage(const QVariant &image)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        image_ = image.value<cv::Mat>();
    }

    emit imageChanged();
    emit update();
}


QString OpenCVImage::filePath() const
{
    return filePath_;
}


void OpenCVImage::setFilePath(const QString& path)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        filePath_ = path;
        image_ = cv::imread( path.toStdString() );
        if ( image_.empty() ) {
            emit error(path + "is not found");
            return;
        }
    }

    emit filePathChanged();
    emit imageChanged();
}


void OpenCVImage::paint(QPainter *painter)
{

    if ( image_.empty() ) return;

    // Scaling to QML Element size
    /*
    if (image_.cols != width() || image_.rows != height()) {
        cv::Mat scaledImg(height(), width(), image_.type());
        cv::resize(image_, scaledImg, scaledImg.size(), cv::INTER_CUBIC);
        image_ = scaledImg;
    }
    */
    cv::Mat scaledImage(height(), width(), image_.type());
    {
        std::lock_guard<std::mutex> lock(mutex_);
        cv::resize(image_, scaledImage, scaledImage.size(), cv::INTER_CUBIC);
    }

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
