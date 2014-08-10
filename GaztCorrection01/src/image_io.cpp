#include "image_io.h"

#define cimg_use_png 1
#define cimg_display 0
//#include "cimg/CImg.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>
//using namespace cimg_library;

namespace image_io {

bool WriteBWLDR(const Image<unsigned char, 1>& out,
    const std::string& image_file_out) {
  try {
	  IplImage * image = cvCreateImage(cvSize(out.Width(), out.Height()),32, 3);
	  CvScalar s; for (int x = 0; x < out.Width(); ++x) {
      for (int y = 0; y < out.Height(); ++y) {
		  s.val[0] = out.GetPixelComponent(x, y, 0);
		  s.val[1] = out.GetPixelComponent(x, y, 0);
		  s.val[2] = out.GetPixelComponent(x, y, 0);
		  cvSet2D(image, y, x, s);
      }
    }
	  return cvSaveImage(image_file_out.c_str(), image);
  } catch (std::exception& ex) {
    return false;
  }
}


bool WriteRGBLDR(const Image<unsigned char, 3>& out,
    const std::string& image_file_out) {
  try {
	  IplImage * image = cvCreateImage(cvSize(out.Width(), out.Height()), 32, out.Channels());
	  CvScalar s;
    for (int x = 0; x < out.Width(); ++x) {
      for (int y = 0; y < out.Height(); ++y) {
		  s.val[0] = out.GetPixelComponent(x, y, 2);
		  s.val[1] = out.GetPixelComponent(x, y, 1);
		  s.val[2] = out.GetPixelComponent(x, y, 0);
		  cvSet2D(image, y, x, s);
      }
    }
	return cvSaveImage(image_file_out.c_str(), image);
  } catch (std::exception& ex) {
    return false;
  }
}

bool LoadRGBLDR(const std::string& image_file,
    Image<unsigned char, 3>* out) {
  try {
    unsigned char max = 0;
	IplImage * image = cvLoadImage(image_file.c_str());
	out->Resize(image->width, image->height);
    unsigned char rgb[3];
	int a, b, c;
    for (int x = 0; x < out->Width(); ++x) {
      for (int y = 0; y < out->Height(); ++y) {
		rgb[0] = static_cast<unsigned char>(cvGet2D(image,y,x).val[2]);
		rgb[1] = static_cast<unsigned char>(cvGet2D(image, y, x).val[1]);
		rgb[2] = static_cast<unsigned char>(cvGet2D(image, y, x).val[0]);
        max = std::max<unsigned char>(rgb[0], max);
        max = std::max<unsigned char>(rgb[1], max);
        max = std::max<unsigned char>(rgb[2], max);
        out->SetPixel(x, y, &(rgb[0]));
      }
    }
    std::cout << "largest pixel = " << max << std::endl;
    return true;
  } catch (std::exception& ex) {
    return false;
  }
}

}  // namespace image_io
