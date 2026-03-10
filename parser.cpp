#include "cube3D.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace cube3D {
Graph3D
parser::parse_to_graph3D(std::string path)
{

  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("couldn't open " + path);
  }

  std::vector<Point> vertices;
  std::vector<Edge> edges;

  std::string line;
  int vertexIndex = 0;

  std::vector<std::vector<int>> faces;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "v") {
      float x, y, z;
      iss >> x >> y >> z;
      vertices.push_back(Point(x, y, z));
      vertexIndex++;
    } else if (type == "f") {
      std::vector<int> face;
      std::string vert;
      while (iss >> vert) {
        int idx = std::stoi(vert.substr(0, vert.find('/')));
        int simplifiedIdx = idx - 1;
        face.push_back(simplifiedIdx);
      }
      faces.push_back(face);
    }
  }

  for (auto& face : faces) {
    int n = face.size();
    for (int i = 0; i < n; ++i) {
      int from = face[i];
      int to = face[(i + 1) % n];
      edges.push_back({ from, to });
    }
  }

  return Graph3D(vertices, edges);
};

Mesh3D
parser::parse_to_mesh3D(std::string path)
{

  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("couldn't open " + path);
  }

  std::vector<Point> vertices;
  std::vector<Edge> edges;

  std::string line;
  int vertexIndex = 0;

  std::vector<std::vector<int>> faces;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "v") {
      float x, y, z;
      iss >> x >> y >> z;
      vertices.push_back(Point(x, y, z));
      vertexIndex++;
    } else if (type == "f") {
      std::vector<int> face;
      std::string vert;
      while (iss >> vert) {
        int idx = std::stoi(vert.substr(0, vert.find('/')));
        int simplifiedIdx = idx - 1;
        face.push_back(simplifiedIdx);
      }
      faces.push_back(face);
    }
  }
};
}
