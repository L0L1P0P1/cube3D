#include <cmath>
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
    return Point(x,
                 y * std::cos(alpha) - z * std::sin(alpha),
                 y * std::sin(alpha) + z * std::cos(alpha));
  }

  Point rotate_y(double beta)
  {
    return Point(x * std::cos(beta) + z * std::sin(beta),
                 y,
                 -x * std::sin(beta) + z * std::cos(beta));
  }

  Point rotate_z(double gamma)
  {
    return Point(x * std::cos(gamma) - y * std::sin(gamma),
                 x * std::sin(gamma) + y * std::cos(gamma),
                 z);
  }

  Point operator+(Point rhs) { return Point(x + rhs.x, y + rhs.y, z + rhs.z); }
  Point operator-(Point rhs) { return Point(x - rhs.x, y - rhs.y, z - rhs.z); }
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

  Orientation operator+(Orientation rhs)
  {
    return Orientation(alpha + rhs.alpha, beta + rhs.beta, gamma + rhs.gamma);
  }
  Orientation operator-(Orientation rhs)
  {
    return Orientation(alpha - rhs.alpha, beta - rhs.beta, gamma - rhs.gamma);
  }

  Orientation operator+=(Orientation rhs) { return *this + rhs; }
};

class Object3D
{
public:
  std::vector<Point> vertices;
  // std::vector<int[2]> edges;
  Point center = Point(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  // Object3D(std::vector<Point> vertices, std::vector<int[2]> edges)
  //   : vertices(vertices)
  //   , edges(edges)
  // {
  // }

  Object3D(std::vector<Point> vertices)
    : vertices(vertices)
  {
  }

  std::vector<Point> get_vertices()
  {
    std::vector<Point> new_vertices;
    for (auto v : vertices) {
      new_vertices.push_back(
        v.rotate_x(orient.alpha).rotate_y(orient.beta).rotate_z(orient.gamma) +
        center);
    }

    return new_vertices;
  }
};

int
is_drawn(int i, int j, std::vector<Point> vertices, double z)
{
  for (auto v : vertices) {
    if (std::round(v.x / (z)) == i && std::round(v.y / (z)) == j) {
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
main()
{
  using namespace std::chrono;
  int fps = 120;
  int frame_time = 1000 / fps;
  double speed = 0.5;

  std::vector<Point> vertices = {
    Point(4, 4, -4), Point(4, -4, -4), Point(-4, 4, -4), Point(-4, -4, -4),
    Point(4, 4, 4),  Point(4, -4, 4),  Point(-4, 4, 4),  Point(-4, -4, 4),
  };

  Object3D cube(vertices);

  std::cout << "\033[2J" << std::endl;
  double theta = 0;

  std::vector<Point> new_vertices = cube.get_vertices();
  while (true) {
    int term_x = 48, term_y = 36;

    for (int i = 0; i < term_y; i++) {
      for (int j = 0; j < term_x; j++) {
        int drawn = is_drawn(j - term_x / 2,
                             i - term_y / 2,
                             new_vertices,
                             0.8 + 0.2 * std::sin(theta));
        switch (drawn) {
          case 3:
            std::cout << "..";
            break;
          case 2:
            std::cout << "**";
            break;
          case 1:
            std::cout << "@@";
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
    std::this_thread::sleep_for(frame_time * 1ms);
    new_vertices = cube.get_vertices();
    theta += 0.01;
    std::cout << "\033[3J" << std::endl;
  }
}
