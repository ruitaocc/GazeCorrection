#include  "videoWidget.h"
#include <QtGui\QPainter> 
#include <QtCore\QPoint> 
void VideoWidget::initdata(){
	resize_frame = NULL;
	capture = NULL;
	frame = NULL;
	iplImg = NULL;
	qImg = NULL;
	timer = NULL;
};
VideoWidget::VideoWidget(const char *filename)
{
	this->initdata();
	filepath.clear();
	filepath.append(filename);
	this->initStatus();
	this->reset();
	
}
VideoWidget::VideoWidget(const char *filename, CvSize size){
	this->initdata();
	m_size = size;
	this->setFixedHeight(size.height);
	this->setFixedWidth(size.width);
	this->resize(m_size.width, m_size.height);
	filepath.clear();
	filepath.append(filename);
	this->initStatus();
	this->reset();
	
};

VideoWidget::VideoWidget(QWidget *parent /* = 0 */){
	firstSet = true;
	status = STATUS_PLAYING;
	this->initdata();
	
};
VideoWidget::VideoWidget(CvSize size){
	this->initdata();
	m_size = size;
	this->setFixedHeight(size.height);
	this->setFixedWidth(size.width);
	this->resize(m_size.width, m_size.height);
	firstSet = true;
	status = STATUS_PLAYING;

};
VideoWidget::~VideoWidget()
{
	if(qImg)delete qImg; qImg = NULL;
	if(timer)delete timer; timer = NULL;
	if (frame) frame = NULL;
	if (iplImg)cvReleaseImageHeader(&iplImg); iplImg = NULL;
	if (resize_frame)cvReleaseImage(&resize_frame); resize_frame = NULL;
	if (capture)cvReleaseCapture(&capture); capture = NULL;
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
	status = STATUS_STOP;
	emit sendTerminateSignal();
};
void VideoWidget::initStatus(){
	capture = cvCaptureFromFile(filepath.c_str());
	if (capture)
	{
		frame = cvQueryFrame(capture);
		if (frame){
			resize_frame = cvCreateImage(m_size, frame->depth, frame->nChannels);
			cvResize(frame, resize_frame);
		}
		qImg = new QImage(QSize(m_size.width, m_size.height),
			QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(cvSize(m_size.width, m_size.height),
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