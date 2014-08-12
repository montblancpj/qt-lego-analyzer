#ifndef PCSDKImage_H
#define PCSDKImage_H

#include "opencv_image.h"
#include <pcsdk.h>
#include <memory>

namespace MontBlanc
{

class PCSDKImage : public OpenCVImage
{
    Q_OBJECT
    Q_PROPERTY(double minDistance MEMBER minDistance_)
    Q_PROPERTY(double maxDistance MEMBER maxDistance_)

public:
    explicit PCSDKImage(OpenCVImage *parent = nullptr);
    ~PCSDKImage();

    void setup();
    void updateColorImage();
    void updateDepthImage();

    void paint(QPainter *painter) override;

private:
    std::shared_ptr<PCSDK> pcsdk_;
    double minDistance_, maxDistance_;
};

}

#endif // PCSDKImage_H
