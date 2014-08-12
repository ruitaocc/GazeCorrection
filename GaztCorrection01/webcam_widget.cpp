#include  "webcam_widget.h"
#include <QtGui\QPainter> 
#include <QtCore\QPoint> 

WebCam_Widget::WebCam_Widget(QWidget *parent /* = 0 */) : QWidget(parent){
	this->initStatus();
	status = W_STATUS_PLAYING;
};
WebCam_Widget::~WebCam_Widget()
{
	cvReleaseImage(&iplImg);
	if (capture)cvReleaseCapture(&capture);
	delete qImg;
	delete timer;
}
void WebCam_Widget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	if (status == W_STATUS_PLAYING || status == W_STATUS_STOP)painter.drawImage(QPoint(0, 0), *qImg);
}
void WebCam_Widget::nextFrame()
{
	frame = cvQueryFrame(capture);
	if (frame)
	{
		if (frame->origin == IPL_ORIGIN_TL)
		{
			cvCopy(frame, iplImg, 0);
		}
		else
		{
			cvFlip(frame, iplImg, 0);
		}
		cvCvtColor(iplImg, iplImg, CV_BGR2RGB);
		this->update();
	}
	else{
		this->reset();
	}
}
void WebCam_Widget::play(){
	if (status == W_STATUS_PLAYING)return;
	if (status == W_STATUS_STOP){
		timer->start();
		status = W_STATUS_PLAYING;
		return;
	}
	status = W_STATUS_PLAYING;
	timer->start();
};
void WebCam_Widget::stop(){
	if (status == W_STATUS_PLAYING){
		status = W_STATUS_STOP;
		timer->stop();
	}
};
void WebCam_Widget::reset(){
	timer->stop();
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, 0);
	frame = cvQueryFrame(capture);
	if (frame)
	{
		if (frame->origin == IPL_ORIGIN_TL)
		{
			cvCopy(frame, iplImg, 0);
		}
		else
		{
			cvFlip(frame, iplImg, 0);
		}
		cvCvtColor(iplImg, iplImg, CV_BGR2RGB);
		this->update();
	}
	status = W_STATUS_STOP;
	emit sendTerminateSignal();
};
void WebCam_Widget::initStatus(){
	capture = cvCaptureFromCAM(0);
	frame = cvQueryFrame(capture);
	if (capture)
	{
		frame = cvQueryFrame(capture);
		if (frame)
			this->resize(frame->width, frame->height);
		qImg = new QImage(QSize(frame->width, frame->height),
			QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(cvSize(frame->width, frame->height),
			8, 3);
		iplImg->imageData = (char*)qImg->bits();
		timer = new QTimer(this);
		double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);   //读取视频的帧率
		int vfps = 1000 / fps;                                        //计算每帧播放的时间
		timer->setInterval(vfps);
		connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
	}
	status = W_STATUS_INIT;
};



void WebCam_Widget::getCurrentFrame(IplImage * dst){
	if (dst){
		cvCopyImage(iplImg, dst);
	}
	else{
		dst = cvCloneImage(iplImg);
	}
};
IplImage * WebCam_Widget::getCurrentFrameClone(){
	return cvCloneImage(iplImg);
};