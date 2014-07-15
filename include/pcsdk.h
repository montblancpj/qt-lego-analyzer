#ifndef PCSDK_H 
#define PCSDK_H

#include <memory>
#include <thread>
#include <mutex>
#include <util_pipeline.h>

#pragma warning (disable : 4275)
#pragma warning (disable : 4251)

class __declspec(dllexport) PCSDK: public UtilPipeline
{
public:
    static const int COLOR_WIDTH   = 1280;
    static const int COLOR_HEIGHT  = 720;
	static const int COLOR_CHANNEL = 3;

    static const int DEPTH_WIDTH   = 320;
    static const int DEPTH_HEIGHT  = 240;
	static const int DEPTH_CHANNEL = 1;

	~PCSDK();

	static const std::shared_ptr<PCSDK>& GetInstance();

	void start();
	void stop();
	bool isRunning() const;

    const unsigned char* getColorImage() const;
    const unsigned short* getDepthImage() const;

private:
    PCSDK();
	PCSDK(const PCSDK&);// = delete;
	PCSDK(PCSDK&&);// = delete;
	PCSDK& operator= (const PCSDK&);// = delete;
	PCSDK& operator= (PCSDK&&);// = delete;

    bool OnNewFrame() override;
    void updateColorImage();
	void updateDepthImage();

	static std::shared_ptr<PCSDK> Instance;

    std::unique_ptr<unsigned char[]> colorImage_;
	std::unique_ptr<unsigned short[]> depthImage_;
	std::thread thread_;
	mutable std::mutex mutex_;
	bool isRunning_;
};

#endif