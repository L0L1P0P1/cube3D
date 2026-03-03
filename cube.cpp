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
  Point operator*(float scalar)
  {
    return Point(x * scalar, y * scalar, z * scalar);
  }
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
  Point center = Point(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  // Object3D(std::vector<Point> vertices, int edges[][2])
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
  int edges[][2];
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
is_drawn(int i,
         int j,
         std::vector<Point> vertices,
         std::vector<Point> edges,
         double z)
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
  for (auto e : edges) {
    if (std::round(e.x / z) == i && std::round(e.y / z) == j) {
      if (e.z >= 3)
        return -1;
      return -2;
    }
  }
  return 0;
}

std::vector<Point>
interpolate_vertices(std::vector<Point> vertices, int edges[][2], float dt)
{
  std::vector<Point> edge_vertices;
  for (int i = 0; i < 12; i++) {
    for (float t = 0; t < 1; t += dt) {
      if (t == 0)
        continue;
      else
        edge_vertices.push_back(vertices[edges[i][0]] * t +
                                vertices[edges[i][1]] * (1 - t));
    }
  }
  return edge_vertices;
}

int
main()
{
  using namespace std::chrono;
  int fps = 60;
  int frame_time = 1000 / fps;
  double speed = 1.5;
  float dt = 0.05;
  float z = 0.5;
  float dz = 0.05;
  float dtheta = 0.02;
  int term_x = 32, term_y = 32;

  std::vector<Point> vertices = {
    Point(4, 4, -4), Point(4, -4, -4), Point(-4, 4, -4), Point(-4, -4, -4),
    Point(4, 4, 4),  Point(4, -4, 4),  Point(-4, 4, 4),  Point(-4, -4, 4),
  };

  int edges[][2] = {
    { 0, 1 }, { 0, 4 }, { 0, 2 }, { 3, 7 }, { 3, 2 }, { 3, 1 },
    { 6, 7 }, { 6, 2 }, { 6, 4 }, { 5, 4 }, { 5, 1 }, { 5, 7 }
  };

  Object3D cube(vertices);

  std::cout << "\033[3J" << std::endl;
  double theta = 0;

  std::vector<Point> new_vertices = cube.get_vertices();
  std::vector<Point> new_edges = interpolate_vertices(new_vertices, edges, dt);
  while (true) {

    std::cout << "\033[2J\n";
    for (int i = 0; i < term_y; i++) {
      for (int j = 0; j < term_x; j++) {
        int vertices_drawn = is_drawn(j - term_x / 2,
                                      i - term_y / 2,
                                      new_vertices,
                                      new_edges,
                                      z + dz * std::sin(theta));
        switch (vertices_drawn) {
          case 3:
            std::cout << "..";
            break;
          case 2:
            std::cout << "oo";
            break;
          case 1:
            std::cout << "@@";
            break;
          case -1:
            std::cout << "  ";
            break;
          case -2:
            std::cout << ". ";
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
    theta += dtheta;
    new_vertices = cube.get_vertices();
    new_edges = interpolate_vertices(new_vertices, edges, dt);
    std::this_thread::sleep_for(frame_time * 1ms);
  }
}
