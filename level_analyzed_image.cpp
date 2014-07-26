#include "level_analyzed_image.h"
#include <array>

namespace MontBlanc {

LevelAnalyzedImage::LevelAnalyzedImage(QQuickItem *parent)
    : OpenCVImage(parent), contrast_(5.0)
{
}

void LevelAnalyzedImage::analyze()
{
    if (inputImage_.empty()) return;

    cv::Mat grayImage;
    {
        std::lock_guard<std::mutex> lock(inputImageMutex_);
        cv::cvtColor(inputImage_, grayImage, CV_BGR2GRAY);
    }

    // Noise reduction
    cv::medianBlur(grayImage, grayImage, 5);

    // Contrast collection
    cv::Mat lut(1, 256, CV_8U);
    for (int i = 0; i < 256; ++i) {
        lut.at<unsigned char>(i) = (i < 128) ? (std::pow(i, 2) / 128) : (256 - std::pow(i, 2) / std::pow(256, 2));
        // lut.at<unsigned char>(i) = 255 / (1 + std::exp(-contrast_ * (i - 128) / 255));
    }
    cv::LUT(grayImage, lut, grayImage);

    // Check each cell color
    result_.clear();
    for (auto&& data : targetRects_) {
        const auto id = data.first;
        auto rect = data.second;

        // To fill gaps
        rect.width  += 1;
        rect.height += 1;

        // Check ROI
        if ( !(rect.x >= 0 && rect.width  >= 0 && rect.x + rect.width  <= grayImage.cols &&
               rect.y >= 0 && rect.height >= 0 && rect.y + rect.height <= grayImage.rows) ) {
            qDebug() << "targetRects[" << id << "] is invalid rect data.";
            continue;
        }

        // Calculate average color
        cv::Mat roiImage = grayImage(rect);
        rect.x += rect.width  / 3;
        rect.y += rect.height / 3;
        rect.width  /= 3;
        rect.height /= 3;
        cv::Mat averageRoiImage = grayImage(rect);
        cv::Mat rowAverage, average;
        cv::reduce(averageRoiImage, rowAverage, 0, CV_REDUCE_AVG);
        cv::reduce(rowAverage, average,  1, CV_REDUCE_AVG);

        // Set pixels in ROI as its average color
        auto averageValue = average.at<unsigned char>(0);
        roiImage = cv::Scalar( averageValue );

        // Set results
        result_[id] = averageValue;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        cv::cvtColor(grayImage, image_, CV_GRAY2BGR);
    }

    emit resultChanged();
    emit update();
}

void LevelAnalyzedImage::setTargetRects(const QVariantList& targetRects)
{
    // [ { id: 2, x: 0.1, y: 0.1, width: 0.1, height: 0.2 }, ...]
    targetRects_.clear();

    for (auto&& data : targetRects) {
        const auto targetRect = data.value<QVariantMap>();

        cv::Rect rect;
        rect.x      = static_cast<int>(targetRect["x"].value<double>()      * image_.cols);
        rect.y      = static_cast<int>(targetRect["y"].value<double>()      * image_.rows);
        rect.width  = static_cast<int>(targetRect["width"].value<double>()  * image_.cols);
        rect.height = static_cast<int>(targetRect["height"].value<double>() * image_.rows);

        const auto id = targetRect["id"].value<int>();
        targetRects_[id] = rect;
    }

    emit targetRectsChanged();
}

QVariantList LevelAnalyzedImage::targetRects() const
{
    QVariantList targetRects;

    for (auto&& rect : targetRects_) {
        const auto key   = rect.first;
        const auto value = rect.second;

        QVariantMap targetRect;
        targetRect.insert("id",     key);
        targetRect.insert("x",      value.x);
        targetRect.insert("y",      value.y);
        targetRect.insert("width",  value.width);
        targetRect.insert("height", value.height);

        targetRects.push_back(targetRect);
    }

    return targetRects;
}

QVariantMap LevelAnalyzedImage::result() const
{
    QVariantMap result;
    for (auto&& idValue : result_) {
        auto id  = idValue.first;
        auto val = idValue.second > 100 ? 1 : 0;
        result.insert(QString::number(id), val);
    }
    return result;
}

void LevelAnalyzedImage::setInputImage(const QVariant& image)
{
    {
        std::lock_guard<std::mutex> lock(inputImageMutex_);
        inputImage_ = image.value<cv::Mat>();
    }
    emit inputImageChanged();
}

QVariant LevelAnalyzedImage::inputImage() const
{
    return QVariant::fromValue(inputImage_);
}

}