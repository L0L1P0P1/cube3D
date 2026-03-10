#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace cube3D {

struct Point
{
  double x;
  double y;
  double z;
  Point(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
  {
  }

  inline Point rotate_x(double alpha)
  {
    return Point(x,
                 y * std::cos(alpha) - z * std::sin(alpha),
                 y * std::sin(alpha) + z * std::cos(alpha));
  }

  inline Point rotate_y(double beta)
  {
    return Point(x * std::cos(beta) + z * std::sin(beta),
                 y,
                 -x * std::sin(beta) + z * std::cos(beta));
  }

  inline Point rotate_z(double gamma)
  {
    return Point(x * std::cos(gamma) - y * std::sin(gamma),
                 x * std::sin(gamma) + y * std::cos(gamma),
                 z);
  }

  inline Point operator+(Point rhs)
  {
    return Point(x + rhs.x, y + rhs.y, z + rhs.z);
  }

  inline Point operator-(Point rhs)
  {
    return Point(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  inline Point operator*(float scalar)
  {
    return Point(x * scalar, y * scalar, z * scalar);
  }
};

struct Edge
{
  int from, to;
};

struct Triangle
{
  int p1, p2, p3;
};

struct Orientation
{
  double alpha, beta, gamma;
  Orientation(double, double, double);
  inline Orientation operator+(Orientation rhs)
  {
    return Orientation(alpha + rhs.alpha, beta + rhs.beta, gamma + rhs.gamma);
  }
  inline Orientation operator-(Orientation rhs)
  {
    return Orientation(alpha - rhs.alpha, beta - rhs.beta, gamma - rhs.gamma);
  }
  inline Orientation operator+=(Orientation rhs) { return *this + rhs; }
};

class Frame
{
public:
  const int height;
  const int width;
  float z_level;
  float focal_length;
  float render_distance;

  std::vector<int> pixel_grid;
  std::vector<float> z_buffer;

  Frame(int, int, float, float);

  inline int& get_pixel(int x, int y) { return pixel_grid[(x * width) + y]; }
  inline float& get_z(int x, int y) { return z_buffer[(x * width) + y]; }
  inline int& operator()(int x, int y) { return this->get_pixel(x, y); }

  void show();
};

class Graph3D
{
public:
  std::vector<Point> vertices;
  std::vector<Edge> edges;
  Point center = Point(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  Graph3D(std::vector<Point>, std::vector<Edge>);
  Point apply_orientation(Point&);
  void draw_edges(Frame&, float);
  void draw_vertices(Frame&);
};

class Mesh3D
{
public:
  std::vector<Point> vertices;
  std::vector<Triangle> triangles;
  Point center = Point(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  Mesh3D(std::vector<Point>, std::vector<Triangle>, double);
  Point apply_orientation(Point&);
};

namespace parser {
Graph3D parse_to_graph3D(std::string);
}

namespace presets {
void
draw_penger();
void
draw_cube();
}
}
