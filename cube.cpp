#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

class Point
{
public:
  double x;
  double y;
  double z;
  Point(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
  {
  }

  Point rotate_x(double alpha)
  {
    // double& x = this->x;
    // double& y = this->y;
    // double& z = this->z;

    return Point(x,
                 y * std::cos(alpha) - z * std::sin(alpha),
                 y * std::sin(alpha) + z * std::cos(alpha));
  }

  Point rotate_y(double beta)
  {
    // double& x = this->x;
    // double& y = this->y;
    // double& z = this->z;
    //
    return Point(x * std::cos(beta) + z * std::sin(beta),
                 y,
                 -x * std::sin(beta) + z * std::cos(beta));
  }

  Point rotate_z(double gamma)
  {
    // double& x = this->x;
    // double& y = this->y;
    // double& z = this->z;

    return Point(x * std::cos(gamma) - y * std::sin(gamma),
                 x * std::sin(gamma) + y * std::cos(gamma),
                 z);
  }

  Point operator+(Point& rhs) { return Point(x + rhs.x, y + rhs.y, z + rhs.z); }
  Point operator-(Point& rhs) { return Point(x - rhs.x, y - rhs.y, z - rhs.z); }
};

struct Orientation
{
  double alpha;
  double beta;
  double gamma;

  Orientation(double alpha, double beta, double gamma)
    : alpha(alpha)
    , beta(beta)
    , gamma(gamma)
  {
  }
};

class Object3D
{
public:
  std::vector<Point> vertices;
  std::vector<int[2]> edges;
  Point center = Point(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  Object3D(std::vector<Point> vertices, std::vector<int[2]> edges)
    : vertices(vertices)
    , edges(edges)
  {
  }

  std::vector<Point> get_vertices()
  {
    std::vector<Point> new_vertices;
    for (auto v : this->vertices) {
      new_vertices.push_back(
        v.rotate_x(orient.alpha).rotate_y(orient.beta).rotate_z(orient.gamma) +
        center);
    }
  }
};

// void
// rotate_vertices(std::vector<Point>& vertices, double da, double db, double
// dg)
// {
//   for (int i = 0; i < vertices.size(); i++) {
//     rotate_x(vertices[i], da);
//     rotate_y(vertices[i], db);
//     rotate_z(vertices[i], dg);
//   }
// }

int
is_drawn(int i, int j, std::vector<Point>& vertices, double z)
{
  for (auto v : vertices) {
    if (std::round(v.x / (z)) == i && std::round(v.y / (z)) == j) {
      if (v.z <= -2)
        return 3;
      else if (v.z >= 2)
        return 1;
      else
        return 2;
    }
  }
  return 0;
}

int
main()
{
  using namespace std::chrono;
  int fps = 90;
  int frame_time = 1000 / fps;
  double speed = 1.5;

  std::vector<Point> vertices = {
    Point(4, 4, -4), Point(4, -4, -4), Point(-4, 4, -4), Point(-4, -4, -4),
    Point(4, 4, 4),  Point(4, -4, 4),  Point(-4, 4, 4),  Point(-4, -4, 4),
  };

  std::cout << "\033[2J" << std::endl;
  double z = 0;
  while (true) {
    int term_x = 64, term_y = 32;

    for (int i = 0; i < term_y; i++) {
      for (int j = 0; j < term_x; j++) {
        int drawn = is_drawn(
          j - term_x / 2, i - term_y / 2, vertices, 0.7 + 0.15 * std::sin(z));
        switch (drawn) {
          case 3:
            std::cout << "  ";
            break;
          case 2:
            std::cout << "--";
            break;
          case 1:
            std::cout << "++";
            break;
          case 0:
            std::cout << "$$";
            break;
        }
      }
      std::cout << "\n";
    }
    rotate_vertices(vertices, 0.001 * speed, 0.01 * speed, 0.01 * speed);
    std::this_thread::sleep_for(frame_time * 1ms);
    std::cout << "\033[3J" << std::endl;
    z += 0.015;
  }
}
