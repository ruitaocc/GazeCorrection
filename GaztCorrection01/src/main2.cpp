//#define THIS_FILE
#ifdef THIS_FILE
#include <iostream>
#include <string>
#include <vector>

#include "image.h"
#include "image_io.h"
#include "poisson.h"

int main(int argc, char** argv) {
  // Validate command line arguments. -----------------------------------------
  if (argc != 5) {
    std::cerr << "Input format is " << std::endl
        << argv[0] << " bg.png fg.png matte.png out.png" << std::endl;
    return 1;
  }

  // Load the input data. -----------------------------------------------------
  std::cout << "Trying to load images..." << std::endl;
  Image<unsigned char, 3> bg(1,1);
  Image<unsigned char, 3> fg(1,1);
  Image<unsigned char, 3> matte(1,1);
 Image<unsigned char, 3>* images[3] = {&bg, &fg, &matte};
 for (int i = 0; i < 3; ++i) {
    bool ok = image_io::LoadRGBLDR(std::string(argv[1 + i]), images[i]);
    if (!ok) {
      std::cerr << "Failed to load " << argv[1 + i] << std::endl;
      return 1;
    } else {
      std::cout << "\tSuccessfully loaded: " << argv[1 + i] << std::endl;
    }
  }
  
  std::cout << "Successfully loaded all input images!" << std::endl;

  // Turn the 8-bit RGB input image into a binary mask. -----------------------
  std::cout << "Turning the matte image into a binary mask..." << std::endl;
  ThreshholdMaskMaker<unsigned char> masker(100);
  Image<bool, 1> binary_mask(1, 1);
  int num_pix_in_mask = matte.Mask(masker, &binary_mask);
  Image<unsigned char, 1> draw_mask(1, 1);
  binary_mask.Convert<unsigned char, BinaryMaskToEightBitConverter>(&draw_mask);
  if (!image_io::WriteBWLDR(draw_mask, "temp.png")) {
    std::cerr << "Could not write converted mask!" << std::endl;
    return 1;
  }
  std::cout << "Done creating mask, found " << num_pix_in_mask
      << " masked pixels, out of " << matte.Width() * matte.Height()
      << " available pixels." << std::endl;

  // Make sure image dimensions are appropriate. ------------------------------
  if (binary_mask.Width() != fg.Width() ||
      binary_mask.Height() != fg.Height()) {
    std::cerr << "Mask image dimensions must match foreground image "
              << "dimensions!" << std::endl;
    return 3;
  }


  // Setup the poisson equation. ----------------------------------------------
  PoissonImageEditor image_editor;
  bool success = image_editor.Edit(fg, binary_mask, &bg);
  if (!success) {
    std::cerr << "Image editing failed!" << std::endl;
    return 4;
  }

  // Write the output
  // TODO(cbraley): Do for real!
  if (!image_io::WriteRGBLDR(bg, argv[4])) {
    std::cerr << "Could not write output to: " << argv[4] << std::endl;
    return 2;
  } else {
    std::cout << "Successfully wrote to: " << argv[4] << std::endl;
  }

  return 0;
}
#endif