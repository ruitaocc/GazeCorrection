#include <vector>
#include <opencv/cv.h>
#include<opencv/highgui.h>
#include<ctime>

using namespace std;

class GazeCorrector
{
public:
	GazeCorrector();
	
public:
	~GazeCorrector();
	//interface
	void FaceWarp(vector<CvPoint> &original_land, vector<CvPoint> &modified_land, IplImage *original);
	//mid data
	//IplImage *transfer_image;//for debug
	//IplImage *mid01_image;//for debug
	//IplImage *sealess_image;//for debug
private:
	void OptimizeSeam(IplImage *original, IplImage *modified, vector<CvPoint> &seam, CvPoint &center, CvPoint &left_upper, CvPoint &right_buttom);
	void seamOptimalUsingEigen(unsigned char * imageData, int width, int height, int PixelChannels);

	bool isInTriangle(CvPoint *a, CvPoint*b, CvPoint *c, CvPoint &p, float *landas); 
	bool isInPolygon(vector<CvPoint> &polygon_points, CvPoint &p);//ֻ������͹�����
	bool isInPolygon(vector<CvPoint> &polygon_points, CvPoint &center, CvPoint &p);
private:
	int face_left, face_right, face_buttom;//��¼��������ߣ����±ߣ����ұ� ���ڼ����Χ��
	int l_x, l_y, m_x, r_x, r_y;//��Χ��AABB
	int x_offset, y_offset;//modified face tranfer vector
	int cur_triangeIndex = 0;
};

