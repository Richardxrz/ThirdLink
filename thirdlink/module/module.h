#ifndef MODULE_H
#define MODULE_H
#include <vector>
struct Vector2 {
  float x;
  float y;
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

  const Links &getlinks() const;

private:
  std::vector<Link> links;
};

#endif // !MODULES_H
