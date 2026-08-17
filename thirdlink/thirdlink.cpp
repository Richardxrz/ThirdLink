#include "raylib.h"
#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>

constexpr Color KBackGroundColor{BLACK};
constexpr int KWidth{800};
constexpr int KHeight{600};
constexpr float KRadius{5.0f};
constexpr Color KJointColor{RED};
constexpr Color KLinkColor{WHITE};
constexpr float KLinkThick{5.0f};
constexpr Vector2 KScreenMid{400.0f, 300.0f};

struct Link {
  Vector2 origin{};
  float length{};
  float angle{};

  Vector2 end() const {
    return {origin.x + length * std::cos(angle),
            origin.y - length * std::sin(angle)};
  }
};

using Arm = std::vector<Link>;

void DrawArm(const Arm &arm);
void InitArm(Arm &arm);
void PrintInitInfo(const Arm &arm);

int main() {
  // Init arm
  Arm arm{Link{{}, 100.0f, std::numbers::pi_v<float> / 5.0f},
          Link{{}, 100.0f, std::numbers::pi_v<float> * 4.0f / 5.0f},
          Link{{}, 45.0f, std::numbers::pi_v<float>}};
  InitArm(arm);
  PrintInitInfo(arm);

  // Init window
  InitWindow(KWidth, KHeight, "ThirdLink Simulation");
  SetTraceLogLevel(LOG_WARNING);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(KBackGroundColor);

    // Draw thriarm
    DrawArm(arm);

    EndDrawing();
  }
  CloseWindow();

  return 0;
}

/*------------------------- Print Info ------------------------*/

void PrintInitInfo(const Arm &arm) {
  for (auto &link : arm) {
    std::cout << "link.origin.x: " << link.origin.x << std::endl;
    std::cout << "link.origin.y: " << link.origin.y << std::endl;
    std::cout << "link.length: " << link.length << std::endl;
    std::cout << "link.angle: " << link.angle << std::endl;
  }
}

/*-------------------------- Draw -----------------------------*/

// Draw a pair of joint and link
void DrawLink(const Link &link) {
  DrawLineEx(link.origin, link.end(), KLinkThick, KLinkColor);
  Vector2 origin = link.origin;
  DrawCircleV(origin, KRadius, KJointColor);
}

// Draw thriarms refferred to Joint0 origin
void DrawArm(const Arm &arm) {
  for (const Link &link : arm) {
    DrawLink(link);
  }
}

/*------------------------- Assignment -------------------------*/

// Assignment JointLinkPair
void InitArm(Arm &arm) {
  // Set initial values
  Vector2 origin = KScreenMid;
  // assignment
  for (auto &link : arm) {
    link.origin = origin;
    origin = link.end();
  }
}
