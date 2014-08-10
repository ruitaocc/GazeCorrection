#include "poisson.h"

#include "image_io.h"

#include <cassert>

#include <limits>
#include <string>
#include <iostream>
#include <set>

#include <Eigen/Sparse>

using Eigen::SparseMatrix;
using Eigen::VectorXd;
using Eigen::Triplet;

namespace {

double Divergence(int x, int y, int chan,
                  const Image<unsigned char, 3>& image) {
  Vec grad_u = image.Gradient(x, y + 1, chan);
  Vec grad_d = image.Gradient(x, y - 1, chan);
  Vec grad_l = image.Gradient(x - 1, y, chan);
  Vec grad_r = image.Gradient(x + 1, y, chan);
  return (grad_u.y - grad_d.y) + (grad_r.x - grad_l.x);
}

typedef struct Pixel {
  Pixel(int xIdx = -1, int yIdx = -1, int idx = -1) : x(xIdx), y(yIdx), index(idx) {}
  int x, y;
  int index;  // Used to track index in the linear system RHS.
  bool operator<(const Pixel& other) const {
    if (x == other.x) {
      return y < other.y;
    } else {
      return x < other.x;
    }
  }
} Pixel;

bool InBounds(const Pixel& pixel, const Image<unsigned char, 3>& image) {
  return pixel.x >= 0 && pixel.y >= 0 && pixel.x < image.Width() && pixel.y <
      image.Height();
}

int GetNeighbors(const Pixel& pixel,
                 const Image<unsigned char, 3>& image,
                 Pixel* out_pixels) {
  // Check if the input pixel itself is OOB.
  if (pixel.x < 0 || pixel.y < 0 || pixel.x >= image.Width() ||
      pixel.y >= image.Height()) {
    return 0;
  }

  int count = 0;
  Pixel pix;

  pix = Pixel(pixel.x - 1, pixel.y);  // Left neighbor.
  if (InBounds(pix, image)) { out_pixels[count++] = pix; }
  pix = Pixel(pixel.x + 1, pixel.y);  // Reft neighbor.
  if (InBounds(pix, image)) { out_pixels[count++] = pix; }
  pix = Pixel(pixel.x, pixel.y + 1);  // Up neighbor.
  if (InBounds(pix, image)) { out_pixels[count++] = pix; }
  pix = Pixel(pixel.x, pixel.y - 1);  // Down neighbor.
  if (InBounds(pix, image)) { out_pixels[count++] = pix; }

  return count;
}

}  // anonymous namespace

