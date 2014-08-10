#ifndef MYWIDGET_H  
#define MYWIDGET_H  
#include <QtWidgets/QMainWindow>

#include <QtGui\QPaintEvent> 
#include <QtGui\QImage> 
#include <QtCore\QTimer> 
#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <string>
using  std::string;
typedef enum{
	STATUS_INIT,
	STATUS_PLAYING,
	STATUS_STOP
}PlayStatus;
class VideoWidget : public QWidget
{
	Q_OBJECT
public:
	VideoWidget(const char *filename, QWidget *parent = 0);
	VideoWidget(QWidget *parent = 0);
	~VideoWidget();
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
signals:
	void sendTerminateSignal();
private:
	CvCapture *capture;
	IplImage *iplImg;
	IplImage *frame;
	QImage *qImg;
	QTimer *timer;
	string filepath;
	PlayStatus status;
	bool firstSet;
};
#endif  