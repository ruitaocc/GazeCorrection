#ifndef image_io_h
#define image_io_h

#include <string>
#include <exception>
#include <cassert>

#include "image.h"

namespace image_io {

bool WriteBWLDR(const Image<unsigned char, 1>& out,
    const std::string& image_file_out);

bool WriteRGBLDR(const Image<unsigned char, 3>& out,
    const std::string& image_file_out);

bool LoadRGBLDR(const std::string& image_file,
    Image<unsigned char, 3>* out);

}  // namespace image_io


#endif  // image_io_h
