#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace cube3D {

struct Vector2D {
  float x;
  float y;
  Vector2D(float x, float y) : x(x), y(y) {}

  inline Vector2D operator*(float rhs) { return Vector2D(x * rhs, y * rhs); }
  inline Vector2D operator+(Vector2D rhs) {
    return Vector2D(x + rhs.x, y + rhs.y);
  }
  inline Vector2D operator-(Vector2D rhs) { return (*this) + rhs * -1; }
};

inline float cross(Vector2D l, Vector2D r) { return l.x * r.y - l.y * r.x; }

struct Vector3D {
  float x;
  float y;
  float z;
  Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

  inline Vector3D rotate_x(float alpha) {
    return Vector3D(x, y * std::cos(alpha) - z * std::sin(alpha),
                    y * std::sin(alpha) + z * std::cos(alpha));
  }

  inline Vector3D rotate_y(float beta) {
    return Vector3D(x * std::cos(beta) + z * std::sin(beta), y,
                    -x * std::sin(beta) + z * std::cos(beta));
  }

  inline Vector3D rotate_z(float gamma) {
    return Vector3D(x * std::cos(gamma) - y * std::sin(gamma),
                    x * std::sin(gamma) + y * std::cos(gamma), z);
  }

  inline Vector2D perspective_project(float z_level, float focal_length) {
    return Vector2D(x / (z - z_level), y / (z - z_level)) * focal_length;
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

class Frame {
public:
  int height;
  int width;
  float z_level;
  float focal_length;

  struct DepthBuffer {
    std::vector<float> buffer;
    int height;
    int width;
    float render_distance;

    DepthBuffer(int, int, float);
    // you can change this to constexpr
    inline float &get_z(int x, int y) { return buffer[(x * width) + y]; }
    inline float &operator()(int x, int y) { return this->get_z(x, y); }
  };

  std::vector<int> pixel_grid;
  DepthBuffer depth_buffer;

  Frame(int, int, float, float);

  inline int &get_pixel(int x, int y) { return pixel_grid[(x * width) + y]; }
  inline int &operator()(int x, int y) { return this->get_pixel(x, y); }

  inline Vector2D offset() { return Vector2D(height / 2, width / 2); };

  void show();
};

struct Edge {
  const int from, to;
};

struct Orientation {
  float alpha, beta, gamma;
  Orientation(float, float, float);
  inline Orientation operator+(Orientation rhs) {
    return Orientation(alpha + rhs.alpha, beta + rhs.beta, gamma + rhs.gamma);
  }
  inline Orientation operator-(Orientation rhs) {
    return Orientation(alpha - rhs.alpha, beta - rhs.beta, gamma - rhs.gamma);
  }
  inline Orientation operator+=(Orientation rhs) { return *this + rhs; }
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

struct Triangle {
  const int p0, p1, p2;
  void rasterize(Frame &, std::vector<Vector3D> &);
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
