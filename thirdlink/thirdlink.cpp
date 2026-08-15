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

void DrawJointLinkPair(const Link &link);
void DrawArm(const Arm &arm);
void InitArm(Arm &arm);
void PrintInitInfo(const Arm &arm);

Link link1, link2, link3;
Arm arm{link1, link2, link3};

int main() {
  // Init links and joints
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
  for (int i = 0; i < arm.size(); i++) {
    std::cout << "arm[ " << i << " ].origin.x: " << arm[i].origin.x
              << std::endl;
    std::cout << "arm[ " << i << " ].origin.y: " << arm[i].origin.y
              << std::endl;
    std::cout << "arm[ " << i << " ].length: " << arm[i].length << std::endl;
    std::cout << "arm[ " << i << " ].angle: " << arm[i].angle << std::endl;
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
  arm[0].length = 100.0f;
  arm[1].length = 100.0f;
  arm[2].length = 45.0f;
  arm[0].angle = std::numbers::pi / 5;
  arm[1].angle = std::numbers::pi / 5 * 4;
  arm[2].angle = std::numbers::pi;
  for (int i = 0; i < arm.size(); i++) {
    arm[i].origin = origin;
    origin = arm[i].end();
  }
}
