#ifndef MODULES_H
#define MODULES_H
#include "raylib.h"
#include <vector>

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

  const Links &myLinks(void) const;

private:
  std::vector<Link> links;
};

#endif // !MODULES_H
