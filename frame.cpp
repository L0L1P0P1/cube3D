#include "cube3D.h"
#include <limits>
#include <vector>

namespace cube3D {

Frame::DepthBuffer::DepthBuffer(int height, int width, float render_distance)
    : height(height), width(width), render_distance(render_distance),
      buffer(height * width) {
  for (auto it = buffer.begin(); it < buffer.end(); ++it)
    *it = render_distance;
};

Frame::Frame(int height, int width, float z_level, float focal_length)
    : height(height), width(width), z_level(z_level),
      focal_length(focal_length), pixel_grid(height * width),
      depth_buffer(height, width, -std::numeric_limits<float>::infinity()) {}

void Frame::show() {
  std::cout << "\033[3J\n";
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      // probably need a "shading" type shi function for here
      if (this->get_pixel(i, j))
        std::cout << "..";
      else
        std::cout << "  ";

      depth_buffer(i, j) = depth_buffer.render_distance;
      this->get_pixel(i, j) = 0;
    }
    std::cout << "\n";
  }
};

} // namespace cube3D
