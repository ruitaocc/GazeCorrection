#include "gaztcorrection.h"
#include <QString>  
#include <QMessageBox>  
#include <QPainter>  
#include <QPen>  
#include <QMouseEvent>
#include <qimage.h>
#include <string>
#include <fstream>
#include <QPixmap>
#include <QGridLayout>
#include <opencv/cv.h>
#include<opencv/highgui.h>
#include<ctime>
#include "videoWidget.h"
using namespace std;

const int linesNum = 76;
string imagepath("C:/Users/VectorCai/Desktop/gazetest/02.png");
string vediopath("C:/Users/VectorCai/Desktop/gazetest/test.avi");
string target_out_imagepath("C:/Users/VectorCai/Desktop/gazetest/03.png");

//in different platform need to change this function to specific platform.
QImage *IplImageToQImage( IplImage *img)
{
	QImage *image;
	cvCvtColor(img, img, CV_BGR2RGB);
	uchar *imgData = (uchar *)img->imageData;
	image = new QImage(imgData, img->width, img->height, QImage::Format_RGB888);
	return image;
	
}
QPoint cvPointToQpoint(CvPoint p){
	return QPoint(p.x, p.y);
}
int lineindex[linesNum][2] = {
	//chin 14
	0, 1,
	1, 2,
	2, 3,
	3, 4,
	4, 5,
	5, 6,
	6, 7,
	7, 8,
	8, 9,
	9, 10,
	10, 11,
	11, 12,
	12, 13,
	13, 14,
	//eye bow 6
	15, 16,
	16, 17,
	17, 18,
	18, 19,
	19, 20,
	20, 15,
	//eye bow 6
	21, 22,
	22, 23,
	23, 24,
	24, 25,
	25, 26,
	26, 21,
	//eye left 8
	27,66,
	66,28,
	28,69,
	69,29,
	29,68,
	68,30,
	30,67,
	67,27,
	//eye right 8
	31,73,
	73,32,
	32,70,
	70,33,//
	33,71,//
	71,34,
	34,72,
	72,31,
	//nose 12
	35,36,
	36,37,
	37,38,
	38,44,
	44,39,
	39,45,
	45,40,
	40,41,
	41,42,
	42,43,
	37,65,
	65,41,
	//mouth
	46,47,//上
	47,48,
	48,49,
	49,50,
	50,51,
	51,52,

	52,53,//下
	53,54,
	54,55,
	55,56,
	56,57,
	57,46,
	46,58,//下 内
	58,59,
	59,60,
	60,52,
	46,63,//上内
	63,62,
	62,61,
	61,52,

	62,64,
	64,59
};

GaztCorrection::GaztCorrection(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QImage imagein(imagepath.c_str());
	QImage image_target_out(target_out_imagepath.c_str());
	this->resize(1200, 800);
	this->paintLands();
	this->ui.m_Image->setPixmap(QPixmap::fromImage(imagein));
	this->ui.m_target_out_image->setPixmap(QPixmap::fromImage(image_target_out));
	//add player
	QGridLayout * mainLayout = new QGridLayout();
	mw = new VideoWidget(vediopath.c_str());
	mw->setFixedWidth(320);
	mw->setFixedHeight(240);
	mainLayout->addWidget(mw, 0, 0, Qt::AlignLeft | Qt::AlignTop);
	this->ui.m_groupBox->setLayout(mainLayout);
	mw->show();
	
}

GaztCorrection::~GaztCorrection()
{

}
void GaztCorrection::playVideo(){
	mw->play();
};
void GaztCorrection::stopVideo(){
	mw->stop();
};
void GaztCorrection::terminateVideo(){
	mw->reset();
};
vector<CvPoint> original_land;
vector<CvPoint> modified_land;

void GaztCorrection::paintLands(){
	//read file
	string land_original_file_path("C:/Users/VectorCai/Desktop/gazetest/02.land");
	string land_modified_file_path("C:/Users/VectorCai/Desktop/gazetest/03.land");
	
	QImage image(imagepath.c_str());
	int s_width = image.width();
	int s_height = image.height();
	int size = 0;
	fstream file(land_original_file_path.c_str());
	file >> size;
	double first, second;
	for (int i = 0; i < size; i++){
		file >> first >> second;
		original_land.push_back(cvPoint(first*s_width + 0.5, (1.0 - second)*s_height + 0.5));
	}
		
	file.close();
	file.open(land_modified_file_path);
	file >> size;
	first=0, second=0;
	for (int i = 0; i < size; i++){
		file >> first >> second;
		modified_land.push_back(cvPoint(first*s_width + 0.5, (1.0 - second)*s_height + 0.5));
	}
	file.close();
	IplImage *inputImage = cvLoadImage(imagepath.c_str());
	coreector.FaceWarp(original_land, modified_land, inputImage);
	this->ui.m_transfer_iamge->setPixmap(QPixmap::fromImage(*IplImageToQImage(coreector.transfer_image)));
	this->ui.m_output_image->setPixmap(QPixmap::fromImage(*IplImageToQImage(coreector.mid01_image)));
	this->ui.m_seamless_image->setPixmap(QPixmap::fromImage(*IplImageToQImage(coreector.sealess_image)));

	
	
	
	
}; 
/*
void GaztCorrection::pait(){
	//paint lands
	QPainter painter(this->ui.m_output_image);
	QPen pen;                                 //创建一个画笔  
	pen.setColor(Qt::red);
	pen.setWidth(1);
	painter.setPen(pen);
	for (int i = 0; i<linesNum; i++){
		painter.drawLine(cvPointToQpoint(modified_land[lineindex[i][1]]), cvPointToQpoint(modified_land[lineindex[i][0]]));
	}
	pen.setWidth(3);
	painter.setPen(pen);
	for (int i = 0; i < modified_land.size(); i++){
		painter.drawPoint(cvPointToQpoint(modified_land[i]));
	}
};
void GaztCorrection::paintEvent(QPaintEvent *){
	this->pait();
	};*/