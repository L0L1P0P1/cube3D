#include "cube3D.h"
#include <ctime>
#include <thread>

void
cube3D::presets::draw_penger()
{
  const int height = 200;
  const int width = 400;
  const int fps = 100;
  const double speed = 0.1;
  const float dt = 0.005;
  const float z_level = 5;
  const float dz = 0;
  const float focal_length = 750;
  const float dtheta = 0.005;
  cube3D::Orientation orient(0, 0, -1.5707963);
  cube3D::Point center(-0.6, 0, 0);

  cube3D::Graph3D penger = cube3D::parser::parse_to_graph3D("obj/penger.obj");
  penger.orient = orient;
  penger.center = center;
  cube3D::Frame frame(height, width, z_level, focal_length);

  using namespace std::chrono;

  double theta = 0;

  while (true) {

    // draw and show
    penger.draw_edges(frame, dt);
    penger.draw_vertices(frame);
    frame.show();

    // animate next frames
    penger.orient = penger.orient + cube3D::Orientation(
                                      0.00 * speed, 0.05 * speed, 0.0 * speed);
    theta += dtheta;

    // sleep to have stuff visible
    std::this_thread::sleep_for((1000 / fps) * 1ms);
  }
}

void
cube3D::presets::draw_cube()
{

  const int height = 90;
  const int width = 180;
  const int fps = 100;
  const double speed = 0.8;
  const float dt = 0.01;
  const float z_level = 12;
  const float dz = 5;
  const float focal_length = 25;
  const float dtheta = 0.005;

  using namespace std::chrono;
  using namespace cube3D;

  std::vector<Point> vertices = {
    Point(4, 4, -4), Point(4, -4, -4), Point(-4, 4, -4), Point(-4, -4, -4),
    Point(4, 4, 4),  Point(4, -4, 4),  Point(-4, 4, 4),  Point(-4, -4, 4),
  };

  std::vector<Edge> edges = { { 0, 1 }, { 0, 4 }, { 0, 2 }, { 3, 7 },
                              { 3, 2 }, { 3, 1 }, { 6, 7 }, { 6, 2 },
                              { 6, 4 }, { 5, 4 }, { 5, 1 }, { 5, 7 } };

  Graph3D cube(vertices, edges);
  Frame frame(height, width, z_level, focal_length);

  double theta = 0;

  while (true) {

    // draw and show
    cube.draw_edges(frame, dt);
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
