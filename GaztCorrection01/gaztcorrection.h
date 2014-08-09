#ifndef GAZTCORRECTION_H
#define GAZTCORRECTION_H

#include <QtWidgets/QMainWindow>
#include "ui_gaztcorrection.h"
#include <vector>
#include <QPoint>
#include "videoWidget.h"
#include "GazeCorrector.h"
using namespace std;
typedef struct myLine{
	QPoint startPnt;
	QPoint endPnt;
}myLine;
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
private:
	GazeCorrector coreector;

	Ui::GaztCorrectionClass ui;
	void paintLands();
	VideoWidget *mw;
};

#endif // GAZTCORRECTION_H
