#include "GazeController.h"
#include <Windows.h>
#include <vector>
#include <fstream>
#define PAINT_LAND 1
using namespace std;
vector<vector<int>> landlines_index;
string landline_file_path("Data/LandmarkLine_74.txt");
GazeController::GazeController()
{
	m_img = NULL;
	m_calibration_status = C_Face_to_Screen;
	//init lands
	CvPoint * p = new CvPoint[79];
	m_face_to_screen_land.assign(&p[0], &p[78]);
	m_face_to_camera_land.assign(&p[0], &p[78]);
	m_diff_of_calibration.assign(&p[0], &p[78]);
	original_land.assign(&p[0], &p[78]);
	modified_land.assign(&p[0], &p[78]);
	m_init_scale_x=m_init_scale_y = 0;
	delete[] p;
	//
	gazecorrector = new GazeCorrector();
	m_ddeWrapper = new cDDEWrapper();
	fstream f(landline_file_path);
	int sep = 0,ps=0;
	f >> sep;
	int index = 0;
	for (int i = 0; i < sep; i++){
		f >> ps;
		vector<int> lsep;
		for (int j = 0; j < ps; j++){
			f >> index;
			lsep.push_back(index);
		}
		landlines_index.push_back(lsep);
	}
}
GazeController::~GazeController()
{
	cvReleaseImage(&m_img);
}
static bool initiated = false;
void GazeController::run(){
	if (!initiated){
		this->dde_init();
		initiated = true;
	}
	this->routine();
	emit sendImg(m_img);
};

void GazeController::setInputAndOutput(VideoWidget_Base * input_ref, VideoWidget_Base * output_ref){
	input = input_ref;
	output = output_ref;
};
static bool hasfirsttrack = false;
static int paintpoint_index = 74;
void GazeController::routine(){
	if (m_img){
		IplImage *img = input->getCurrentFrameClone();
		cvCopyImage(img, m_img);
		cvReleaseImage(&img);
	}
	else{
		m_img = input->getCurrentFrameClone();
	}
	// DDE 
	if (hasfirsttrack){
		// Call the API
		cvFlip(m_img);
		if (!m_ddeWrapper->trackVideoFrame((uchar*)m_img->imageData)){
			printf("Track track error!");
		}
		// Get the tracking results
		m_ddeWrapper->getParam(m_frmParam);
		int landdim = m_frmParam.mLandDim;
		float* pland = m_frmParam.mLandList;
		int meshdim = m_frmParam.mMeshPixPosNum;
		float* pmesh = m_frmParam.mMeshPixPosList;
		int min = MAXINT, min_index = 0,cur_sum=0;
		/*
		for (int k = 0; k < meshdim; k++){
			cur_sum = pow((pmesh[k * 3] - pland[27 * 2]), 2) + pow((pmesh[k * 3 + 1] - pland[27 * 2 + 1]), 2);
			if (cur_sum < min){
				min = cur_sum;
				min_index = k;
			}
		}
		*/
		//paint land
#if PAINT_LAND		
		for (int i = 0; i < landlines_index.size(); i++){
			for (int j = 0; j < landlines_index[i].size() - 1; j++){
				int index1 = landlines_index[i][j];
				int index2 = landlines_index[i][j + 1];
				cvLine(m_img, cvPoint(pland[index1 * 2], pland[index1 * 2 + 1]), \
					cvPoint(pland[index2 * 2], pland[index2 * 2 + 1]), cvScalar(255, 0, 0));
			}
		}
		for (int i = 0; i < landdim; i++){
			cvLine(m_img, cvPoint(pland[i*2], pland[i*2+1]), \
				cvPoint(pland[i*2], pland[i*2 + 1]), cvScalar(255, 0, 0),2);
		}
#endif
		cvFlip(m_img);
		//
		if (m_calibration_status == C_Calibrated){
			for (int i = 0; i < landdim; i++){
				original_land[i] = cvPoint(pland[i * 2], 480 - pland[i * 2 + 1]);
			}
			float cur_scale_x = (original_land[33].x - original_land[27].x) + \
				(original_land[31].x - original_land[29].x);
			cur_scale_x /= 2;
			cur_scale_x = cur_scale_x / m_init_scale_x;
			float diff_x, diff_y;
			for (int i = 0; i < landdim; i++){
				diff_x = original_land[i].x - cur_scale_x* m_diff_of_calibration[i].x;
				diff_y = original_land[i].y - cur_scale_x* m_diff_of_calibration[i].y;
				modified_land[i].x = diff_x;
				modified_land[i].y = diff_y;
			}
			gazecorrector->FaceWarp(original_land, modified_land, m_img);
		}
	}
	
	
};

void GazeController::setData(vector<CvPoint> a, vector<CvPoint> b){
	//this->modified_land = b;
	//this->original_land = a;
}
void GazeController::dde_init(){
	string faceWarehouseName("Data\\IdenTensor_75_47.bin");
	string regName("Data\\FaceWarehouse_Profile_LWF_12144_Near_Rand.rgs");
	string maskRegName("Data\\Rgs\\MaskReg.msrgs");
	m_ddeWrapper->initialize(regName.c_str(), maskRegName.c_str(), faceWarehouseName.c_str());
	m_ddeWrapper->setUpdateIdenFov(1);
};
void GazeController::dde_first_track(){
	if (m_img){
		IplImage *img = input->getCurrentFrameClone();
		cvCopyImage(img, m_img);
		cvReleaseImage(&img);
	}
	else{
		m_img = input->getCurrentFrameClone();
	}
	IplImage * m_imgNofilp = cvCloneImage(m_img);
	cvFlip(m_img);
	// Get the image data from camera, video etc.
	if (!m_ddeWrapper->trackFirstFrame((uchar*)m_img->imageData)){
		printf("Track first frame error!\n");
		exit(0);
	}
	// Get the tracking results
	hasfirsttrack = true;
	m_ddeWrapper->getParam(m_frmParam);
	
};
void GazeController::dde_track(){
	
	
};
void  GazeController::calibration(){
	m_ddeWrapper->getParam(m_frmParam);
	int landdim = m_frmParam.mLandDim;
	float* pland = m_frmParam.mLandList;
	if (m_calibration_status == C_Face_to_Screen){
		for (int i = 0; i < landdim; i++){
			m_face_to_screen_land[i].x = pland[i * 2];
			m_face_to_screen_land[i].y = 480-pland[i * 2 + 1];
		}
		m_calibration_status = C_Face_to_Camera;
		return;
	}
	if (m_calibration_status == C_Face_to_Camera){
		for (int i = 0; i < landdim; i++){
			m_face_to_camera_land[i].x = pland[i * 2];
			m_face_to_camera_land[i].y = 480 - pland[i * 2 + 1];
		}
		//calculate diff
		float diff_x, diff_y;
		for (int i = 0; i < landdim; i++){
			diff_x = m_face_to_screen_land[i].x - m_face_to_camera_land[i].x;
			diff_y = m_face_to_screen_land[i].y - m_face_to_camera_land[i].y;
			m_diff_of_calibration[i].x = diff_x;
			m_diff_of_calibration[i].y = diff_y;
		}
		//calculate scale
		m_init_scale_x = (m_face_to_screen_land[33].x - m_face_to_screen_land[27].x) + \
			(m_face_to_screen_land[31].x - m_face_to_screen_land[29].x);
		m_init_scale_x /= 2;
		m_calibration_status = C_Calibrated;
		return;
	}
};