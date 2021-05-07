#include "Capture.h"
#include "Render.h"
#include "Encoder.h"
#include "Decoder.h"
#include "Component.hpp"

#include <iostream>


using namespace std;
Capture::Capture(Render* render_, Encoder* encoder_) : Component() {
	render = render_;
	encoder = encoder_;
}
void Capture::start() {
	videoCapture.open(0);
}
bool Capture::wait() {
	return false;
}
void Capture::process() {
	std::shared_ptr<cv::Mat> framePtr(new cv::Mat());
	videoCapture >> (*framePtr);
	if (!framePtr->empty()) {
		render->receive(framePtr);
		encoder->receive(framePtr);
	}
}
void Capture::end() {
	videoCapture.release();
}