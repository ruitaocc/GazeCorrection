#include "gaztcorrection.h"
#include <QString>  
#include <QMessageBox>  
#include <QPainter>  
#include <QPen>  
#include <qfiledialog.h>
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

#define vWidth 640
#define vHeight 480
const int linesNum = 76;
string imagepath("C:/Users/VectorCai/Desktop/gazetest/02.png");
string default_vediopath("C:/Users/VectorCai/Desktop/gazetest/test.avi");
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
	m_video_filepath = default_vediopath;
	m_mode = P_OFFLINE_MODE;
	;
	loop = false;
	this->ui.m_radioBtn_offline->setChecked(1);

	QImage imagein(imagepath.c_str());
	QImage image_target_out(target_out_imagepath.c_str());
	IplImage *t_test = cvLoadImage(imagepath.c_str());
	this->resize(1400, 800);
	this->ui.m_Image->setPixmap(QPixmap::fromImage(imagein));
	this->ui.m_target_out_image->setPixmap(QPixmap::fromImage(image_target_out));
	
	//
	m_gaze_conthroller = new GazeController();
	connect(m_gaze_conthroller, SIGNAL(sendImg(IplImage*)), this, SLOT(setNextFrameToOutput(IplImage*)));
	connect(this, SIGNAL(sendData(vector<CvPoint>, vector<CvPoint>)), m_gaze_conthroller, SLOT(setData(vector<CvPoint>, vector<CvPoint>)));
	//
	this->paintLands();
	//add player
	QGridLayout * in_group_Layout = new QGridLayout();
	//add local video
	m_video_local_input_widget = new VideoWidget(m_video_filepath.c_str(), cvSize(vWidth, vHeight));
	in_group_Layout->addWidget(m_video_local_input_widget, 0, 0, Qt::AlignLeft | Qt::AlignTop);
	m_video_local_input_widget->show();
	connect(m_video_local_input_widget, SIGNAL(sendTerminateSignal()), this, SLOT(receiveTerminateSignal()));
	//add real time
	m_video_webcam_input_widget = new WebCam_Widget(cvSize(vWidth, vHeight));
	
	in_group_Layout->addWidget(m_video_webcam_input_widget, 0, 0, Qt::AlignLeft | Qt::AlignTop);
	m_video_webcam_input_widget->hide();
	this->ui.m_groupBox->setLayout(in_group_Layout);
	
	//add output frame
	QGridLayout * out_group_Layout = new QGridLayout();
	m_video_output_widget = new VideoWidget(cvSize(vWidth, vHeight));
	m_video_output_widget->setNextFrame(m_video_local_input_widget->getCurrentFrameClone());
	out_group_Layout->addWidget(m_video_output_widget, 0, 0, Qt::AlignLeft | Qt::AlignTop);
	this->ui.m_out_groupBox->setLayout(out_group_Layout);
	m_video_output_widget->show();
	

}

GaztCorrection::~GaztCorrection()
{
	delete m_video_local_input_widget;
	delete m_video_output_widget;
	delete m_video_webcam_input_widget;
}

void GaztCorrection::setNextFrameToOutput(IplImage *img){
	m_video_output_widget->setNextFrame(img);
	if (loop)m_gaze_conthroller->start();
};
void GaztCorrection::playVideo(){
	if (m_mode == P_OFFLINE_MODE){
		m_gaze_conthroller->setInputAndOutput(m_video_local_input_widget, m_video_output_widget);
		m_video_local_input_widget->play();
		loop = true;
		m_gaze_conthroller->start();
	}
	if (m_mode == P_REALTIME_MODE){
		m_gaze_conthroller->setInputAndOutput(m_video_webcam_input_widget, m_video_output_widget);
		m_video_webcam_input_widget->play();
		loop = true;
		m_gaze_conthroller->start();
	}
};
void GaztCorrection::stopVideo(){
	if (m_mode == P_OFFLINE_MODE){
		m_video_local_input_widget->stop();
		loop = false;
	}
	if (m_mode == P_REALTIME_MODE){
		m_video_webcam_input_widget->stop();
		loop = false;
	}
};
void GaztCorrection::terminateVideo(){
	m_video_local_input_widget->reset();
	loop = false;
};
void GaztCorrection::receiveTerminateSignal(){
	loop = false;
};
vector<CvPoint> original_land;
vector<CvPoint> modified_land;

void GaztCorrection::paintLands(){
	//read file
	string land_original_file_path("C:/Users/VectorCai/Desktop/gazetest/02.land");
	string land_modified_file_path("C:/Users/VectorCai/Desktop/gazetest/03.land");
	
	QImage image(imagepath.c_str());
	int s_width = 640;
	int s_height = 480;
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
	//m_gaze_conthroller->setData(original_land, modified_land);
	emit sendData(original_land, modified_land);
	//IplImage *inputImage = cvLoadImage(imagepath.c_str());
	//GazeCorrector *gazecorrector = new GazeCorrector();
	//gazecorrector->FaceWarp(original_land, modified_land, inputImage);
	//this->ui.m_transfer_iamge->setPixmap(QPixmap::fromImage(*IplImageToQImage(gazecorrector->transfer_image)));
	//this->ui.m_output_image->setPixmap(QPixmap::fromImage(*IplImageToQImage(gazecorrector->mid01_image)));
	//this->ui.m_seamless_image->setPixmap(QPixmap::fromImage(*IplImageToQImage(gazecorrector->sealess_image)));
}; 

void GaztCorrection::file(){
	this->stopVideo();
	QString qstr_path = QFileDialog::getOpenFileName(this, "Select file", "", "allfiles(*.*)");
	if (!qstr_path.isEmpty()){
		m_video_filepath = qstr_path.toStdString();
		this->m_video_local_input_widget->setFilePath(m_video_filepath);
	}
};
void GaztCorrection::processMode(){
	this->stopVideo();// stop the current input
	if (this->ui.m_radioBtn_Realtime->isChecked()){
		m_mode = P_REALTIME_MODE;
		m_video_local_input_widget->hide();
		m_video_webcam_input_widget->show();
		this->hideControlBtns();
		this->showRealTime(true);
	}
	else {
		m_mode = P_OFFLINE_MODE;
		m_video_local_input_widget->show();
		m_video_webcam_input_widget->hide();
		this->showControlBtns();
		this->showRealTime(false);
	}
};
void GaztCorrection::showRealTime(bool isRealTime){

}
void GaztCorrection::hideControlBtns(){
	this->ui.m_file_btn->hide();
	this->ui.m_reset_btn->hide();
	//this->ui.m_stop_btn->hide();
	//this->ui.m_play_btn->hide();
};
void GaztCorrection::showControlBtns(){
	this->ui.m_file_btn->show();
	this->ui.m_reset_btn->show();
	//this->ui.m_stop_btn->show();
	//this->ui.m_play_btn->show();
};
void GaztCorrection::dde_init(){
	this->m_gaze_conthroller->dde_init();
};
void GaztCorrection::dde_first_track(){
	this->m_gaze_conthroller->dde_first_track();
};
void GaztCorrection::dde_track(){
	this->m_gaze_conthroller->dde_init();
};
void GaztCorrection::calibration(){
	this->m_gaze_conthroller->calibration();
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