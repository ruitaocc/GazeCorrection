#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <algorithm>

class ImageBase {
 public:
  static const int DYNAMIC_NUM_CHANNELS;
 protected:
  ~ImageBase() {}
};

namespace pixel_ops {

template<typename PixelT>
PixelT Magnitude(const PixelT* const pixels, const int num_channels) {
  double accum = 0.0;
  for (int i = 0; i < num_channels; ++i) {
    const double curr(pixels[i]);
    accum += curr * curr;
  }
  return PixelT(sqrt(accum));
}

}  // namespace pixel_ops

typedef struct Vec {
 public:
  Vec(double xVal, double yVal) : x(xVal), y(yVal) {}
  inline double Magnitude() const { return sqrt(x * x + y * y); }
  double x, y;
} Vec;


// Helper structs for masking images ------------------------------------------
// TODO(cbraley): RM the masking stuff...

template<typename MaskMakerPixelT>
struct MaskMaker {
  public:
  virtual bool IsMasked(const MaskMakerPixelT* const pixels,
      int channel_count) const = 0;
};

template<typename MaskMakerPixelT>
struct ThreshholdMaskMaker : public MaskMaker<MaskMakerPixelT> {
  public:
  ThreshholdMaskMaker(const MaskMakerPixelT& threshhold)
      : magnitude_threshhold_(threshhold) {}
  virtual bool IsMasked(const MaskMakerPixelT* const pixels,
      int channel_count) const {
    return pixel_ops::Magnitude(pixels, channel_count) >=
        magnitude_threshhold_;
  }
  private:
  MaskMakerPixelT magnitude_threshhold_;
};

// Helpers for converting between image types ---------------------------------

template<typename PixelTIn, typename PixelTOut>
struct SimpleCastConverter {
 public:
  inline PixelTOut operator()(const PixelTIn& in) const {
    return static_cast<PixelTOut>(in);
  }
};

struct BinaryMaskToEightBitConverter {
 public:
  inline unsigned char operator()(const bool in) const {
    return in ? 255 : 0;
  }
};

struct EightBitToBinaryConverter{
 public:
  inline bool operator()(const unsigned char in) const {
    return in == 255;
  }
};

template<typename PixelT, int NumChannels = ImageBase::DYNAMIC_NUM_CHANNELS>
class Image : public ImageBase {
 public:
  // Constructors and destructors ---------------------------------------------

  Image(int width, int height, int channels = DYNAMIC_NUM_CHANNELS)
      : buffer_(NULL), owns_buffer_(true),
        w_(width), h_(height), c_(channels) {
    // Make sure that the client is either setting the
    // number of channels at compile time or at runtime (not both).
    assert((NumChannels == DYNAMIC_NUM_CHANNELS && c_ > 0) ||
      (NumChannels > 0 && c_ == -1));

    // Allocate space.
    buffer_ = new PixelT[w_ * h_ * Channels()];

    // Zero the buffer out with whatever the default PixelT is.
    const PixelT default_value(0.0);
    Fill(default_value);
  }

  ~Image() {
    if (owns_buffer_) {
      delete[] buffer_;
    }
  }

  // Trivial accessors --------------------------------------------------------

  inline int Width() const { return w_; }
  inline  int Height() const { return h_; }
  inline int Channels() const { return GetNumChannels<NumChannels>(); }

  // Image resizing -----------------------------------------------------------

  void Resize(int new_width, int new_height) {
    if (w_ == new_width && h_ == new_height) {
      return;
    }

    PixelT* new_buf = new PixelT[new_width * new_height * Channels()];

    const int row_copy_bytes = std::min<int>(new_width, w_) *
        Channels() * sizeof(PixelT);
    for (int y = 0; y < std::min<int>(new_height, h_); ++y) {
      memcpy(static_cast<void*>(new_buf + (new_width * Channels() * y)),
        static_cast<void*>(buffer_ + (w_ * Channels() * y)),
        row_copy_bytes);
    }

    w_ = new_width;
    h_ = new_height;
    if (owns_buffer_) {
      delete[] buffer_;
    }
    buffer_ = new_buf;
  }

  // Getting pixel values -----------------------------------------------------

  inline PixelT GetPixelComponent(int x, int y, int c) const {
    assert(x >= 0 && y >= 0 && c >= 0);
    assert(x < Width());
    assert(y < Height());
    assert(c < Channels());
    return buffer_[y * (w_ * Channels()) + x * Channels() + c];
  }

  inline void GetPixel(int x, int y, PixelT* out) const {
    assert(x >= 0 && x >= 0);
    assert(x < Width());
    assert(y < Height());
    assert(out != NULL);
    memcpy(
        static_cast<void*>(out),
        static_cast<void*>(buffer_ + (y * (w_ * Channels()) + x * Channels())),
        Channels() * sizeof(PixelT));
  }

  void GetScanline(int y, PixelT* out) const {
    assert(y >= 0);
    assert(y < Height());
    assert(out != NULL);
    memcpy(
        static_cast<void*>(out),
        static_cast<void*>(buffer_ + (y * (w_ * Channels()))),
        w_ * Channels() * sizeof(PixelT));
  }

  // Setting pixel values -----------------------------------------------------

