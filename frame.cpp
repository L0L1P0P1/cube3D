#include "cube3D.h"
#include <limits>
#include <vector>

namespace cube3D {

Frame::Frame(int height, int width, float z_level, float focal_length)
  : height(height)
  , width(width)
  , z_level(z_level)
  , focal_length(focal_length)
{
  render_distance = -std::numeric_limits<float>::infinity();
  pixel_grid.reserve(height * width);
  z_buffer.reserve(height * width);
  for (auto it = z_buffer.begin(); it < z_buffer.end(); ++it) {
    *it = render_distance;
  }
}

void
Frame::show()
{
  std::cout << "\033[3J\n";
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      // probably need a "shading" type shi function for here
      if (this->get_pixel(i, j))
        std::cout << "..";
      else
        std::cout << "  ";

      this->get_z(i, j) = render_distance;
      this->get_pixel(i, j) = 0;
    }
    std::cout << "\n";
  }
};

}
