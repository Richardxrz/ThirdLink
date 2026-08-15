#include "raylib.h"
#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>

constexpr Color BGCOLOR{BLACK};
constexpr int WIDTH{800};
constexpr int HEIGHT{600};
constexpr float RADIUS{5.0f};
constexpr Color JOINT_COLOR{RED};
constexpr Color LINK_COLOR{WHITE};
constexpr float LINK_THICK{5.0f};
constexpr Vector2 BUTTON_MID{400.0f, 600.0f};
constexpr Vector2 SCREEN_MID{400.0f, 300.0f};

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
  Arm arm{
      Link{{}, 100.0f, std::numbers::pi_v<float> / 5.0f},        // 连杆 0
      Link{{}, 100.0f, std::numbers::pi_v<float> * 4.0f / 5.0f}, // 连杆 1
      Link{{}, 45.0f, std::numbers::pi_v<float>},                // 连杆 2
  };
  InitArm(arm);
  PrintInitInfo(arm);

  // Init window
  InitWindow(WIDTH, HEIGHT, "ThirdLink Simulation");
  SetTraceLogLevel(LOG_WARNING);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BGCOLOR);

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
  DrawLineEx(link.origin, link.end(), LINK_THICK, LINK_COLOR);
  Vector2 origin = link.origin;
  DrawCircleV(origin, RADIUS, JOINT_COLOR);
}

// Draw thriarms refferred to Joint0 origin
void DrawArm(const Arm &arm) {
  for (int i = 0; i < arm.size(); i++) {
    DrawLink(arm[i]);
  }
}

/*------------------------- Assignment -------------------------*/

// Assignment JointLinkPair
void InitArm(Arm &arm) {
  // Set initial values
  Vector2 origin = SCREEN_MID;
  // assignment
  for (auto &link : arm) {
    link.origin = origin;
    origin = link.end();
  }
}
