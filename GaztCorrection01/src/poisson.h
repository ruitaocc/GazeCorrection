#ifndef POISSON_H
#define POISSON_H

#include "image.h"

// TODO(cbraley): Allow arbitrary number of color channels.

class PoissonImageEditor {
 public:
  bool Edit(const Image<unsigned char, 3>& foreground,
      const Image<bool, 1>& foreground_mask,
      Image<unsigned char, 3>* bg_to_edit);
};

#endif  // POISSON_H
