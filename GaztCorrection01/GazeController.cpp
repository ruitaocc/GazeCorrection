#include "GazeController.h"
#include <Windows.h>
GazeController::GazeController()
{
	m_img = NULL;
	gazecorrector = new GazeCorrector();
}
GazeController::~GazeController()
{
}

void GazeController::run(){
	this->routine();
	emit sendImg(m_img);
};

void GazeController::setInputAndOutput(VideoWidget * input_ref, VideoWidget * output_ref){
	input = input_ref;
	output = output_ref;
};

void GazeController::routine(){
	if (m_img){
		IplImage *img = input->getCurrentFrameClone();
		cvCopyImage(img, m_img);
		cvReleaseImage(&img);
	}
	else{
		m_img = input->getCurrentFrameClone();
	}
		vector<CvPoint> t_original_land = original_land;
		vector<CvPoint> t_modified_land = modified_land;
		gazecorrector->FaceWarp(t_original_land, t_modified_land, m_img);
};

void GazeController::setData(vector<CvPoint> a, vector<CvPoint> b){
	this->modified_land = b;
	this->original_land = a;
}