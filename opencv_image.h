#ifndef IMAGE_H
#define IMAGE_H

#include <QQuickPaintedItem>
#include <QVariant>
#include <QPainter>
#include <memory>
#include <opencv2/opencv.hpp>

Q_DECLARE_METATYPE(cv::Mat)

namespace MontBlanc
{

class OpenCVImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QVariant image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(double gamma MEMBER gamma_)

public:
    explicit OpenCVImage(QQuickItem *parent = 0);
    QVariant image() const;
    void setImage(const QVariant& image);
    void applyGammeCorrection();
    void paint(QPainter *painter) override;

protected:
    cv::Mat image_;
    double gamma_;

signals:
    void imageChanged() const;
    void error(const QString& message) const;
};

}

#endif // IMAGE_H
