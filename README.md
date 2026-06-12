# Cube3D: Terminal-Based 3D Software Rasterizer

A lightweight, zero-dependency 3D software rendering engine built from scratch in modern C++. `Cube3D` bypasses high-level graphics APIs (like OpenGL or DirectX) to process vertices, project coordinates, and rasterize wireframes and solid meshes directly into a standard terminal window using ANSI escape sequences.

---

## 🚀 Key Features

* **Custom Wavefront `.obj` Parser**: Built-in I/O tokenizers that decode geometric vertices (`v`) and polygonal face elements (`f`) directly into renderable entities.
* **Dynamic Polygon Triangulation**: Automatically decomposes complex n-gon face structures ($n > 3$) into discrete triangles on-the-fly to maintain pipeline compatibility.
* **Euler-Angle Rotation Engine**: Full 3D rotation mechanics ($\alpha, \beta, \gamma$) mapped across independent pitch, yaw, and roll transform matrices.
* **Mathematical Surface Interpolation**: Uses barycentric coordinates to map surface spaces dynamically, tracking spatial distributions across individual primitives.
* **Depth-Buffer Occlusion Culling**: Employs a custom per-cell Z-buffer checking mechanism to handle visual sorting and prevent background geometries from overwriting foreground fragments.

---

## 🛠️ Architecture & Pipeline Overview

The rendering engine operates via a discrete graphics pipeline executed entirely on the CPU:

> [ .obj File ] ---> [ Parser: Triangulation ] ---> [ Apply Rotation/Scale ]
> |
> [ Frame Buffer ] <--- [ Depth Buffer Test ] <--- [ Perspective Projection ]

### 1. File Parsing & Topology Reconstruction

The parser scans target files sequentially. When loading polygonal meshes, it tracks global structural reference arrays and decomposes faces containing arbitrary vertex sequences into uniform hardware-friendly triangles.

### 2. Coordinate Transformation & Perspective Projection

Vertices are translated using 3D trigonometric rotation functions based on the object's current orientation matrices. The system then applies a perspective division operation to project 3D space vectors onto a flat 2D viewport plane relative to the camera focal constraints:

$$x_{2d} = f \cdot \left(\frac{x_{3d}}{z_{3d} + z_{level}}\right)$$

### 3. Z-Buffer Occlusion Processing

To resolve pixel mapping order without sorting polygons manually, target fragment coordinates undergo an active depth evaluation check against a managed `DepthBuffer` matrix. Points are only written to the frame grid if they sit in front of the existing cell data.

---

## 📂 Project Structure

* `cube3D.cpp`: Entry point and initialization of application presets.
* `cube3D.h`: Primary namespaces, custom vectors, and interface wrappers.
* `frame.cpp`: Terminal display controller, viewport memory, and buffer resets.
* `object3D.cpp`: 3D spatial transformations, edge tracking, and rasterization algorithms.
* `parser.cpp`: Wavefront .obj text tokenizers for graphs and meshes.
* `presets.cpp`: Hardcoded scene assets and animation loop behaviors (Tux, Cube, Penger).

---

## ⚡ Getting Started

### Prerequisites

* A compiler supporting **C++17** or higher (e.g., `gcc`, `clang`).
* A terminal environment that supports ANSI/VT100 escape codes (for screen refreshes).

### Compilation

Compile the implementation layers together using your preferred compiler setup:

> g++ -std=c++17 -O3 cube3D.cpp frame.cpp object3D.cpp parser.cpp presets.cpp -o cube3D

*Note: Using the `-O3` flag is highly recommended to optimize the floating-point math inside the tight nested rendering loops.*

### Running a Scene

Run the compiled binary to cycle through the active default demonstration:

> ./cube3D

---

## 📈 Technical Roadmap

* **Bresenham's Line Optimization**: Replace parametric line rendering (`dt` stepping loops) with an integer-based step optimizer to eliminate spacing gaps across rapid scale changes.
* **Bounded Box Rasterization Fragment Pass**: Connect calculated triangle bounds inside `Triangle::rasterize` directly to a structural scanline filling matrix to allow for opaque solid-colored meshes.
* **Illumination Vector Maps**: Introduce surface normal processing pipelines to support flat ambient, directional, and Lambertian surface shading modules.

---

## 📄 License

This project is open-source and available under the [MIT License](https://www.google.com/search?q=LICENSE).
