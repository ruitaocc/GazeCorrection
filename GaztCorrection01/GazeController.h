#include "GazeCorrector.h"
#include "videoWidget.h"
#include <vector>
#include <Qthread>
using namespace std;
class GazeController : public QThread
{
	Q_OBJECT
signals:
	void sendImg(IplImage*);
protected:
	void run();
public:
	GazeController();
	void setInputAndOutput(VideoWidget * input_ref, VideoWidget * output_ref);
	~GazeController();
public slots:
	void setData(vector<CvPoint> a, vector<CvPoint> b);
private:
	void routine();
private:
	VideoWidget * input;
	VideoWidget * output;
	IplImage * m_img;
	bool loop;
	//for debug
	vector<CvPoint> original_land;
	vector<CvPoint> modified_land;
	GazeCorrector * gazecorrector;
};
