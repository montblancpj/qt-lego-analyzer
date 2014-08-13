#include "level_analyzed_image.h"
#include <array>

namespace MontBlanc
{


LevelAnalyzedImage::LevelAnalyzedImage(QQuickItem *parent)
    : OpenCVImage(parent), checkFrame_(10), contrast_(5.0), threshold_(128)
{
}


void LevelAnalyzedImage::analyze()
{
    if ( inputImage_.empty() ) return;

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
    std::vector<int> heights;
    for (auto rect : targetRects_) { // copy
        // To fill gaps
        rect.width  += 1;
        rect.height += 1;

        // Check ROI
        if ( !(rect.x >= 0 && rect.width  >= 0 && rect.x + rect.width  <= grayImage.cols &&
               rect.y >= 0 && rect.height >= 0 && rect.y + rect.height <= grayImage.rows) ) {
            qDebug() << "targetRects contains invalid rect data.";
            continue;
        }

        // Calculate average color
        cv::Mat roiImage = grayImage(rect);
        if (rect.width > 3 && rect.height > 3) {
            rect.x += rect.width  / 3;
            rect.y += rect.height / 3;
            rect.width  /= 3;
            rect.height /= 3;
        }
        cv::Mat averageRoiImage = grayImage(rect);
        cv::Mat rowAverage, average;
        cv::reduce(averageRoiImage, rowAverage, 0, CV_REDUCE_AVG);
        cv::reduce(rowAverage, average,  1, CV_REDUCE_AVG);

        // Set all pixels in ROI as its average color
        auto averageValue = average.at<unsigned char>(0);
        roiImage = cv::Scalar(averageValue);

        // Set results
        auto brickDetected = averageValue > threshold_ ? 1 : 0;
        heights.push_back(brickDetected);
    }

    // check change
    addHeightsCache(heights);
    checkResult();

    {
        std::lock_guard<std::mutex> lock(mutex_);
        cv::cvtColor(grayImage, image_, CV_GRAY2BGR);
        emit imageChanged();
    }

    emit update();
}


void LevelAnalyzedImage::setTargetRects(const QVariantList& targetRects)
{
    // [ { x: 0.1, y: 0.1, width: 0.1, height: 0.2 }, ...]
    targetRects_.clear();
    ignoreRectMap_.clear();

    for (auto&& data : targetRects) {
        const auto targetRect = data.value<QVariantMap>();

        cv::Rect rect;
        rect.x       = static_cast<int>(targetRect["x"].value<double>()      * image_.cols);
        rect.y       = static_cast<int>(targetRect["y"].value<double>()      * image_.rows);
        rect.width   = static_cast<int>(targetRect["width"].value<double>()  * image_.cols);
        rect.height  = static_cast<int>(targetRect["height"].value<double>() * image_.rows);
        bool ignored = targetRect["isIgnored"].value<int>() > 0;

        targetRects_.push_back(rect);
        ignoreRectMap_.push_back(ignored);
    }

    emit targetRectsChanged();
}


QVariantList LevelAnalyzedImage::targetRects() const
{
    QVariantList targetRects;

    for (auto&& rect : targetRects_) {
        QVariantMap targetRect;
        targetRect.insert("x",      rect.x);
        targetRect.insert("y",      rect.y);
        targetRect.insert("width",  rect.width);
        targetRect.insert("height", rect.height);
        targetRects.push_back(targetRect);
    }

    return targetRects;
}


QVariantList LevelAnalyzedImage::result() const
{
    QVariantList result;
    for (auto&& value : result_) {
        result.append( QVariant(value) );
    }
    return result;
}


QVariantList LevelAnalyzedImage::heights() const
{
    QVariantList heights;
    for (auto&& value : lastHeights_) {
        heights.append( QVariant(value) );
    }
    return heights;
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


void LevelAnalyzedImage::addHeightsCache(const std::vector<int> &heights)
{
    heightsCache_.push_front(heights);
    while (heightsCache_.size() > checkFrame_) {
        heightsCache_.pop_back();
    }
}


void LevelAnalyzedImage::checkResult()
{
    // first time
    if (heightsCache_.size() == 0) {
        return;
    }

    if (lastHeights_.empty()) {
        lastHeights_ = std::vector<int>(targetRects_.size(), 0);
    }

    bool isSteady = true;
    for (int i = 1; i < heightsCache_.size(); ++i) {
        for (int j = 0; j < heightsCache_[i].size() && j < heightsCache_[i-1].size(); ++j) {
            bool changed = heightsCache_[i][j] != heightsCache_[i-1][j];
            bool ignored = ignoreRectMap_[j];
            if (changed && !ignored) {
                isSteady = false;
                break;
            }
        }
        if (!isSteady) break;
    }

    if (isSteady) {
        // result     = { 0, 0, 0, 1, 1, 0, ... }
        // lastResult = { 1, 1, 0, 0, 1, 0, ... }
        //          --> {-1,-1, 0, 1, 0, 0, ... }
        auto currentHeights = heightsCache_[0];
        std::vector<int> newResult( currentHeights.size() );
        bool isResultChanged = false;
        for (int i = 0; i < currentHeights.size() && i < lastHeights_.size(); ++i) {
            auto deltaHeight = currentHeights[i] - lastHeights_[i];
            if (deltaHeight != 0) isResultChanged = true;
            newResult[i] = deltaHeight;
        }
        if (isResultChanged) {
            lastHeights_ = currentHeights;
            result_ = newResult;
            emit resultChanged();
            emit heightsChanged();
        }
    }
}


}
