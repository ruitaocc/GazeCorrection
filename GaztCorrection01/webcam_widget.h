#ifndef WEBCAM_H  
#define WEBCAM_H  
#include "VideoWidget_Base.h"
class WebCam_Widget : public VideoWidget_Base
{
	Q_OBJECT
public:
	WebCam_Widget(CvSize size);
	~WebCam_Widget();
	void initdata();
	void play();
	void stop();
	void reset();
	void getCurrentFrame(IplImage * dst);
	IplImage * getCurrentFrameClone();
	void setFilePath(string file){};
	void setNextFrame(IplImage *next_frame) {};

protected:
	void paintEvent(QPaintEvent *e);
private slots:
	void nextFrame();
	void initStatus();

};
#endif  