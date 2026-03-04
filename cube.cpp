#include "cube.h"
#include <ctime>
#include <iostream>
#include <thread>

const int FRAME_X = 32;
const int FRAME_Y = 32;
const int fps = 60;
const double speed = 1.25;
const float dt = 0.05;
const float z = 6;
const float dz = 1.1;
const float sigma = 20;
const float dtheta = 0.02;

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
        return 3;
      else if (v.z <= -6)
        return 1;
      else
        return 2;
    }
  }
  for (auto e : edges) {
    if (std::round(sigma * (e.x / (e.z - z))) == i &&
        std::round(sigma * (e.y / (e.z - z))) == j) {
      if (e.z <= -8)
        return -2;
      return -1;
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
  }
  return frame;
}

void
draw_frame(std::vector<std::vector<int>> frame)
{
  std::cout << "\033[2J\n";
  for (auto i : frame) {
    for (auto j = i.begin(); j < i.end(); ++j) {
      switch (*j) {
        case 3:
          std::cout << "@@";
          break;
        case 2:
          std::cout << "oo";
          break;
        case 1:
          std::cout << ". ";
          break;
        case -1:
          std::cout << ". ";
          break;
        case -2:
          std::cout << "  ";
          break;
        case 0:
          std::cout << "  ";
          break;
      }
      *j = 0;
    }
  }
};

int
main()
{
  using namespace std::chrono;

  int term_x = 32, term_y = 32;
  Point c1 = Point(0, 0, 0);

  std::vector<Point> vertices = {
    Point(4, 4, -4), Point(4, -4, -4), Point(-4, 4, -4), Point(-4, -4, -4),
    Point(4, 4, 4),  Point(4, -4, 4),  Point(-4, 4, 4),  Point(-4, -4, 4),
  };

  std::vector<std::vector<int>> edges = { { 0, 1 }, { 0, 4 }, { 0, 2 },
                                          { 3, 7 }, { 3, 2 }, { 3, 1 },
                                          { 6, 7 }, { 6, 2 }, { 6, 4 },
                                          { 5, 4 }, { 5, 1 }, { 5, 7 } };

  Object3D cube(vertices);
  cube.center = c1;

  double theta = 0;
  std::vector<Point> new_vertices = cube.get_vertices();
  std::vector<Point> new_edges = interpolate_vertices(new_vertices, edges, dt);

  while (true) {

    std::cout << "\033[2J\n\n\n";
    for (int i = 0; i < term_y; i++) {
      for (int j = 0; j < term_x; j++) {
        int vertices_drawn = is_drawn(
          j - term_x / 2, i - term_y / 2, new_vertices, new_edges, z, sigma);
        switch (vertices_drawn) {
          case 3:
            std::cout << "@@";
            break;
          case 2:
            std::cout << "oo";
            break;
          case 1:
            std::cout << ". ";
            break;
          case -1:
            std::cout << ". ";
            break;
          case -2:
            std::cout << "  ";
            break;
          case 0:
            std::cout << "  ";
            break;
        }
      }
      std::cout << "\n";
    }
    cube.orient =
      cube.orient + Orientation(0.001 * speed, 0.05 * speed, 0.01 * speed);
    cube.center = Point(0, 0, -z + dz * std::sin(theta));
    theta += dtheta;
    new_vertices = cube.get_vertices();
    new_edges = interpolate_vertices(new_vertices, edges, dt);
    std::this_thread::sleep_for((1000 / fps) * 1ms);
  }
}
