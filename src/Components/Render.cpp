#include "Render.h"

Render::Render(MyGLWidget* glWidget)
{
}

void Render::start() {
	srand(time(NULL));
	name = to_string(rand());
}
bool Render::wait() {
	return getSize() == 0;
}
void Render::process() {
	cv::Mat frame;
	frame = getData();

	glWidget->setTexture(frame);

	//cv::imshow(name, frame);
	//cv::waitKey(10);
}
void Render::end() {
	while (getSize()) {
		getData();
	}
}