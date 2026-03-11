#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace cube3D {

struct Vector2D {};

struct Vector3D {
  double x;
  double y;
  double z;
  Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

  inline Vector3D rotate_x(double alpha) {
    return Vector3D(x, y * std::cos(alpha) - z * std::sin(alpha),
                    y * std::sin(alpha) + z * std::cos(alpha));
  }

  inline Vector3D rotate_y(double beta) {
    return Vector3D(x * std::cos(beta) + z * std::sin(beta), y,
                    -x * std::sin(beta) + z * std::cos(beta));
  }

  inline Vector3D rotate_z(double gamma) {
    return Vector3D(x * std::cos(gamma) - y * std::sin(gamma),
                    x * std::sin(gamma) + y * std::cos(gamma), z);
  }

  inline Vector3D operator+(Vector3D rhs) {
    return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
  }

  inline Vector3D operator-(Vector3D rhs) {
    return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  inline Vector3D operator*(float scalar) {
    return Vector3D(x * scalar, y * scalar, z * scalar);
  }
};

struct Edge {
  const int from, to;
};

struct Triangle {
  const int p1, p2, p3;
};

struct Orientation {
  double alpha, beta, gamma;
  Orientation(double, double, double);
  inline Orientation operator+(Orientation rhs) {
    return Orientation(alpha + rhs.alpha, beta + rhs.beta, gamma + rhs.gamma);
  }
  inline Orientation operator-(Orientation rhs) {
    return Orientation(alpha - rhs.alpha, beta - rhs.beta, gamma - rhs.gamma);
  }
  inline Orientation operator+=(Orientation rhs) { return *this + rhs; }
};

class Frame {
public:
  const int height;
  const int width;
  float z_level;
  float focal_length;
  float render_distance;

  std::vector<int> pixel_grid;
  std::vector<float> z_buffer;

  Frame(int, int, float, float);

  inline int &get_pixel(int x, int y) { return pixel_grid[(x * width) + y]; }
  inline float &get_z(int x, int y) { return z_buffer[(x * width) + y]; }
  inline int &operator()(int x, int y) { return this->get_pixel(x, y); }

  void show();
};

class Graph3D {
public:
  std::vector<Vector3D> vertices;
  std::vector<Edge> edges;
  Vector3D center = Vector3D(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  Graph3D(std::vector<Vector3D>, std::vector<Edge>);
  Vector3D apply_orientation(Vector3D &);
  void draw_edges(Frame &, float);
  void draw_vertices(Frame &);
};

class Mesh3D {
public:
  std::vector<Vector3D> vertices;
  std::vector<Triangle> triangles;
  Vector3D center = Vector3D(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  Mesh3D(std::vector<Vector3D>, std::vector<Triangle>);
  Vector3D apply_orientation(Vector3D &);
  void draw_triangles(Frame &, float);
};

namespace parser {
Graph3D parse_to_graph3D(std::string);
Mesh3D parse_to_mesh3D(std::string);
} // namespace parser

namespace presets {
void draw_penger();
void draw_cube();
void draw_tux();
void draw_sample_shape();
} // namespace presets
} // namespace cube3D
