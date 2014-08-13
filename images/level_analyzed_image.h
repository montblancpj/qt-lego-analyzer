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
    Q_PROPERTY(QVariantList heights READ heights NOTIFY heightsChanged)
    Q_PROPERTY(QVariant inputImage READ inputImage WRITE setInputImage NOTIFY inputImageChanged)
    Q_PROPERTY(double contrast MEMBER contrast_)
    Q_PROPERTY(double threshold MEMBER threshold_)
    Q_PROPERTY(int checkFrame MEMBER checkFrame_)

public:
    explicit LevelAnalyzedImage(QQuickItem *parent = 0);
    Q_INVOKABLE void analyze();

private:
    cv::Mat inputImage_;
    std::mutex inputImageMutex_;
    std::vector<cv::Rect> targetRects_;
    std::vector<bool> ignoreRectMap_;
    std::vector<int> result_;
    std::vector<int> lastHeights_;
    std::deque<std::vector<int>> heightsCache_;

    double threshold_;
    double contrast_;
    int checkFrame_;

    void setTargetRects(const QVariantList& rects);
    QVariantList targetRects() const;

    QVariantList result() const; // readonly
    QVariantList heights() const; // readonly

    void setInputImage(const QVariant& image);
    QVariant inputImage() const;

    void addHeightsCache(const std::vector<int>& result);
    void checkResult();

signals:
    void targetRectsChanged() const;
    void resultChanged() const;
    void heightsChanged() const;
    void inputImageChanged() const;
};

}

#endif // LEVELANALYZEDIMAGE_H
