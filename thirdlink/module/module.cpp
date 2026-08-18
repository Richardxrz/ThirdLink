#include "module.h"
#include <cmath>
#include <numbers>

Vector2 Link::end() const {
  return {origin.x + length * std::cos(angle),
          origin.y - length * std::sin(angle)};
}

using Links = std::vector<Link>;

Arm::Arm(Vector2 origin)
    : links{Link{{}, 100.0f, std::numbers::pi_v<float> / 5.0f},
            Link{{}, 100.0f, std::numbers::pi_v<float> * 4.0f / 5.0f},
            Link{{}, 45.0f, std::numbers::pi_v<float>}} {
  for (Link &link : links) {
    link.origin = origin;
    origin = link.end();
  }
}

const Links &Arm::myLinks(void) const { return Arm::links; };
