#ifndef GAZTCORRECTION_H
#define GAZTCORRECTION_H

#include <QtWidgets/QMainWindow>
#include "ui_gaztcorrection.h"
#include <vector>
#include <QPoint>
#include "videoWidget.h"
#include "webcam_widget.h"
#include "GazeController.h"
#include <string>
using namespace std;
typedef struct myLine{
	QPoint startPnt;
	QPoint endPnt;
}myLine;
typedef enum{
	P_REALTIME_MODE,
	P_OFFLINE_MODE
}ProcessMode;
class GaztCorrection : public QMainWindow
{
	Q_OBJECT

public:
	GaztCorrection(QWidget *parent = 0);
	~GaztCorrection();
public slots:
	void playVideo();
	void stopVideo();
	void terminateVideo();
	void file();
	void processMode();
	void setNextFrameToOutput(IplImage *img);
	void receiveTerminateSignal();
private:
	Ui::GaztCorrectionClass ui;
private:
	void paintLands();
	void hideControlBtns();
	void showControlBtns();
signals:
	void sendData(vector<CvPoint> a, vector<CvPoint> b);
private:
	VideoWidget *m_video_local_input_widget;
	VideoWidget *m_video_output_widget;
	WebCam_Widget * m_video_webcam_input_widget;
	string m_video_filepath;
	ProcessMode m_mode;

	GazeController *m_gaze_conthroller;
	bool loop;
};

#endif // GAZTCORRECTION_H
