#include "GazeCorrector.h"
#include <cmath>
#include <cstdlib>
#include <Eigen/Sparse>
#include <algorithm>
#include<windows.h>
const int TrianglesNum = 93;
const int MaxWidth = 640;
int lastTriangleIndex[MaxWidth];//记录当前的前w个点的triangle index.
CvMat* all_Invet_Mat[TrianglesNum];
CvMat* matA;
CvMat* matY; //创建矩阵
CvMat* matX; //创建矩阵


int trianglesindex[TrianglesNum][3] = {
	73, 0, 76,
	73, 21, 0,
	73, 22, 21,
	73, 77, 22,
	77, 23, 22,
	77, 24, 23,
	77, 18, 24,
	77, 17, 18,
	77, 16, 17,
	77, 74, 16,//
	74, 15, 16,
	74, 14, 15,
	74, 75, 14,

	21, 22, 26,
	22, 23, 26,
	26, 23, 25,
	23, 24, 25,

	18, 17, 19,
	17, 16, 19,
	19, 16, 20,
	20, 16, 15,

	0, 21, 27,
	21, 26, 27,
	27, 26, 65,
	65, 26, 28,

	26, 25, 28,
	28, 25, 68,
	68, 25, 29,
	25, 24, 29,
	29, 24, 33,
	24, 18, 33,
	18, 19, 33,
	19, 69, 33,
	19, 32, 69,

	19, 20, 32,
	32, 20, 72,
	20, 31, 72,
	20, 15, 31,
	15, 14, 31,
	14, 13, 31,

	27, 65, 30,
	65, 28, 30,
	28, 68, 30,
	68, 29, 30,
	33, 69, 34,
	69, 32, 34,
	32, 72, 34,
	72, 31, 34,

	0, 27, 1,
	1, 27, 37,
	27, 30, 37,
	30, 29, 37,
	29, 64, 37,
	29, 33, 64,
	33, 41, 64,
	33, 34, 41,
	34, 31, 41,
	31, 13, 41,
	13, 12, 41,
	1, 37, 2,
	2, 37, 46,
	37, 62, 46,//
	37, 64, 62,//
	64, 41, 62,//
	41, 52, 62,
	41, 12, 52,
	2, 46, 3,
	3, 46, 4,
	4, 46, 5,
	5, 46, 6,
	6, 46, 7,
	7, 46, 62,//
	62, 52, 7,//
	7, 52, 8,
	8, 52, 9,
	9, 52, 10,
	10, 52, 11,
	11, 52, 12,

	76, 0, 1,//#
	76, 1, 2,
	
	76, 2, 3,
	76, 3, 4,
	76, 4, 5,
	76, 5, 6,
	76, 6, 7,
	76, 7, 75,
	75, 7, 8,
	75, 8, 9,
	75, 9, 10,
	75, 10, 11,
	75, 11, 12,
	75, 12, 13,
	75,13,14
};
GazeCorrector::GazeCorrector()
{
	//this->sealess_image = NULL;
	//this->transfer_image = NULL;
	//this->mid01_image = NULL;
	for (int i = 0; i < TrianglesNum; i++){
		all_Invet_Mat[i] = cvCreateMat(3, 3, CV_32FC1);//创建矩阵
	}
	matA = cvCreateMat(3, 3, CV_32FC1);//创建矩阵
	matY= cvCreateMat(3, 1, CV_32FC1);
	matX = cvCreateMat(3, 1, CV_32FC1);
}
GazeCorrector::~GazeCorrector()
{
	//cvReleaseImage(&transfer_image);
	//cvReleaseImage(&sealess_image);
	//cvReleaseImage(&mid01_image);
	cvReleaseMat(&matA);
	for (int i = 0; i < TrianglesNum; i++){
		cvReleaseMat(&all_Invet_Mat[i]);//创建矩阵
	}
	cvReleaseMat(&matX);
	cvReleaseMat(&matY);
}

