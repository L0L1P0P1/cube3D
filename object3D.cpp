#include "cube3D.h"
#include <cmath>
#include <vector>

namespace cube3D {

Orientation::Orientation(double alpha, double beta, double gamma)
  : alpha(alpha)
  , beta(beta)
  , gamma(gamma)
{
}

Graph3D::Graph3D(std::vector<Point> vertices, std::vector<Edge> edges)
  : vertices(vertices)
  , edges(edges)
{
}
Point
Graph3D::apply_orientation(cube3D::Point& point)
{
  return point.rotate_x(orient.alpha)
           .rotate_y(orient.beta)
           .rotate_z(orient.gamma) +
         center;
}
void
Graph3D::draw_edges(cube3D::Frame& frame, float dt)
{
  for (auto e : edges) {
    Point p = apply_orientation(vertices[e.from]);
    Point q = apply_orientation(vertices[e.to]);
    for (float t = 0; t <= 1; t += dt) {
      Point edge = p * t + q * (1 - t);
      int edge_x =
        std::round(frame.focal_length * (edge.x / (edge.z - frame.z_level)) +
                   (float)frame.height / 2);
      int edge_y =
        std::round(frame.focal_length * (edge.y / (edge.z - frame.z_level)) +
                   (float)frame.width / 2);
      if (edge_x < frame.height && edge_x >= 0 && edge_y < frame.width &&
          edge_y >= 0 && frame.z_level > edge.z &&
          edge.z > frame.get_z(edge_x, edge_y)) {
        frame(edge_x, edge_y) = 1;
        frame.get_z(edge_x, edge_y) = edge.z;
      }
    }
  }
}
void
Graph3D::draw_vertices(cube3D::Frame& frame)
{
  for (auto v : vertices) {
    Point p = apply_orientation(v);
    int p_x = std::round(frame.focal_length * (p.x / (p.z - frame.z_level)) +
                         (float)frame.height / 2);
    int p_y = std::round(frame.focal_length * (p.y / (p.z - frame.z_level)) +
                         (float)frame.width / 2);
    if (p_x < frame.height && p_x >= 0 && p_y < frame.width && p_y >= 0 &&
        frame.z_level > p.z && p.z > frame.get_z(p_x, p_y)) {
      frame(p_x, p_y) = 1;
      frame.get_z(p_x, p_y);
    }
  }
};

Mesh3D::Mesh3D(std::vector<Point> vertices,
               std::vector<Triangle> triangles,
               double dt)
  : vertices(vertices)
  , triangles(triangles)
{
}
Point
Mesh3D::apply_orientation(Point& point)
{
  return point.rotate_x(orient.alpha)
           .rotate_y(orient.beta)
           .rotate_z(orient.gamma) +
         center;
}
void
Mesh3D::draw_triangles(Frame& frame, float ds)
{
  for (auto t : triangles) {
    Point p1 = apply_orientation(vertices[t.p1]);
    Point p2 = apply_orientation(vertices[t.p2]);
    Point p3 = apply_orientation(vertices[t.p3]);
    // using barycentric coordinates a*p1+b*p2+c*p3 = p
    // such that a+b+c=1 and a,b,c >= 0
    for (float a = ds; a < 1 - ds; a += ds)
      for (float b = ds; b < 1 - ds; b += ds) {
        float c = 1 - a - b;
        Point p = p1 * a + p2 * b + p3 * c;
        int p_x =
          std::round(frame.focal_length * (p.x / (p.z - frame.z_level)) +
                     (float)frame.height / 2);
        int p_y =
          std::round(frame.focal_length * (p.y / (p.z - frame.z_level)) +
                     (float)frame.width / 2);
        if (p_x < frame.height && p_x >= 0 && p_y < frame.width && p_y >= 0 &&
            frame.z_level > p.z && p.z > frame.get_z(p_x, p_y)) {
          frame(p_x, p_y) = 1;
          frame.get_z(p_x, p_y);
        }
      }
  }
}

}
