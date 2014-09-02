#include  "webcam_widget.h"
#include <QtGui\QPainter> 
#include <QtCore\QPoint> 

WebCam_Widget::WebCam_Widget(CvSize size) {
	this->initdata();
	m_size = size;
	this->initStatus();
	this->setFixedHeight(size.height);
	this->setFixedWidth(size.width);
	this->resize(m_size.width,m_size.height);
};
void WebCam_Widget::initdata(){
	resize_frame = NULL;
	capture = NULL;
	frame = NULL;
	iplImg = NULL;
	qImg = NULL;
	timer = NULL;
};
WebCam_Widget::~WebCam_Widget()
{
	if (iplImg)iplImg->imageData = NULL;
	if (qImg)delete qImg; qImg = NULL;
	if (timer)delete timer; timer = NULL;
	if (frame)frame=NULL;
	if (iplImg)cvReleaseImageHeader(&iplImg); iplImg = NULL;
	if (resize_frame)cvReleaseImage(&resize_frame); resize_frame = NULL;
	if (capture)cvReleaseCapture(&capture); capture = NULL;
}
void WebCam_Widget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	if (status == STATUS_PLAYING)painter.drawImage(QPoint(0, 0), *qImg);
}
void WebCam_Widget::nextFrame()
{
	
	frame = cvQueryFrame(capture);
	if (frame)
	{
		cvResize(frame, resize_frame);
		if (frame->origin == IPL_ORIGIN_TL)
		{
			cvCopy(resize_frame, iplImg, 0);
		}
		else
		{
			cvFlip(resize_frame, iplImg, 0);
		}
		cvCvtColor(iplImg, iplImg, CV_BGR2RGB);
		this->update();
	}
	else{
		this->reset();
	}
}
void WebCam_Widget::play(){
	if (status == STATUS_PLAYING)return;
	if (status == STATUS_STOP){
		timer->start();
		status = STATUS_PLAYING;
		return;
	}
	status = STATUS_PLAYING;
	timer->start();
};
void WebCam_Widget::stop(){
	if (status == STATUS_PLAYING){
		status = STATUS_STOP;
		timer->stop();
	}
};
void WebCam_Widget::reset(){
	timer->stop();
	status = STATUS_STOP;
	emit sendTerminateSignal();
};
void WebCam_Widget::initStatus(){
	capture = cvCaptureFromCAM(0);
	frame = cvQueryFrame(capture);
	if (capture)
	{
		frame = cvQueryFrame(capture);
		if (frame){
			resize_frame = cvCreateImage(m_size, frame->depth, frame->nChannels);
			cvResize(frame, resize_frame);
		}
		qImg = new QImage(QSize(resize_frame->width, resize_frame->height),
			QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(cvSize(resize_frame->width, resize_frame->height),
			8, 3);
		iplImg->imageData = (char*)qImg->bits();
		timer = new QTimer(this);
		double fps = 30;   //读取视频的帧率
		int vfps = 1000 / fps;                                        //计算每帧播放的时间
		timer->setInterval(vfps);
		connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
	}
	status = STATUS_INIT;
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