bool GazeCorrector::isInTriangle(CvPoint *a, CvPoint*b, CvPoint *c, CvPoint &p, float *landas){
	//先用向量叉乘判断是否在三角形内，如果是在继续计算landas
	CvPoint ab, ap, bc, bp, ca, cp;
	ab = cvPoint(b->x - a->x, b->y - a->y);
	ap = cvPoint(p.x - a->x, p.y - a->y); 
	bc = cvPoint(c->x - b->x,c->y - b->y); 
	bp = cvPoint(p.x - b->x, p.y - b->y); 
	ca = cvPoint(a->x - c->x, a->y - c->y);
	cp = cvPoint(p.x - c->x, p.y - c->y); 
	int y1 = ab.x*ap.y - ap.x*ab.y;//注意y坐标是向下递增的。
	int y2 = bc.x*bp.y - bp.x*bc.y;
	int y3 = ca.x*cp.y - cp.x*ca.y;
	if (y1 < 0 || y2 < 0 || y3 < 0){
		return 0;
	}
	CvMat* matAP = all_Invet_Mat[cur_triangeIndex];//创建矩阵
	CV_MAT_ELEM(*matY, float, 0, 0) = p.x;
	CV_MAT_ELEM(*matY, float, 1, 0) = p.y;
	CV_MAT_ELEM(*matY, float, 2, 0) = 1.f;
	cvMatMul(matAP, matY, matX);
	landas[0] = CV_MAT_ELEM(*matX, float, 0, 0);
	landas[1] = CV_MAT_ELEM(*matX, float, 1, 0);
	landas[2] = CV_MAT_ELEM(*matX, float, 2, 0);
	
	return 1;
};
void GazeCorrector::FaceWarp(vector<CvPoint> &original_land, vector<CvPoint> &modified_land, IplImage *result){
	int64 Start_time;
	int64 Finish_time;
	double TickFrequency = 1000.0 / cv::getTickFrequency();
	Start_time = cv::getTickCount();

	IplImage *tmpimage = cvCloneImage(result);
	int width = tmpimage->width;
	int height = tmpimage->height;
	int x, y;
	bool is_intriangle = false;
	float landas[3] = { 0 };
	cur_triangeIndex = 0;
	CvScalar value;
	double map_x = 0, map_y = 0;
	CvScalar value00, value01, value10, value11;
	double aa, bb;//smoth 
	CvPoint original_face_center = cvPoint(0, 0), modified_face_center = cvPoint(0, 0);//use for align face transfer
	//calculate face_left face_right face_buttom
	face_left = width; face_right = 0; face_buttom = 0;
	for (int i = 0; i < 73; i++){
		if (original_land[i].x < face_left)face_left = original_land[i].x;
		if (original_land[i].x > face_right)face_right = original_land[i].x;
		if (original_land[i].y >face_buttom)face_buttom = original_land[i].y;
	}
	//calculate face center, avg of eye and mouth 27-34 46-64 total 29 points
	int ox = 0, oy = 0, mx = 0, my = 0;
	for (int i = 27; i < 35; i++){
		ox += original_land[i].x;
		oy += original_land[i].y;
		mx += modified_land[i].x;
		my += modified_land[i].y;
	}
	int ox1 = 0, oy1 = 0, mx1 = 0, my1 = 0;
	for (int i = 46; i < 64; i++){
		ox1 += original_land[i].x;
		oy1 += original_land[i].y;
		mx1 += modified_land[i].x;
		my1 += modified_land[i].y;
	}
	original_face_center.x = ((ox / 8 + ox1 / 21) / 2);
	original_face_center.y = ((oy / 8 + oy1 / 21) / 2);
	modified_face_center.x = ((mx / 8 + mx1 / 21) / 2);
	modified_face_center.y = ((my / 8 + my1 / 21) / 2);
	
	//calculate AABB
	int face_half_width = (face_right - face_left) / 2;
	int face_half_height = face_buttom - original_face_center.y;
	float scale = 0.5;
	 l_x = face_left- scale*face_half_width;
	 l_y = original_face_center.y - (1.3)*face_half_height;
	 m_x = original_face_center.x;
	 r_x = face_right + scale*face_half_width;
	 r_y = face_buttom + scale*face_half_height;
	 l_x = l_x ? l_x : 0;
	 l_y = l_y ? l_y : 0;
	 r_x = r_x >= width ? width-1 : r_x;
	 r_y = r_y >= height ? height-1 : r_y;

	 original_land[73].x = l_x; original_land[73].y = l_y; 
	 original_land[74].x = r_x; original_land[74].y = l_y;
	 original_land[75].x = r_x; original_land[75].y = r_y;
	 original_land[76].x = l_x; original_land[76].y = r_y;
	 original_land[77].x = m_x; original_land[77].y = l_y;

	 modified_land[73].x = l_x; modified_land[73].y = l_y;
	 modified_land[74].x = r_x; modified_land[74].y = l_y;
	 modified_land[75].x = r_x; modified_land[75].y = r_y;
	 modified_land[76].x = l_x; modified_land[76].y = r_y;
	 modified_land[77].x = m_x; modified_land[77].y = l_y;

	//crete seam init path chin0-14 + 7个对称的 22个 顺时针方向 前10不用进行优化
	vector<CvPoint> seam;
	for (int i = 12; i >= 0; i--){
		seam.push_back(modified_land[i]);
	}
	for (int i = 4; i <11; i++){
		seam.push_back(cvPoint(modified_land[i].x, modified_land[i].y - (modified_land[i].y - modified_face_center.y) * 2));
	}
	seam.push_back(modified_land[14]);
	seam.push_back(modified_land[13]);
	seam.push_back(seam[0]);


	//init backup buffer
	for (int i = 0; i < MaxWidth; i++)lastTriangleIndex[i] = -1;
	//pre calculate matrix for each triangle M-1
	for (int i = 0; i < TrianglesNum; i++){
		CvPoint a = modified_land[trianglesindex[i][0]];
		CvPoint b = modified_land[trianglesindex[i][1]];
		CvPoint c = modified_land[trianglesindex[i][2]];
		CV_MAT_ELEM(*matA, float, 0, 0) = a.x;
		CV_MAT_ELEM(*matA, float, 0, 1) = b.x;
		CV_MAT_ELEM(*matA, float, 0, 2) = c.x;
		CV_MAT_ELEM(*matA, float, 1, 0) = a.y;
		CV_MAT_ELEM(*matA, float, 1, 1) = b.y;
		CV_MAT_ELEM(*matA, float, 1, 2) = c.y;
		CV_MAT_ELEM(*matA, float, 2, 0) = 1.f;
		CV_MAT_ELEM(*matA, float, 2, 1) = 1.f;
		CV_MAT_ELEM(*matA, float, 2, 2) = 1.f;
		cvInvert(matA, all_Invet_Mat[i], CV_LU);//计算矩阵的ni -1
	}

	//warp
	int sum = 0, num = 0;
	for (int h = l_y; h < r_y; h++){
		for (int w = l_x; w < r_x; w++){
			sum++;
			is_intriangle = false;
			if (w>0 && lastTriangleIndex[w - 1] != -1){//若左边已经计算了
				cur_triangeIndex = lastTriangleIndex[w - 1];
				is_intriangle = isInTriangle(&modified_land[trianglesindex[lastTriangleIndex[w - 1]][0]], &modified_land[trianglesindex[lastTriangleIndex[w - 1]][1]], \
					&modified_land[trianglesindex[lastTriangleIndex[w - 1]][2]], cvPoint(w, h), landas);
				if (is_intriangle){//若跟左边点在同个三角形 更新信息
					lastTriangleIndex[w] = lastTriangleIndex[w - 1];
				}
				else if (lastTriangleIndex[w] != -1){//不然看是否跟上面的点在同一三角形
					cur_triangeIndex = lastTriangleIndex[w];
					is_intriangle = isInTriangle(&modified_land[trianglesindex[lastTriangleIndex[w]][0]], &modified_land[trianglesindex[lastTriangleIndex[w]][1]], \
						&modified_land[trianglesindex[lastTriangleIndex[w]][2]], cvPoint(w, h), landas);
					if (is_intriangle){
						//cur_triangeIndex = lastTriangleIndex[w];
					}
				}
			}
			
			if (!is_intriangle){//若左边还没计算,或没有邻居信息，那就搜索。
				num++;
				for (int i = 0; i < TrianglesNum; i++){
					cur_triangeIndex = i;
					is_intriangle = isInTriangle(&modified_land[trianglesindex[i][0]], &modified_land[trianglesindex[i][1]], \
						&modified_land[trianglesindex[i][2]], cvPoint(w, h), landas);
					if (is_intriangle){
						lastTriangleIndex[w] = i;
						break;
					}
				}
			}
			if (is_intriangle){
				// 得到landas 进行仿射变换
				map_x = map_y = 0;
				for (int index = 0; index < 3; index++){
					map_x += landas[index] * (original_land[trianglesindex[cur_triangeIndex][index]].x);
					map_y += landas[index] * (original_land[trianglesindex[cur_triangeIndex][index]].y);
				}
				if (map_x >= width)
					map_x = width - 2;
				if (map_y >= height)
					map_y = height - 2;
				if (map_x < 0)
					map_x = 0;
				if (map_y < 0)
					map_y = 0;
				//warp 像素分界优化
				aa = map_x - ((int)map_x);
				bb = map_y - ((int)map_y);
				value00 = cvGet2D(tmpimage, (int)map_y, (int)map_x);
				value01 = cvGet2D(tmpimage, (int)(map_y + 1), (int)map_x);
				value10 = cvGet2D(tmpimage, (int)map_y, (int)(map_x + 1));
				value11 = cvGet2D(tmpimage, (int)(map_y + 1), (int)(map_x + 1));
				for (int ii = 0; ii < 3; ii++){
					value.val[ii] = (1 - aa)*(1 - bb)*value00.val[ii] + aa*(1 - bb)*value10.val[ii] + aa*bb*value11.val[ii] + (1 - aa)*bb*value01.val[ii];
				}
				//value.val[0] = 0; value.val[1] = 1;
				cvSet2D(result, h, w, value);
			}
			else{
				cout << "desn't in any triangle! BUG!!!!" << endl;
			}
		}
	}
	
	//完成warp
	//根据seam 将modified face transfer original上
	IplImage *tmpimage2 = cvCloneImage(result);
	cvCopyImage(tmpimage, result);

	OptimizeSeam(tmpimage, tmpimage2, seam, modified_face_center, cvPoint(l_x, l_y), cvPoint(r_x,r_y));

	x_offset = modified_face_center.x - original_face_center.x;
	y_offset = modified_face_center.y - original_face_center.y;
	CvPoint point_transfer;
	//cvZero(result);
	// creat polygon map
	IplImage * polygonMap = cvCreateImage(cvSize(result->width, result->height), result->depth, 1);
	cvZero(polygonMap);
	CvPoint * polygonVertex = new CvPoint[seam.size()];
	for (int i = 0; i < seam.size(); i++){
		polygonVertex[i] = seam[i];
	}
	cvFillConvexPoly(polygonMap, polygonVertex, seam.size(), cvScalar(255));

	int counter = 0;
	for (int h = l_y; h < r_y; h++){
		for (int w = l_x; w < r_x; w++){
			//point_transfer.x = w;
			//point_transfer.y = h;
			//if (isInPolygon(seam, modified_face_center,point_transfer)){// 比两个参数的性能的性能低约20ms
			if (cvGet2D(polygonMap,h,w).val[0]){// 比起上面的方法性能提升 30+ ms
				value = cvGet2D(tmpimage2, h, w);
				counter++;
				cvSet2D(result, h, w, value);
			}
		}
	}
	//cout <<"clone points: "<< counter<<endl;
	//if (this->transfer_image){
	//	cvCopyImage(result, this->transfer_image);
	//}
	//else { 
	//	this->transfer_image = cvCloneImage(result); 
	//}

	// creat seam path
	vector<CvPoint> blend_seam_path2;
	IplImage *black2 = cvCreateImage(cvSize(result->width, result->height), result->depth, 1);
	cvZero(black2);
	for (int i = 0; i != seam.size() - 1; i++){
		cvLine(black2, seam[i], seam[i + 1], cvScalar(255), 5);
	}
	for (int i = 0; i < black2->width; i++){
		for (int j = 0; j < black2->height; j++){
			if (cvGet2D(black2, j, i).val[0])blend_seam_path2.push_back(cvPoint(i, j));
		}
	}
	
	/*
	//interplation
	vector<CvPoint> blend_seam_path;
	IplImage *black = cvCreateImage(cvSize(result->width, result->height), result->depth, 1);
	cvZero(black);
	for (int i = 12; i != seam.size() - 3; i++){
		cvLine(black, seam[i], seam[i + 1], cvScalar(255), 2);
	}
	for (int i = 0; i < black->width; i++){
		for (int j = 0; j < black->height; j++){
			if (cvGet2D(black, j, i).val[0])blend_seam_path.push_back(cvPoint(i, j));
		}
	}
	IplImage *aImage = cvCreateImage(cvSize(result->width, result->height), result->depth, 4);
	cvSet(aImage, cvScalar(255, 255, 255, 0xff));
	cvCvtColor(result, aImage, CV_BGR2BGRA);
	for (int i = 0; i < blend_seam_path.size(); i++){
		cvSet2D(aImage, blend_seam_path[i].y, blend_seam_path[i].x, cvScalar(255, 255, 255, 0));
	}
	this->seamOptimalUsingEigen((unsigned char *)aImage->imageData, aImage->width, aImage->height, aImage->nChannels);
	cvCvtColor(aImage, result, CV_BGRA2BGR);
	*/
	// smooth seam
	IplImage *smooth_src = cvCreateImage(cvSize(result->width, result->height), result->depth, result->nChannels);
	cvCopyImage(result, smooth_src);
	IplImage *smooth_dst = cvCreateImage(cvSize(result->width, result->height), result->depth, result->nChannels);
	cvSmooth(smooth_src, smooth_dst, CV_GAUSSIAN);
	int blend_x, blend_y;
	CvScalar blend_scalar;
	for (int i = 0; i < blend_seam_path2.size(); i++){
		blend_x = blend_seam_path2[i].x;
		blend_y = blend_seam_path2[i].y;
		blend_scalar = cvGet2D(smooth_dst, blend_y, blend_x);
		cvSet2D(result, blend_y, blend_x, blend_scalar);
	}
	
	//cout << blend_seam_path2.size() << endl;
	//if (this->mid01_image){
	//	cvCopyImage(tmpimage2, this->mid01_image);
	//}
	//else {
	//	this->mid01_image = cvCloneImage(tmpimage2);
	//}
	//if (this->sealess_image){
	//	cvCopyImage(result, this->sealess_image);
	//}
	//else {
	//	this->sealess_image = cvCloneImage(result);
	//}
	Finish_time = cv::getTickCount();
	cvReleaseImage(&black2);
	cvReleaseImage(&tmpimage);
	cvReleaseImage(&tmpimage2);
	cvReleaseImage(&smooth_dst);
	cvReleaseImage(&smooth_src);
	cvReleaseImage(&polygonMap);
	delete[] polygonVertex;
	
	COORD p; p.X = 0; p.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	cout << "Current output FPS: " << 1000/((Finish_time - Start_time)*TickFrequency) <<"\t";
	cout << "sum:" << sum << "\tnum:" << num << endl;
	return;
};

