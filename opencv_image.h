#ifndef IMAGE_H
#define IMAGE_H

#include <QQuickPaintedItem>
#include <QVariant>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>

Q_DECLARE_METATYPE(cv::Mat)

namespace MontBlanc
{

class OpenCVImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QVariant image READ image WRITE setImage NOTIFY imageChanged)

public:
    explicit OpenCVImage(QQuickItem *parent = 0);
    QVariant image() const;
    void setImage(const QVariant& image);
    void paint(QPainter *painter) override;

protected:
    cv::Mat image_;
    mutable std::mutex mutex_;

signals:
    void imageChanged() const;
    void error(const QString& message) const;
};

}

#endif // IMAGE_H
