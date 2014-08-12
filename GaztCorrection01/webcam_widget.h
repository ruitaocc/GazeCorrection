#ifndef WEBCAM_H  
#define WEBCAM_H  
#include <QtWidgets/QMainWindow>

#include <QtGui\QPaintEvent> 
#include <QtGui\QImage> 
#include <QtCore\QTimer> 
#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <string>
using  std::string;
typedef enum{
	W_STATUS_INIT,
	W_STATUS_PLAYING,
	W_STATUS_STOP
}W_PlayStatus;
class WebCam_Widget : public QWidget
{
	Q_OBJECT
public:
	WebCam_Widget(QWidget *parent = 0);
	~WebCam_Widget();
	void play();
	void stop();
	void reset();
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
	W_PlayStatus status;
};
#endif  