bool GazeCorrector::isInPolygon(vector<CvPoint> &polygon_points, CvPoint &p){//使用这样参数会减低性能
	//只能用于凸多边形
	CvPoint cur_next;
	CvPoint cur_p;
	for (int i = 0; i !=polygon_points.size()-1; i++){
		cur_next = cvPoint(polygon_points[i + 1].x - polygon_points[i].x, polygon_points[i + 1].y - polygon_points[i].y);
		cur_p = cvPoint(p.x - polygon_points[i].x, p.y - polygon_points[i].y);
		if ((cur_next.x*cur_p.y - cur_p.x*cur_next.y) < 0){
			return 0;
		};
	}
	return 1;
}; 
bool GazeCorrector::isInPolygon(vector<CvPoint> &polygon_points, CvPoint &center, CvPoint &p){//使用这样参数会减低性能
	CvPoint cur_next;
	CvPoint next_center;
	CvPoint center_cur;
	CvPoint cur_p;
	CvPoint next_p;
	CvPoint center_p;
	for (int i = 0; i != polygon_points.size() - 1; i++){
		cur_next = cvPoint(polygon_points[i + 1].x - polygon_points[i].x, polygon_points[i + 1].y - polygon_points[i].y);
		next_center = cvPoint(center.x - polygon_points[i + 1].x, center.y - polygon_points[i+1].y);
		center_cur = cvPoint(polygon_points[i].x - center.x, polygon_points[i].y-center.y);
		cur_p = cvPoint(p.x - polygon_points[i].x, p.y - polygon_points[i].y);
		next_p = cvPoint(p.x - polygon_points[i+1].x, p.y - polygon_points[i+1].y);
		center_p = cvPoint(p.x - center.x, p.y - center.y);
		bool inthis=true;
		if ((cur_next.x*cur_p.y - cur_p.x*cur_next.y) < 0){
			inthis = false;
		};
		if ((next_center.x*next_p.y - next_p.x*next_center.y) < 0){
			inthis = false;
		};
		if ((center_cur.x*center_p.y - center_p.x*center_cur.y) < 0){
			inthis = false;
		};
		if (inthis){
			return true;
		}
	}
	return 0;
};
float blockAvgIntensity(IplImage *image, CvPoint &point){
	float sum = 0;
	//TODO: here has out of range bug!
	for (int i = point.x - 2; i < point.x + 3; i++){
		for (int j = point.y - 2; j < point.y + 3; j++){
			sum += cvGet2D(image, i, j).val[0];
		}
	}
	return sum / 25.0;
}
void GazeCorrector::OptimizeSeam(IplImage *original, IplImage *modified, vector<CvPoint> &seam, CvPoint &center, CvPoint &left_upper, CvPoint &right_buttom){
	IplImage *original_gray = cvCreateImage(cvSize(original->width, original->height), original->depth,1);
	IplImage *modified_gray = cvCreateImage(cvSize(modified->width, modified->height), modified->depth, 1);
	cvCvtColor(original, original_gray, CV_BGR2GRAY);
	cvCvtColor(modified, modified_gray, CV_BGR2GRAY);
	int start_pos = 12; //just optimize from start_pos to seam.size();
	float threshold = 5;
	CvPoint minP;
	float minS;
	float diff_o_m;
	float original_intensity, modified_intensity;
	for (int i = start_pos; i != seam.size()-2; i++){
		minS = 255;
		minP = seam[i];
		int iterator = 0;
		while (1){
			iterator++;
			original_intensity = blockAvgIntensity(original_gray, seam[i]);
			modified_intensity = blockAvgIntensity(modified_gray, seam[i]);
			diff_o_m = fabs(original_intensity - modified_intensity);
			if (diff_o_m < threshold){
				break;
			}
			else{
				if (diff_o_m < minS){
					minS = diff_o_m; 
					minP = seam[i]; 
				}
				float k = sqrt(pow(abs(seam[i].y - center.y), 2) + pow(abs(seam[i].x - center.x), 2));
				seam[i].x = seam[i].x + 2 / k*(seam[i].x - center.x);
				seam[i].y = seam[i].y + 2 / k*(seam[i].y - center.y);
				if (seam[i].x < left_upper.x)seam[i].x = left_upper.x;
				if (seam[i].y < left_upper.y)seam[i].y = left_upper.y;
				if (seam[i].x > right_buttom.x)seam[i].x = right_buttom.x;
				if (seam[i].y > right_buttom.y)seam[i].y = right_buttom.y;
				
			}
			if (iterator > 10){
				seam[i] = minP;
				break;
			}
		}
	}
	cvReleaseImage(&original_gray);
	cvReleaseImage(&modified_gray);
};

