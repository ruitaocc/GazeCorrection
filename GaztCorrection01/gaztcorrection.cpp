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
#include <opencv/cv.h>
#include<opencv/highgui.h>
#include<ctime>
using namespace std;
const int linesNum = 76;
const int TrianglesNum = 41;
const int MaxWidth = 1024;
int lastTriangleIndex[MaxWidth];//记录当前的前w个点的triangle index.
CvMat* all_Invet_Mat[TrianglesNum];
int g_triangeIndex = 0;
string imagepath("C:/Users/VectorCai/Desktop/gazetest/00.png");
#define isForWarp 0
#pragma warning QPoint may have pricise problem.
void FaceWarp(vector<QPoint> &original_land, vector<QPoint> &modified_land,IplImage *image);

bool isInTriangle(QPoint *a, QPoint*b, QPoint *c, QPoint p, float *landas);
QImage *IplImageToQImage( IplImage *img)
{
	QImage *image;
	cvCvtColor(img, img, CV_BGR2RGB);
	uchar *imgData = (uchar *)img->imageData;
	image = new QImage(imgData, img->width, img->height, QImage::Format_RGB888);
	return image;
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
	70,33,
	33,71,
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
int trianglesindex[TrianglesNum][3] = {
	74,0,77,
	74,27,0,
	74,29,27,
	74,78,29,
	78,31,29,
	78,75,31,
	75,33,31,
	75,14,33,
	75,76,14,
	0,2,77,
	0,27,2,
	27,46,2,
	27,29,46,
	29,52,46,
	29,31,52,
	31,33,52,
	33,12,52,
	33,14,12,
	14,76,12,
	2,3,77,
	2,46,3,
	12,11,52,
	3,46,4,
	4,46,5,
	5,46,6,
	6,46,7,
	46,52,7,
	52,8,7,
	52,9,8,
	52,10,9,
	52,11,10,
	3,4,77,
	4,5,77,
	5,6,77,
	6,7,77,
	7,76,77,
	7,8,76,
	8,9,76,
	9,10,76,
	10,11,76,
	11,12,76
};
GaztCorrection::GaztCorrection(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QImage image(imagepath.c_str());
	this->resize(image.width() * 3, image.height() * 3);
	this->paintLands();
	this->ui.m_Image->setPixmap(QPixmap::fromImage(image));
	
}

GaztCorrection::~GaztCorrection()
{

}

void GaztCorrection::paintLands(){
	float scale = 1.0;
	float shit = 3.0;
	//read file
	vector<QPoint> original_land;
	vector<QPoint> modified_land;
	string land_original_file_path("C:/Users/VectorCai/Desktop/gazetest/00.land");
	string land_modified_file_path("C:/Users/VectorCai/Desktop/gazetest/01.land");
	
	QImage image(imagepath.c_str());
	int width = image.width();
	int height = image.height();
	int s_width = width*scale;
	int s_height = height*scale;
	int size = 0;
	fstream file(land_original_file_path.c_str());
	file >> size;
	double first, second;
	for (int i = 0; i < size; i++){
		file >> first >> second;
		original_land.push_back(QPoint(first*s_width, (1.0 - second)*s_height));
	}
	original_land.push_back(QPoint((132.0 / width)*s_width, (34.0 / height) * s_height));
	original_land.push_back(QPoint((252.0 / width) *s_width, (34.0 / height) * s_height));
	original_land.push_back(QPoint((252.0 / width) *s_width, (200.0 / height) *s_height));
	original_land.push_back(QPoint((132.0 / width) * s_width, (200.0 / height) *s_height));
	original_land.push_back(QPoint((192.0 / width) * s_width, (34.0 / height) * s_height));
	
	file.close();
	file.open(land_modified_file_path);
	file >> size;
	first=0, second=0;
	for (int i = 0; i < size; i++){
		file >> first >> second;
		modified_land.push_back(QPoint(first*s_width, (1.0 - second)*s_height));
	}
	modified_land.push_back(QPoint((132.0 / width)*s_width, (34.0 / height) * s_height));
	modified_land.push_back(QPoint((252.0 / width) *s_width, (34.0 / height) * s_height));
	modified_land.push_back(QPoint((252.0 / width) *s_width, (200.0 / height) *s_height));
	modified_land.push_back(QPoint((132.0 / width) * s_width, (200.0 / height) *s_height));
	modified_land.push_back(QPoint((192.0 / width) * s_width, (34.0 / height) * s_height));
	file.close();
	IplImage *inputImage = cvLoadImage(imagepath.c_str());
	FaceWarp(original_land, modified_land, inputImage);
	this->ui.m_output_image->setPixmap(QPixmap::fromImage(*IplImageToQImage(inputImage)));


	/*
	//paint lands
	QPainter painter(this);
	QPen pen;                                 //创建一个画笔  
	pen.setColor(Qt::red);
	pen.setWidth(1);
	painter.setPen(pen);
	for (int i = 0; i<linesNum; i++){
		painter.drawLine(original_land[lineindex[i][1]], original_land[lineindex[i][0]]);
	}
	pen.setWidth(3);
	painter.setPen(pen);
	for (int i = 0; i < original_land.size(); i++){
		painter.drawPoint(original_land[i]);
	}
	
	pen.setColor(Qt::green);
	pen.setWidth(1);
	painter.setPen(pen);
	for (int i = 0; i<linesNum; i++){
		painter.drawLine(modified_land[lineindex[i][1]], modified_land[lineindex[i][0]]);
	}
	pen.setWidth(3);
	painter.setPen(pen);
	for (int i = 0; i < modified_land.size(); i++){
		painter.drawPoint(modified_land[i]);
	}
	*/
}; 
/*
void GaztCorrection::paintEvent(QPaintEvent *){
	//this->paintLands();
};*/
bool isInTriangle(QPoint *a, QPoint*b, QPoint *c, QPoint p,float *landas){
	
	//先用向量叉乘判断是否在三角形内，如果是在继续计算landas
	QPoint ab,ap,bc,bp,ca,cp;
	ab = *b - *a;
	ap = p - *a;
	bc = *c - *b;
	bp = p - *b;
	ca = *a - *c;
	cp = p - *c;
	int y1 = ab.x()*ap.y() - ap.x()*ab.y();
	int y2 = bc.x()*bp.y() - bp.x()*bc.y();
	int y3 = ca.x()*cp.y() - cp.x()*ca.y();
	if (y1 < 0 || y2 < 0 || y3 < 0){
		return 0;
	}
	CvMat* matA = cvCreateMat(3, 3, CV_32FC1);//创建矩阵
	CvMat* matAP = cvCreateMat(3, 3, CV_32FC1);//创建矩阵
	CV_MAT_ELEM(*matA, float, 0, 0) = a->x();
	CV_MAT_ELEM(*matA, float, 0, 1) = b->x();
	CV_MAT_ELEM(*matA, float, 0, 2) = c->x();
	CV_MAT_ELEM(*matA, float, 1, 0) = a->y();
	CV_MAT_ELEM(*matA, float, 1, 1) = b->y();
	CV_MAT_ELEM(*matA, float, 1, 2) = c->y();
	CV_MAT_ELEM(*matA, float, 2, 0) = 1.f;
	CV_MAT_ELEM(*matA, float, 2, 1) = 1.f;
	CV_MAT_ELEM(*matA, float, 2, 2) = 1.f;
	CvMat* matY = cvCreateMat(3, 1, CV_32FC1);//创建矩阵
	CV_MAT_ELEM(*matY, float, 0, 0) = p.x();
	CV_MAT_ELEM(*matY, float, 1, 0) = p.y();
	CV_MAT_ELEM(*matY, float, 2, 0) = 1.f;
	CvMat* matX = cvCreateMat(3, 1, CV_32FC1);//创建矩阵
	cvInvert(matA, matAP, CV_LU);
	cvMatMul(matAP, matY, matX);
	landas[0] = CV_MAT_ELEM(*matX, float, 0, 0);
	landas[1] = CV_MAT_ELEM(*matX, float, 1, 0);
	landas[2] = CV_MAT_ELEM(*matX, float, 2, 0);
	return 1;
	/*
	CvMat* matAP = all_Invet_Mat[g_triangeIndex];//创建矩阵
	CvMat* matY = cvCreateMat(3, 1, CV_32FC1);//创建矩阵
	CV_MAT_ELEM(*matY, float, 0, 0) = p.x();
	CV_MAT_ELEM(*matY, float, 1, 0) = p.y();
	CV_MAT_ELEM(*matY, float, 2, 0) = 1.f;
	CvMat* matX = cvCreateMat(3, 1, CV_32FC1);//创建矩阵
	cvMatMul(matAP, matY, matX);
	landas[0] = CV_MAT_ELEM(*matX, float, 0, 0);
	landas[1] = CV_MAT_ELEM(*matX, float, 1, 0);
	landas[2] = CV_MAT_ELEM(*matX, float, 2, 0);
	return 1;
	*/
};
void FaceWarp(vector<QPoint> &original_land, vector<QPoint> &modified_land, IplImage *result){
	clock_t start, finish;
	start = clock();
	IplImage *tmpimage = cvCloneImage(result);
	cvZero(result);
	//tmpimage = cvCloneImage (result);
	//return;
	int width = tmpimage->width;
	int height = tmpimage->height;
	int x, y;
	bool is_intriangle = false;
	float landas[3] = {0};
	g_triangeIndex = 0;
	CvScalar value;
	int map_x = 0, map_y = 0;
	for (int i = 0; i < MaxWidth; i++)lastTriangleIndex[i] = -1;
	for (int i = 0; i < TrianglesNum; i++){
		//计算 矩阵的逆
		all_Invet_Mat[i] = cvCreateMat(3, 3, CV_32FC1);//创建矩阵
		QPoint a = modified_land[trianglesindex[i][0]];
		QPoint b = modified_land[trianglesindex[i][1]];
		QPoint c = modified_land[trianglesindex[i][2]];
		CvMat* matA = cvCreateMat(3, 3, CV_32FC1);//创建矩阵
		CV_MAT_ELEM(*matA, float, 0, 0) = a.x();
		CV_MAT_ELEM(*matA, float, 0, 1) = b.x();
		CV_MAT_ELEM(*matA, float, 0, 2) = c.x();
		CV_MAT_ELEM(*matA, float, 1, 0) = a.y();
		CV_MAT_ELEM(*matA, float, 1, 1) = b.y();
		CV_MAT_ELEM(*matA, float, 1, 2) = c.y();
		CV_MAT_ELEM(*matA, float, 2, 0) = 1.f;
		CV_MAT_ELEM(*matA, float, 2, 1) = 1.f;
		CV_MAT_ELEM(*matA, float, 2, 2) = 1.f;
		cvInvert(matA, all_Invet_Mat[i], CV_LU);
	}
	int sum = 0, num = 0;
	for (int h = 0; h < height; h++){
		for (int w = 0; w < width; w++){
			if (w < 132 || w >= 252 || h < 34 || h >= 200){
				value = cvGet2D(tmpimage, h, w);
				cvSet2D(result, h, w, value);
				continue;
			}
			sum++;
			is_intriangle = false;
			if (w>0 && lastTriangleIndex[w - 1] != -1){//若左边已经计算了
				is_intriangle = isInTriangle(&modified_land[trianglesindex[lastTriangleIndex[w - 1]][0]], &modified_land[trianglesindex[lastTriangleIndex[w - 1]][1]], \
					&modified_land[trianglesindex[lastTriangleIndex[w - 1]][2]], QPoint(w, h), landas);
				if (is_intriangle){//若跟左边点在同个三角形 更新信息
					g_triangeIndex = lastTriangleIndex[w - 1];
					lastTriangleIndex[w] = g_triangeIndex;
				}
				else if (lastTriangleIndex[w] != -1){//不然看是否跟上面的点在同一三角形
					is_intriangle = isInTriangle(&modified_land[trianglesindex[lastTriangleIndex[w]][0]], &modified_land[trianglesindex[lastTriangleIndex[w ]][1]], \
						&modified_land[trianglesindex[lastTriangleIndex[w ]][2]], QPoint(w, h), landas);
					if (is_intriangle){
						g_triangeIndex = lastTriangleIndex[w];
					}else{//如果不是，就只能搜了
						num++;
						for (int i = 0; i < TrianglesNum; i++){
							is_intriangle = isInTriangle(&modified_land[trianglesindex[i][0]], &modified_land[trianglesindex[i][1]], \
								&modified_land[trianglesindex[i][2]], QPoint(w, h), landas);
							if (is_intriangle){
								lastTriangleIndex[w] = i;
								g_triangeIndex = i;
								break;
							}
						}
					}
				}
			}else{//若左边还没计算，那就搜索。
				num++;
				for (int i = 0; i < TrianglesNum; i++){
					is_intriangle = isInTriangle(&modified_land[trianglesindex[i][0]], &modified_land[trianglesindex[i][1]], \
						&modified_land[trianglesindex[i][2]], QPoint(w, h), landas);
					if (is_intriangle){
						lastTriangleIndex[w] = i;
						g_triangeIndex = i;
						break;
					}
				}
			}
			if (is_intriangle){
				map_x = landas[0] * (original_land[trianglesindex[g_triangeIndex][0]].x());
				map_x += landas[1] * (original_land[trianglesindex[g_triangeIndex][1]].x());
				map_x += landas[2] * (original_land[trianglesindex[g_triangeIndex][2]].x());
				map_y = landas[0] * (original_land[trianglesindex[g_triangeIndex][0]].y());
				map_y += landas[1] * (original_land[trianglesindex[g_triangeIndex][1]].y());
				map_y += landas[2] * (original_land[trianglesindex[g_triangeIndex][2]].y());
				if (map_x >= width)map_x = width - 1;
				if (map_y >= height)map_y = height - 1;
				if (map_x <= 0)map_x = 0;
				if (map_y <= 0)map_y = 0;
				value = cvGet2D(tmpimage, map_y, map_x);
				cvSet2D(result, h, w, value);
			}
		}
	}
	finish = clock();
	double totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nruntime:" << totaltime << " s！" << endl;
	cout << "sum:" << sum << "\tnum:" << num << endl;
};
