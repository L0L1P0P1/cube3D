#include "cube3D.h"
#include <cmath>
#include <vector>

namespace cube3D {

Orientation::Orientation(double alpha, double beta, double gamma)
    : alpha(alpha), beta(beta), gamma(gamma) {}

Graph3D::Graph3D(std::vector<Vector3D> vertices, std::vector<Edge> edges)
    : vertices(vertices), edges(edges) {}
Vector3D Graph3D::apply_orientation(cube3D::Vector3D &point) {
  return point.rotate_x(orient.alpha)
             .rotate_y(orient.beta)
             .rotate_z(orient.gamma) +
         center;
}

// probably need a better method for rasterizing
// edges instead of just linear sampling
void Graph3D::draw_edges(cube3D::Frame &frame, float dt) {
  for (auto e : edges) {
    Vector3D p = apply_orientation(vertices[e.from]);
    Vector3D q = apply_orientation(vertices[e.to]);
    for (float t = 0; t <= 1; t += dt) {
      Vector3D edge = p * t + q * (1 - t);
      int edge_x =
          std::round(frame.focal_length * (edge.x / (edge.z + frame.z_level)) +
                     (float)frame.height / 2);
      int edge_y =
          std::round(frame.focal_length * (edge.y / (edge.z + frame.z_level)) +
                     (float)frame.width / 2);
      if (edge_x < frame.height && edge_x >= 0 && edge_y < frame.width &&
          edge_y >= 0 && frame.z_level > edge.z &&
          edge.z > frame.depth_buffer(edge_x, edge_y)) {
        frame(edge_x, edge_y) = 1;
        frame.depth_buffer(edge_x, edge_y) = edge.z;
      }
    }
  }
}
void Graph3D::draw_vertices(cube3D::Frame &frame) {
  for (auto v : vertices) {
    Vector3D p = apply_orientation(v);
    int p_x = std::round(frame.focal_length * (p.x / (p.z + frame.z_level)) +
                         (float)frame.height / 2);
    int p_y = std::round(frame.focal_length * (p.y / (p.z + frame.z_level)) +
                         (float)frame.width / 2);
    if (p_x < frame.height && p_x >= 0 && p_y < frame.width && p_y >= 0 &&
        frame.z_level > p.z && p.z > frame.depth_buffer(p_x, p_y)) {
      frame(p_x, p_y) = 1;
      frame.depth_buffer(p_x, p_y);
    }
  }
};

Mesh3D::Mesh3D(std::vector<Vector3D> vertices, std::vector<Triangle> triangles)
    : vertices(vertices), triangles(triangles) {}
Vector3D Mesh3D::apply_orientation(Vector3D &point) {
  return point.rotate_x(orient.alpha)
             .rotate_y(orient.beta)
             .rotate_z(orient.gamma) +
         center;
}
void Mesh3D::draw_triangles(Frame &frame, float ds) {
  for (auto t : triangles) {
    Vector3D p1 = apply_orientation(vertices[t.p1]);
    Vector3D p2 = apply_orientation(vertices[t.p2]);
    Vector3D p3 = apply_orientation(vertices[t.p3]);
    // using barycentric coordinates a*p1+b*p2+c*p3 = p
    // such that a+b+c=1 and a,b,c >= 0
    for (float a = ds; a < 1 - ds; a += ds)
      for (float b = ds; b < 1 - ds; b += ds) {
        if (a + b >= 1)
          continue;
        float c = 1 - a - b;
        Vector3D p = p1 * a + p2 * b + p3 * c;
        int p_x =
            std::round(frame.focal_length * (p.x / (p.z + frame.z_level)) +
                       (float)frame.height / 2);
        int p_y =
            std::round(frame.focal_length * (p.y / (p.z + frame.z_level)) +
                       (float)frame.width / 2);
        if (p_x < frame.height && p_x >= 0 && p_y < frame.width && p_y >= 0 &&
            frame.z_level > p.z && p.z > frame.depth_buffer(p_x, p_y)) {
          frame(p_x, p_y) = 1;
          frame.depth_buffer(p_x, p_y);
        }
      }
  }
}

} // namespace cube3D
