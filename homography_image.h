#ifndef HOMOGRAPHY_IMAGE_H
#define HOMOGRAPHY_IMAGE_H

#include <QVariantList>
#include "opencv_image.h"

namespace MontBlanc
{

class HomographyImage : public OpenCVImage
{
    Q_OBJECT
    Q_PROPERTY(QVariant image READ image WRITE setImage)
    Q_PROPERTY(QVariantList srcPoints MEMBER srcPoints_)

public:
    explicit HomographyImage(OpenCVImage *parent = nullptr);
    void setImage(const QVariant& image);

private:
    QVariantList srcPoints_;
};

}

#endif // HOMOGRAPHY_IMAGE_H
