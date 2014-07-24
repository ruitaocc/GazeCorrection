#ifndef GAZTCORRECTION_H
#define GAZTCORRECTION_H

#include <QtWidgets/QMainWindow>
#include "ui_gaztcorrection.h"
#include <vector>
#include <QPoint>
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
	//void paintEvent(QPaintEvent *);
private:
	Ui::GaztCorrectionClass ui;
	void paintLands();
	vector<myLine> orignal_lines;   
	vector<myLine> modified_lines;
};

#endif // GAZTCORRECTION_H