void GazeCorrector::seamOptimalUsingEigen(unsigned char * imageData, int width, int height, int PixelChannels)
{
	//此方法为不保留gradient的均匀插值混合。
	//image data 像素格式为RGBA
	//其中A的值小于0xff的表示带求解的poisson区域
	const int size = width * height;
	const int totalImageBytes = size * PixelChannels;
	const int xOffset[4] = { 0, -1, +1, 0 };
	const int yOffset[4] = { -1, 0, 0, +1 };
	int indexOffset[4];			//某个像素点的某通道
	int anchorPointOffset[4]; //像素alpha锚点

	indexOffset[0] = -width;
	indexOffset[1] = -1;
	indexOffset[2] = +1;
	indexOffset[3] = +width;

	anchorPointOffset[0] = -width * PixelChannels;
	anchorPointOffset[1] = -PixelChannels;
	anchorPointOffset[2] = PixelChannels;
	anchorPointOffset[3] = width * PixelChannels;
	

	//count poisson pixel num
	int possionNum = 0; 
	for (int i = 3; i<totalImageBytes; i += PixelChannels)
	if (imageData[i] < 0xff)
		possionNum++;

	if (possionNum == 0)
	{
		printf("No need to perform background Poisson processing.\n");
		return;
	}

	// create poisson needed pixel maps between poisson index and image index
	int * mapPoisson2Tex = (int *)malloc(possionNum *sizeof(int));
	int * mapTex2Poisson = (int *)malloc(size * sizeof(int));
	int pIndex = 0;
	int anchorPoint = 3;//alpha 通道
	for (int i = 0; i<size; i++)
	{
		if (imageData[anchorPoint] < 0xff)
		{
			mapTex2Poisson[i] = pIndex;
			mapPoisson2Tex[pIndex++] = i;
		}

		anchorPoint += PixelChannels;
	}
	printf("        possion num = %d\n", possionNum);

	//create and init a, x and b
	Eigen::SparseMatrix<float> A(possionNum, possionNum);
	A.reserve(Eigen::VectorXi::Constant(possionNum, 5));
	Eigen::VectorXf bR(possionNum);
	Eigen::VectorXf bG(possionNum);
	Eigen::VectorXf bB(possionNum);
	Eigen::VectorXf xR(possionNum);
	Eigen::VectorXf xG(possionNum);
	Eigen::VectorXf xB(possionNum);


	int selfIndex, selfPosX, selfPosY;
	int neiborCount = 0;
	int neiborNeedPossionCount = 0;
	int noneNeiborSumR, noneNeiborSumG, noneNeiborSumB;
	int neiborPosX, neiborPosY, neiborIndex;
	bool isNeiborAvalable[4];
	int neiborAnchorPoint = 0;
	anchorPoint = 0;


	for (int i = 0; i<possionNum; i++)
	{
		selfIndex = mapPoisson2Tex[i];
		selfPosX = selfIndex % width;
		selfPosY = selfIndex / width;
		anchorPoint = selfIndex * PixelChannels;
		// for point (selfPosX, selfPosY)

		// count neibor
		neiborCount = 0;
		neiborNeedPossionCount = 0;
		noneNeiborSumR = noneNeiborSumG = noneNeiborSumB = 0;
		for (int j = 0; j<4; j++)
		{
			neiborPosX = selfPosX + xOffset[j];//上 左 右 下
			neiborPosY = selfPosY + yOffset[j];
			isNeiborAvalable[j] = false;

			if ((neiborPosX >= 0 && neiborPosX < width) && (neiborPosY >= 0 && neiborPosY < height))
			{
				neiborCount++;

				neiborAnchorPoint = anchorPoint + anchorPointOffset[j];
				if (imageData[neiborAnchorPoint + 3] == 0xff)
				{
					//已知的.
					noneNeiborSumR += imageData[neiborAnchorPoint];
					noneNeiborSumG += imageData[neiborAnchorPoint + 1];
					noneNeiborSumB += imageData[neiborAnchorPoint + 2];
				}
				else
				{
					isNeiborAvalable[j] = true;//neibor is poisson
					neiborNeedPossionCount++;
				}
			}
		}

		// fill matrix
		for (int j = 0; j<2; j++)
		{
			if (isNeiborAvalable[j])
			{// neibor is poisson
				neiborIndex = selfIndex + indexOffset[j];
				A.insert(i, mapTex2Poisson[neiborIndex]) = -1.0f;
			}

		}

		A.insert(i, i) = (float)neiborCount;

		for (int j = 2; j<4; j++)
		{
			if (isNeiborAvalable[j])
			{
				neiborIndex = selfIndex + indexOffset[j];
				A.insert(i, mapTex2Poisson[neiborIndex]) = -1.0f;
			}
		}

		bR(i) = noneNeiborSumR;
		bG(i) = noneNeiborSumG;
		bB(i) = noneNeiborSumB;
	}
	A.makeCompressed();

	//perform lu factor
	Eigen::SparseLU< Eigen::SparseMatrix<float> > solver;
	solver.compute(A);
	if (solver.info() != Eigen::Success)
	{
		printf("Failed to perform lu factor.\n");
		return;
	}

	//evaluation
	xR = solver.solve(bR);
	if (solver.info() != Eigen::Success)
	{
		printf("Failed to evalute red channel.\n");
		return;
	}

	xG = solver.solve(bG);
	if (solver.info() != Eigen::Success)
	{
		printf("Failed to evalute red channel.\n");
		return;
	}

	xB = solver.solve(bB);
	if (solver.info() != Eigen::Success)
	{
		printf("Failed to evalute red channel.\n");
		return;
	}

	//write back to imageData
	int R, G, B;
	for (int i = 0; i<possionNum; i++)
	{
		R = (int)xR(i);
		G = (int)xG(i);
		B = (int)xB(i);

		R = min(max(R, 0), 255);
		G = min(max(G, 0), 255);
		B = min(max(B, 0), 255);

		anchorPoint = mapPoisson2Tex[i] * PixelChannels;

		imageData[anchorPoint] = R;
		imageData[anchorPoint + 1] = G;
		imageData[anchorPoint + 2] = B;
		imageData[anchorPoint + 3] = 0xff;
	}

	//release memory
	free(mapTex2Poisson);
	free(mapPoisson2Tex);
}