bool PoissonImageEditor::Edit(const Image<unsigned char, 3>& foreground,
    const Image<bool, 1>& foreground_mask,
    Image<unsigned char, 3>* bg_to_edit) {

  Image<unsigned char, 3> boundary(foreground.Width(), foreground.Height());
  boundary.Fill(0);

  // Check image dimensions.
  if (foreground.Width() != foreground_mask.Width() ||
      foreground.Width() != bg_to_edit->Width() ||
      foreground.Height() != foreground_mask.Height() ||
      foreground.Height() != bg_to_edit->Height()) {
    // TODO(cbraley): Support this!
    std::cerr << "Differing image dimensions are not currently supported." << std::endl;
    return false;
  }

  std::cout << "Creating PDE boundary value problem..." << std::endl;

  // Determine the number of unknown pixels, and make an index of all of them.
  std::set<Pixel> unknown_pixels;
  int counter = 0;
  for (int x = 0; x < foreground_mask.Width(); ++x) {
    for (int y = 0; y < foreground_mask.Height(); ++y) {
      if (foreground_mask.GetPixelComponent(x, y, 0)) {
        unknown_pixels.insert(Pixel(x, y, counter++));
      }
    }
  }
  if (unknown_pixels.empty()) {
    std::cerr << "There are no unknown pixels!" << std::endl;
    return false;
  }

  // Now, get the boundary condition values.
  std::set<Pixel> boundary_condition;
  Pixel neighbors[4];
  for (std::set<Pixel>::const_iterator iter = unknown_pixels.begin();
      iter != unknown_pixels.end(); ++iter) {
    // Check each of this pixel's direct neighbors.  If they are NOT in
    // unknown_pixels and are also in the image boundary, then they are
    // boundary condition pixels.

    Pixel curr = *iter;
    const int in_bounds_neighbor_count =
        GetNeighbors(curr, foreground, &(neighbors[0]));
    assert(in_bounds_neighbor_count <= 4 && in_bounds_neighbor_count >= 0);

    for (int i = 0; i < in_bounds_neighbor_count; ++i) {
      // Skip pixels outside the image boundary, and also skip pixels that
      // are in the list of unknowns.
      assert(InBounds(neighbors[i], foreground));
      if (unknown_pixels.find(neighbors[i]) == unknown_pixels.end()) {
        // If we made it here, we have a boundary condition pixel!
        boundary_condition.insert(neighbors[i]);
        boundary.SetPixelComponent(neighbors[i].x, neighbors[i].y, 0, 255);
      }
    }
  }
  // Print progress.
  std::cout << "Found " << unknown_pixels.size() << " unknown pixels, and "
            << boundary_condition.size() << " boundary condition pixels."
            << std::endl;

  std::vector< VectorXd > solution_vecs_per_chan;
  for (int chan = 0; chan < foreground.Channels(); ++chan) {
    std::cout << "Working on channel: " << chan << std::endl;

    // Setup the linear system.
    std::cout << "\tSetting up sparse linear system..." << std::endl;

    SparseMatrix<double> A(unknown_pixels.size(), unknown_pixels.size());
	A.reserve(5 * unknown_pixels.size());
    VectorXd b(unknown_pixels.size());
    b.fill(0.0);

    // Set the -4's along the main diagonal of A resulting from the disretization
    // of the Laplace operator.
    std::vector< Triplet<double> > sparse_mat_entries;
    for (std::set<Pixel>::const_iterator iter = unknown_pixels.begin();
        iter != unknown_pixels.end(); ++iter) {
      Pixel curr_unknown = *iter;
      assert(curr_unknown.index >= 0 && curr_unknown.index < unknown_pixels.size());

      // Add the -4.0 on the main diagonal.
      sparse_mat_entries.push_back(Triplet<double>(curr_unknown.index,
                                                  curr_unknown.index,
                                                  -4.0));

      // Add the rest of the stencil.
      const int in_bounds_neighbor_count =
          GetNeighbors(curr_unknown, foreground, &(neighbors[0]));
      assert(in_bounds_neighbor_count >= 0 && in_bounds_neighbor_count <= 4);

      // Loop over all the neighbors, and process boundary conditions differently.
      for (int i = 0; i < in_bounds_neighbor_count; ++i) {
        Pixel curr_neighbor = neighbors[i];

        if (unknown_pixels.find(curr_neighbor) != unknown_pixels.end()) {
          curr_neighbor = *unknown_pixels.find(curr_neighbor);  // We need the index!
          assert(curr_neighbor.index >= 0 && curr_neighbor.index < unknown_pixels.size());

          sparse_mat_entries.push_back(Triplet<double>(
              curr_unknown.index, curr_neighbor.index, 1.0));


        } else if (boundary_condition.find(curr_neighbor) != boundary_condition.end()) {
          b(curr_unknown.index) = b(curr_unknown.index) -
              bg_to_edit->GetPixelComponent(curr_neighbor.x,
                                            curr_neighbor.y,
                                            chan);

        } else {
          // This indicates an error!  Each neighbor should be a boundary condition or an
          // unknown!
          std::cerr << "\tLogic error - Could not find neighbor!" << std::endl;
          assert(false);
          return false;
        }
      }

      // Now finalize the RHS by adding in the divergence.
      b(curr_unknown.index) = b(curr_unknown.index) + Divergence(curr_unknown.x,
                                                                 curr_unknown.y,
                                                                 chan,
                                                                 foreground);


    }
    A.setFromTriplets(sparse_mat_entries.begin(), sparse_mat_entries.end());
    A.makeCompressed();

    std::cout << "\tDone creating linear system.  The lhs matrix A had "
              << A.nonZeros() << " non-zero entires, out of "
              << static_cast<long>(A.rows()) * static_cast<long>(A.cols())
              << " total entries.  This comes to "
              <<  (static_cast<double>(A.nonZeros()) /
                  static_cast<double>(static_cast<long>(A.rows()) *
                  static_cast<long>(A.cols())) * 100.0) << " percent "
              << "fill in." <<  std::endl;

    // Solve the system!!!
    std::cout << "\tSolving linear system..." << std::endl;
	Eigen::SimplicialCholesky< SparseMatrix<double> > solver(A);  // performs a Cholesky factorization of A.
	Eigen::VectorXd x = solver.solve(b); // use the factorization to solve for the given right hand side.

	//Eigen::SparseLU< Eigen::SparseMatrix<double> > solver;
	//solver.compute(A);
	//Eigen::VectorXd x = solver.solve(b); // use the factorization to solve for the given right hand side.
	if (solver.info() != Eigen::Success) {
      std::cerr << "Failed to solve linear system!" << std::endl;
      return false;
    }
    std::cout << "\tDone solving system." << std::endl;
    solution_vecs_per_chan.push_back(x);
  }

  // TODO(cbraley): Export the result to a file:
  //saveAsBitmap(x, n, argv[1]);

  // Fill out the answer!
  std::cout << "Updating output image..." << std::endl;
  unsigned char* color_out = new unsigned char[foreground.Channels()];
  int oor_count = 0;
  for (std::set<Pixel>::const_iterator iter = unknown_pixels.begin();
      iter != unknown_pixels.end(); ++iter) {
    Pixel curr = *unknown_pixels.find(*iter);  // We need the index!

    // Loop over all channels and set each one.
    for (int c = 0; c < foreground.Channels(); ++c) {
      double solved_val = solution_vecs_per_chan[c](curr.index);
      if (solved_val < 0.0 || solved_val > std::numeric_limits<unsigned char>::max()) {
        if (oor_count < 100) {
          std::cerr << "\tSolution value of f(" << curr.x << ", " << curr.y << ") = "
                    << solved_val << " is out of range!" << std::endl;
        } else if (oor_count == 100) {
          std::cerr << "\t(no longer printing any out of range messages)" << std::endl;
        }
        ++oor_count;
        solved_val = std::max<double>(std::min<double>(solved_val, std::numeric_limits<unsigned char>::max()), 0.0);
      }
      color_out[c] = static_cast<unsigned char>(solved_val);
    }
    bg_to_edit->SetPixel(curr.x, curr.y, color_out);
  }
  delete[] color_out;
  std::cout << "\t" << 
                (static_cast<double>(oor_count) / 
                static_cast<double>(unknown_pixels.size() * foreground.Channels())) * 100.0
            << " percent of the pixels were out of range." << std::endl;
  std::cout << "Done updating output image." << std::endl;

  if (!image_io::WriteRGBLDR(boundary, "boundary.png")) {
    std::cerr << "Could not write output to boundary.png." << std::endl;
    return false;
  }

  return true;
}
