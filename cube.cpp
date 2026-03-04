#include "object3D.h"
#include <ctime>
#include <thread>

const int FRAME_X = 90;
const int FRAME_Y = 180;
const int fps = 100;
const double speed = 0.8;
const float dt = 0.01;
const float z_level = 12;
const float dz = 5;
const float scale = 25;
const float dtheta = 0.005;

int
main()
{
  using namespace std::chrono;

  std::vector<Point> vertices = {
    Point(4, 4, -4), Point(4, -4, -4), Point(-4, 4, -4), Point(-4, -4, -4),
    Point(4, 4, 4),  Point(4, -4, 4),  Point(-4, 4, 4),  Point(-4, -4, 4),
  };

  std::vector<std::vector<int>> edges = { { 0, 1 }, { 0, 4 }, { 0, 2 },
                                          { 3, 7 }, { 3, 2 }, { 3, 1 },
                                          { 6, 7 }, { 6, 2 }, { 6, 4 },
                                          { 5, 4 }, { 5, 1 }, { 5, 7 } };

  Object3D cube(vertices, edges, dt);
  Frame frame(FRAME_X, FRAME_Y, z_level, scale);

  double theta = 0;

  while (true) {

    // draw and show
    cube.draw_edges(frame);
    cube.draw_vertices(frame);
    frame.show();

    // animate next frames
    cube.orient =
      cube.orient + Orientation(0.001 * speed, 0.05 * speed, 0.01 * speed);
    cube.center = Point(0, 0, dz * std::sin(theta));
    theta += dtheta;

    // sleep to have stuff visible
    std::this_thread::sleep_for((1000 / fps) * 1ms);
  }
}
