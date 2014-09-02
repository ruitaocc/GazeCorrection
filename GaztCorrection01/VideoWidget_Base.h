#ifndef VideoWidget_Base_H  
#define VideoWidget_Base_H
#include <QtWidgets/QMainWindow>
#include <QtGui\QPaintEvent> 
#include <QtGui\QImage> 
#include <QtCore\QTimer> 
#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <string>
using namespace std;
typedef enum{
	STATUS_INIT,
	STATUS_PLAYING,
	STATUS_STOP
}PlayStatus;
class VideoWidget_Base : public QWidget
{
	Q_OBJECT
public:
	VideoWidget_Base(const char *filename, QWidget *parent = 0){};
	VideoWidget_Base(QWidget *parent = 0){};
	virtual  ~VideoWidget_Base(){};
	virtual void play() = 0;
	virtual void stop() = 0;
	virtual void reset() = 0;
	virtual void setFilePath(string file) = 0;
	virtual void setNextFrame(IplImage *next_frame) = 0;
	virtual void getCurrentFrame(IplImage * dst) = 0;
	virtual IplImage * getCurrentFrameClone() = 0;
protected:
	virtual void paintEvent(QPaintEvent *e) = 0;
	virtual void initStatus() = 0;
private slots:
virtual  void nextFrame() = 0;

signals:
	void sendTerminateSignal();
protected:
	CvCapture *capture;
	IplImage *iplImg;
	IplImage *frame;
	IplImage *resize_frame;
	QImage *qImg;
	QTimer *timer;
	string filepath;
	PlayStatus status;
	CvSize m_size;
};
#endif  