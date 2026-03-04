#include <iostream>
#include <vector>

class Frame
{
public:
  const int FRAME_X; // frame height
  const int FRAME_Y; // frame width
  double z_level;    // distance from xy plane
  double scale;      // scaling factor

  std::vector<std::vector<int>> pixel_grid;

  Frame(int FRAME_X, int FRAME_Y, double z_level, double scale)
    : FRAME_X(FRAME_X)
    , FRAME_Y(FRAME_Y)
    , z_level(z_level)
    , scale(scale)
  {
    for (int i = 0; i < FRAME_X; i++) {
      std::vector<int> row;
      for (int j = 0; j < FRAME_Y; j++)
        row.push_back(0);

      pixel_grid.push_back(row);
    }
  }

  void show()
  {
    std::cout << "\033[3J\n";
    for (auto i = pixel_grid.begin(); i < pixel_grid.end(); ++i) {
      for (auto j = i->begin(); j < i->end(); ++j) {
        switch (*j) {
          case 5:
            std::cout << "@@";
            break;
          case 4:
            std::cout << "oo";
            break;
          case 3:
            std::cout << ". ";
            break;
          case 2:
            std::cout << ". ";
            break;
          case 1:
            std::cout << "  ";
            break;
          case 0:
            std::cout << "  ";
            break;
        }
        *j = 0;
      }
      std::cout << "\n";
    }
  };
};
