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
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)

public:
    explicit OpenCVImage(QQuickItem *parent = 0);
    QVariant image() const;
    void setImage(const QVariant& image);
    QString filePath() const;
    void setFilePath(const QString& path);
    void paint(QPainter *painter) override;

protected:
    QString filePath_;
    cv::Mat image_;
    mutable std::mutex mutex_;

signals:
    void imageChanged() const;
    void filePathChanged() const;
    void error(const QString& message) const;
};

}

#endif // IMAGE_H
