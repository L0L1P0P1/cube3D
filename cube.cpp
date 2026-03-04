#include "cube.h"
#include <ctime>
#include <iostream>
#include <thread>

const int FRAME_X = 200;
const int FRAME_Y = 400;
const int fps = 100;
const double speed = 1;
const float dt = 0.01;
const float z = 12;
const float dz = 4;
const float sigma = 60;
const float dtheta = 0.005;

int
is_drawn(int i, int j, std::vector<Point> vertices, double z, double sigma)
{
  for (auto v : vertices) {
    if (std::round(sigma * v.x / (v.z - z)) == i &&
        std::round(sigma * v.y / (v.z - z)) == j) {
      if (v.z <= -2)
        return 1;
      else if (v.z >= 2)
        return 3;
      else
        return 2;
    }
  }
  return 0;
}

int
is_drawn(int i,
         int j,
         std::vector<Point> vertices,
         std::vector<Point> edges,
         double z,
         double sigma)
{
  for (auto v : vertices) {
    if (std::round(sigma * (v.x / (v.z - z))) == i &&
        std::round(sigma * (v.y / (v.z - z))) == j) {
      if (v.z >= 0)
        return 5;
      else if (v.z <= -6)
        return 3;
      else
        return 4;
    }
  }
  for (auto e : edges) {
    if (std::round(sigma * (e.x / (e.z - z))) == i &&
        std::round(sigma * (e.y / (e.z - z))) == j) {
      if (e.z <= -8)
        return 1;
      return 2;
    }
  }
  return 0;
}

std::vector<Point>
interpolate_vertices(std::vector<Point> vertices,
                     std::vector<std::vector<int>> edges,
                     float dt)
{
  std::vector<Point> edge_vertices;
  for (auto e : edges) {
    for (float t = 0; t < 1; t += dt) {
      if (t == 0)
        continue;
      else
        edge_vertices.push_back(vertices[e[0]] * t + vertices[e[1]] * (1 - t));
    }
  }
  return edge_vertices;
}

std::vector<std::vector<int>>
initiate_frame(int x, int y)
{
  std::vector<std::vector<int>> frame;
  for (int i = 0; i < x; i++) {
    std::vector<int> row;
    for (int j = 0; j < y; j++) {
      row.push_back(0);
    }
    frame.push_back(row);
  }
  return frame;
}

void
show_frame(Frame& frame)
{
  std::cout << "\033[3J\n";
  for (auto i = frame.frame.begin(); i < frame.frame.end(); ++i) {
    for (auto j = i->begin(); j < i->end(); ++j) {
      switch (*j) {
        case 5:
          std::cout << "@@";
          break;
        case 4:
          std::cout << "oo";
          break;
        case 3:
          std::cout << ". ";
          break;
        case 2:
          std::cout << ". ";
          break;
        case 1:
          std::cout << "  ";
          break;
        case 0:
          std::cout << "  ";
          break;
      }
      *j = 0;
    }
    std::cout << "\n";
  }
};

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
  Frame frame(FRAME_X, FRAME_Y, z, sigma);

  double theta = 0;

  while (true) {

    cube.draw_edges(frame);
    cube.draw_vertices(frame);
    show_frame(frame);

    // break;
    cube.orient =
      cube.orient + Orientation(0.001 * speed, 0.05 * speed, 0.01 * speed);
    cube.center = Point(0, 0, dz * std::sin(theta));
    theta += dtheta;
    std::this_thread::sleep_for((1000 / fps) * 1ms);
  }
}
