#ifndef LEVELANALYZEDIMAGE_H
#define LEVELANALYZEDIMAGE_H

#include "opencv_image.h"
#include <map>
#include <vector>

namespace MontBlanc {

class LevelAnalyzedImage : public OpenCVImage
{
    Q_OBJECT

    Q_PROPERTY(QVariantList targetRects READ targetRects WRITE setTargetRects NOTIFY targetRectsChanged)
    Q_PROPERTY(QVariantList result READ result NOTIFY resultChanged)
    Q_PROPERTY(QVariant inputImage READ inputImage WRITE setInputImage NOTIFY inputImageChanged)
    Q_PROPERTY(double contrast MEMBER contrast_)

public:
    explicit LevelAnalyzedImage(QQuickItem *parent = 0);
    Q_INVOKABLE void analyze();

private:
    std::vector<cv::Rect> targetRects_;
    std::vector<int> result_;
    cv::Mat inputImage_;
    std::mutex inputImageMutex_;

    void setTargetRects(const QVariantList& rects);
    QVariantList targetRects() const;

    QVariantList result() const; // readonly

    void setInputImage(const QVariant& image);
    QVariant inputImage() const;

    double contrast_;

signals:
    void targetRectsChanged() const;
    void resultChanged() const;
    void inputImageChanged() const;
};

}

#endif // LEVELANALYZEDIMAGE_H
