#include "GazeCorrector.h"
#include "VideoWidget_Base.h"
#include <vector>
#include <Qthread>
#include "DDE\DDEApplyDLL.h"
using namespace std;
//calibration step
typedef enum _C_status{
	C_None,
	C_Face_to_Screen,
	C_Face_to_Camera,
	C_Calibrated
}C_status;


class GazeController : public QThread
{
	Q_OBJECT
signals:
	void sendImg(IplImage*);
protected:
	void run();
public:
	GazeController();
	void setInputAndOutput(VideoWidget_Base * input_ref, VideoWidget_Base * output_ref);
	~GazeController();
	void dde_init();
	void dde_first_track();
	void dde_track();
	void calibration();
public slots:
	void setData(vector<CvPoint> a, vector<CvPoint> b);
private:
	void routine();
private:
	VideoWidget_Base * input;
	VideoWidget_Base * output;
	IplImage * m_img;
	bool loop;
	//for debug
	vector<CvPoint> original_land;
	vector<CvPoint> modified_land;
	GazeCorrector * gazecorrector;
	cDDEWrapper  *m_ddeWrapper;
	sFrameParam m_frmParam;
	C_status m_calibration_status;

	//calibration para
	vector<CvPoint> m_face_to_screen_land;
	vector<CvPoint> m_face_to_camera_land;
	vector<CvPoint> m_diff_of_calibration;
	float m_init_scale_x;
	float m_init_scale_y;
	//
};
