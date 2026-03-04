#include <cmath>
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

class Frame
{
public:
  const int FRAME_X;
  const int FRAME_Y;
  const double z;
  const double sigma;

  std::vector<std::vector<int>> frame;

  Frame(int FRAME_X, int FRAME_Y, double z, double sigma)
    : FRAME_X(FRAME_X)
    , FRAME_Y(FRAME_Y)
    , z(z)
    , sigma(sigma)
  {
    for (int i = 0; i < FRAME_X; i++) {
      std::vector<int> row;
      for (int j = 0; j < FRAME_Y; j++)
        row.push_back(0);

      frame.push_back(row);
    }
  }
};

class Object3D
{
public:
  const double dt;
  std::vector<Point> vertices;
  std::vector<std::vector<int>> edges;
  Point center = Point(0, 0, 0);
  Orientation orient = Orientation(0, 0, 0);

  Object3D(std::vector<Point> vertices,
           std::vector<std::vector<int>> edges,
           double dt)
    : vertices(vertices)
    , edges(edges)
    , dt(dt)
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

  Point apply_orientation(Point& point)
  {
    return point.rotate_x(orient.alpha)
             .rotate_y(orient.beta)
             .rotate_z(orient.gamma) +
           center;
  }

  void draw_edges(Frame frame, double dt)
  {
    for (auto e : edges) {
      Point p = apply_orientation(vertices[e[0]]);
      Point q = apply_orientation(vertices[e[1]]);
      for (float t = 0; t <= 1; t += dt) {
        Point edge = p * t + q * (1 - t);
        int edge_x = std::round(frame.sigma * (edge.x / edge.z));
        int edge_y = std::round(frame.sigma * (edge.y / edge.z));
        if (edge_x < frame.FRAME_X && edge_y < frame.FRAME_Y) {
          if (frame.frame[edge_x][edge_y] < 3)
            // shaders come here
            frame.frame[edge_x][edge_y] = 2;
        }
      }
    }
  };

  void draw_vertices(Frame frame)
  {
    for (auto v : vertices) {
      Point p = apply_orientation(v);
      int p_x = std::round(frame.sigma * (p.x / p.z));
      int p_y = std::round(frame.sigma * (p.y / p.z));
      if (p_x < frame.FRAME_X && p_y < frame.FRAME_Y) {
        if (frame.frame[p_x][p_y] < 6)
          frame.frame[p_x][p_y] = 4;
      }
    }
  }
};
