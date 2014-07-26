#include "homography_image.h"

namespace MontBlanc
{

HomographyImage::HomographyImage(OpenCVImage *parent) :
    OpenCVImage(parent)
{
}

void HomographyImage::setImage(const QVariant& image)
{
    if (srcPoints_.length() == 0) {
        OpenCVImage::setImage(image);
        return;
    }

    cv::Mat srcImage = image.value<cv::Mat>();

    // Scaling to QML Element size
    /*
    if (srcImage.cols != width() || srcImage.rows != height()) {
        cv::Mat scaledImg(height(), width(), srcImage.type());
        cv::resize(srcImage, scaledImg, scaledImg.size(), cv::INTER_CUBIC);
        srcImage = scaledImg;
    }
    */

    cv::Mat destImage(srcImage.rows, srcImage.cols, srcImage.type());

    std::vector<double> srcPointVec;
    for (const QVariant& data : srcPoints_) {
        const auto point = data.value<QVariantList>();
        const auto x = point[0].value<double>();
        const auto y = point[1].value<double>();
        srcPointVec.push_back(x * srcImage.cols);
        srcPointVec.push_back(y * srcImage.rows);
    }

    double destPointArray[] = {
        0,                  0,
        destImage.cols - 1, 0,
        destImage.cols - 1, destImage.rows - 1,
        0,                  destImage.rows - 1
    };

    const cv::Mat srcPoints(srcPoints_.length(), 2, CV_64FC1, &srcPointVec[0]);
    const cv::Mat destPoints(srcPoints_.length(), 2, CV_64FC1, destPointArray);

    auto homographyMat = cv::findHomography(srcPoints, destPoints);
    cv::warpPerspective(srcImage, destImage, homographyMat, destImage.size());

    {
        std::lock_guard<std::mutex> lock(mutex_);
        image_ = destImage;
    }
    emit imageChanged();
    emit update();
}

}
