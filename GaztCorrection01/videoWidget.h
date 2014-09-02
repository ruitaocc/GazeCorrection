#ifndef MYWIDGET_H  
#define MYWIDGET_H  
#include "VideoWidget_Base.h"
//#include "vld.h"
class VideoWidget : public VideoWidget_Base
{
	Q_OBJECT
public:
	VideoWidget(const char *filename);
	VideoWidget(const char *filename,CvSize size);
	VideoWidget(QWidget *parent = 0);
	VideoWidget(CvSize size);
	~VideoWidget();
	void initdata();
	void play();
	void stop();
	void reset();
	void setFilePath(string file);
	void setNextFrame(IplImage *next_frame);
	void getCurrentFrame(IplImage * dst);
	IplImage * getCurrentFrameClone();
protected:
	void paintEvent(QPaintEvent *e);
private slots:
	void nextFrame();
	void initStatus();
private:
	bool firstSet;
};
#endif  