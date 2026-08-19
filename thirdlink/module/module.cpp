#include "module.h"
#include <cmath>
#include <numbers>

using namespace Module;

Vector2 Module::Link::end() const {
  return {origin.x + length * std::cos(angle),
          origin.y - length * std::sin(angle)};
}

Arm::Arm(Vector2 origin)
    : links{Link{{}, 100.0f, std::numbers::pi_v<float> / 5.0f},
            Link{{}, 100.0f, std::numbers::pi_v<float> * 4.0f / 5.0f},
            Link{{}, 45.0f, std::numbers::pi_v<float>}} {
  for (Link &link : links) {
    link.origin = origin;
    origin = link.end();
  }
}

const Links &Arm::getLinks() const { return Arm::links; };
const Vector2 &Arm::getEnd() const { return Arm::end; };

void Arm::rotateLink90(int x) {
  links[x].angle += std::numbers::pi_v<float> / 2.0f;
};

void Arm::update() {
  for (int i = 1; i < 3; i++) {
    Arm::links[i].origin = Arm::links[i - 1].end();
  }
  Arm::end = links[2].end();
}
