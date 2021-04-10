#pragma once
#include "Component.h"
#include "DataType.h"
#include <opencv2/opencv.hpp>
extern "C" {
#include <libavformat/avformat.h>
};

class Router;

class Encoder: public Component<cv::Mat> {
public:
    Encoder(Router*);
protected:
    void start();
    bool wait();
    void process();
    void end();
private:
    Router* router;
    AVCodecContext* avContext;
    AVFrame* avFrame;
    AVPacket* avPacket;
    int frameIndex;
    int init(EncodeConfig);
    AVPacket* encode(const cv::Mat&);
};