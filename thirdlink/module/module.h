#ifndef MODULE_H
#define MODULE_H
#include <vector>

/*------------------- Namespace start -------------------*/
namespace Module {
struct Vector2 {
  float x;
  float y;
};

struct CoordinateFrame {
  Vector2 origin{400.0f, 300.0f};  // 坐标系原点
  Vector2 basis_x{}; // X轴单位向量
  Vector2 basis_y{}; // Y轴单位向量
  Vector2 transform{origin};
};

struct Link {
  Vector2 origin{};
  float length{};
  float angle{};

  Vector2 end() const;
};

using Links = std::vector<Link>;

class Arm {
public:
  Arm(Vector2 origin);

  const Links &getLinks() const;
  const Vector2 &getEnd() const;
  void rotateLink90(int);
  void update();

private:
  std::vector<Link> links;
  Vector2 end;
};
} // namespace Module
/*------------------- Namespace end -----------------*/

#endif // !MODULES_H