  void SetPixel(int x, int y, const PixelT* const pixel_values) {
    assert(x >= 0 && x >= 0);
    assert(x < Width());
    assert(y < Height());
    assert(pixel_values != NULL);
    memcpy(static_cast<void*>(buffer_ + (y * (w_ * Channels()) + x * Channels())),
        static_cast<const void* const>(pixel_values),
        Channels() * sizeof(PixelT));
  }

  void SetPixelComponent(int x, int y, int c, const PixelT& value) const {
    assert(x >= 0 && x >= 0 && c >= 0);
    assert(x < Width());
    assert(y < Height());
    assert(c < Channels());
    buffer_[y * (w_ * Channels()) + x * Channels() + c] = value;
  }


  // Fill each component with the same value.
  void Fill(const PixelT& value) const {
    // TODO(cbraley): This function could be made faster for various different
    // types.  For example, for byte sized PixelT's we could do a single memset.
    // For other primitive types, we could do sizeof(PixelT) strided loops.
    PixelT* iter = buffer_;
    const PixelT* end = buffer_ + Width() * Height() * Channels();
    while(iter < end) {  // TODO(cbraley): This could also be unrolled.
      *iter++ = value;
    }
  }

  // TODO(cbraley): Filling channels.


  // Changing image types -----------------------------------------------------

  // Convert an image into a binary image via a visitor function "mask_maker"
  // that operates at each pixel.
  int Mask(const MaskMaker<PixelT>& mask_maker, Image<bool, 1>* out_mask) const {
    assert(out_mask != NULL);

    int masked_pixel_count = 0;

    // Make the mask the same size as the output.
    out_mask->Resize(Width(), Height());
    // TODO(cbraley): Resize channels here if we knew width at compile time.

    PixelT* pixel = new PixelT[Channels()];  // TODO(cbraley): alloca

    for (int x = 0; x < Width(); ++x) {
      for (int y = 0; y < Height(); ++y) {
        GetPixel(x, y, pixel);
        const bool masked = mask_maker.IsMasked(pixel, Channels());
        out_mask->SetPixelComponent(x, y, 0, masked);
        if (masked) {
          ++masked_pixel_count;
        }
      }
    }
    delete[] pixel;

    return masked_pixel_count;
  }

  template<typename PixelTOut, typename ConversionFunc>
  void Convert(Image<PixelTOut, NumChannels>* out_image) const {
    out_image->Resize(Width(), Height());
    // TODO(cbraley): Resize channels here if we knew width at compile time.

    ConversionFunc converter;

    const int num_pix = Width() * Height() * Channels();
    PixelT* my_buf = buffer_;
    PixelTOut* their_buf = out_image->GetBuffer();

    // TODO(cbraley): Unroll!
    for (int i = 0; i < num_pix; ++i) {
      their_buf[i] = converter(my_buf[i]);
    }
  }


  // Sampling the image (non-integer coordinates) -----------------------------
  // TODO(cbraley): Code!


  // Image processing operations ----------------------------------------------

  // Simple method for getting the gradient based on a finite difference.
  inline Vec Gradient(int x, int y, int c) const {
    assert(c >= 0 && c < Channels());

    // Clamp the indices themselves to the grid!
    x = std::max<int>(std::min<int>(x, Width() - 1), 0);
    y = std::max<int>(std::min<int>(y, Height() - 1), 0);

    // Get finite difference indices, but clamp them to the image bounds.
    const int x_next = std::max<int>(std::min<int>(x + 1, Width() - 1), 0);
    const int x_prev = std::max<int>(std::min<int>(x - 1, Width() - 1), 0);
    const int y_next = std::max<int>(std::min<int>(y + 1, Height() - 1), 0);
    const int y_prev = std::max<int>(std::min<int>(y - 1, Height() - 1), 0);

    // Compute simple finite differences like: f'(x) = f(x+h) - f(x-h).
    return Vec(
        static_cast<double>(GetPixelComponent(x_next, y, c)) -
        static_cast<double>(GetPixelComponent(x_prev, y, c)),
        static_cast<double>(GetPixelComponent(x, y_next, c)) -
        static_cast<double>(GetPixelComponent(x, y_prev, c)));
  }

  // Scary stuff --------------------------------------------------------------

  // With great power, comes great responsibility.
  PixelT* GetBuffer() { return buffer_; }

 private:
  PixelT* buffer_;
  bool owns_buffer_;
  int w_, h_, c_;

  // We want to make sure that we properly inline the number of
  // channels if the caller is using a number of channels known
  // at compile time.  This allows the compiler to unroll loops and
  // generate faster code.  Therefore, whenever any code internal
  // code gets the number of channels, it should do so via
  // GetNumChannels.
  int num_channels_;

  // This is the default implementation of GetNumChannels(), that returns
  // the static number of channels that has been chosen at compile time.
  template<int FuncNumChannels>
  inline int GetNumChannels() const {
    if (FuncNumChannels == DYNAMIC_NUM_CHANNELS) {
      // TODO(cbraley): Make sure this case is inlined!
      return c_;
    } else {
      return FuncNumChannels;
    }
  }

  // Disallow default constructors and implicit copies.
  Image();
  Image(const Image& other);
  Image& operator=(const Image& rhs);
};



#endif  // IMAGE_H
