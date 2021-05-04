#include "Render.h"
#include "MyGLWidget.h"

Render::Render(MyGLWidget* _glWidget)
{
	glWidget = _glWidget;
}

void Render::start() {
	srand(time(NULL));
	name = std::to_string(rand());
}
bool Render::wait() {
	return getSize() == 0;
}
void Render::process() {
	std::shared_ptr<cv::Mat> frame;
	frame = getData();

	if(glWidget != nullptr)
		glWidget->setTexture(frame);

	//cv::imshow(name, frame);
	//cv::waitKey(10);
}
void Render::end() {
	while (getSize()) {
		getData();
	}
}