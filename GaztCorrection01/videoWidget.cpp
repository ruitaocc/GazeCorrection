#include  "videoWidget.h"
#include <QtGui\QPainter> 
#include <QtCore\QPoint> 
VideoWidget::VideoWidget(const char *filename, QWidget *parent /* = 0 */) : QWidget(parent)
{
	filepath.clear();
	filepath.append(filename);
	this->initStatus();
	this->reset();
}
VideoWidget::VideoWidget(QWidget *parent /* = 0 */) : QWidget(parent){
	firstSet = true;
	status = STATUS_PLAYING;
};
VideoWidget::~VideoWidget()
{
	cvReleaseImage(&iplImg);
	if (capture)cvReleaseCapture(&capture);
	delete qImg;
	delete timer;
}
void VideoWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	if (status == STATUS_PLAYING || status == STATUS_STOP)painter.drawImage(QPoint(0, 0), *qImg);
}
void VideoWidget::nextFrame()
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
void VideoWidget::play(){
	if (status == STATUS_PLAYING)return;
	if (status == STATUS_STOP){
		timer->start();
		status = STATUS_PLAYING;
		return;
	}
	status = STATUS_PLAYING;
	timer->start();
};
void VideoWidget::stop(){
	if (status == STATUS_PLAYING){
		status = STATUS_STOP;
		timer->stop();
	}
};
void VideoWidget::reset(){
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
	status = STATUS_STOP;
	emit sendTerminateSignal();
};
void VideoWidget::initStatus(){
	capture = cvCaptureFromFile(filepath.c_str());
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
	status = STATUS_INIT;
};
void VideoWidget::setFilePath(string file){
	filepath = file;
	this->initStatus();
	this->reset();
};

void VideoWidget::setNextFrame(IplImage *next_frame){
	if (next_frame)
	{
		
		if (firstSet){
			if (next_frame)
				this->resize(next_frame->width, next_frame->height);
			qImg = new QImage(QSize(next_frame->width, next_frame->height),
				QImage::Format_RGB888);
			iplImg = cvCreateImageHeader(cvSize(next_frame->width, next_frame->height),
				8, 3);
			iplImg->imageData = (char*)qImg->bits();
			firstSet = false;
		}
		if (next_frame->origin == IPL_ORIGIN_TL)
		{
			cvCopy(next_frame, iplImg, 0);
		}
		else
		{
			cvFlip(next_frame, iplImg, 0);
		}
		this->update();
	}
};
void VideoWidget::getCurrentFrame(IplImage * dst){
	if (dst){
		cvCopyImage(iplImg, dst);
	}
	else{
		dst = cvCloneImage(iplImg);
	}
};
IplImage * VideoWidget::getCurrentFrameClone(){
	return cvCloneImage(iplImg);
};