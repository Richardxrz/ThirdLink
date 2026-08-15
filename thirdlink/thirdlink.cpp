#include "raylib.h"
#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>

#define BGCOLOR BLACK
#define WIDTH 800
#define HEIGHT 600
#define RADIUS 5.0f
#define JOINT_COLOR RED
#define LINK_COLOR WHITE
#define LINK_THICK 5.0f

constexpr Vector2 BUTTON_MID{400.0f, 600.0f};

struct JointLinkPair {
  Vector2 origin{};
  float length{};
  float phi{};

  Vector2 end() const {
    return {origin.x + length * std::cos(phi),
            origin.y - length * std::sin(phi)};
  }
};

using Arm = std::vector<JointLinkPair>;

void DrawJointLinkPair(const JointLinkPair &jlpair);
void DrawArm(const Arm &arm);
void PrintInfo(void);
void InitJointLinkPairs(Arm &arm);
inline void PrintInitJointLinkPairInfo(const JointLinkPair jlpairs[]);

JointLinkPair jlpair1, jlpair2, jlpair3;
Arm arm{jlpair1, jlpair2, jlpair3};

int main() {
  // Init links and joints
  InitJointLinkPairs(arm);
  PrintInfo();

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

/*---------------------- Private Tool Function ------------------------*/

namespace {
void PrintInitJointLinkPairInfo(const Arm &arm, int i) {
  std::cout << "arm[ " << i << " ].origin.x: " << arm[i].origin.x << std::endl;
  std::cout << "arm[ " << i << " ].origin.y: " << arm[i].origin.y << std::endl;
  std::cout << "arm[ " << i << " ].length: " << arm[i].length << std::endl;
  std::cout << "arm[ " << i << " ].phi: " << arm[i].phi << std::endl;
}
} // namespace

/*------------------------- Print Info ------------------------*/

void PrintInfo(void) {
  for (int i = 0; i < arm.size(); i++) {

    PrintInitJointLinkPairInfo(arm, i);
  }
}

/*-------------------------- Draw -----------------------------*/

// Draw a pair of joint and link
void DrawJointLinkPair(const JointLinkPair &jlpair) {
  DrawLineEx(jlpair.origin, jlpair.end(), LINK_THICK, LINK_COLOR);
  Vector2 origin = jlpair.origin;
  DrawCircleV(origin, RADIUS, JOINT_COLOR);
}

// Draw thriarms refferred to Joint0 origin
void DrawArm(const Arm &arm) {
  for (int i = 0; i < arm.size(); i++) {
    DrawJointLinkPair(arm[i]);
  }
}

/*------------------------- Assignment -------------------------*/

// Assignment JointLinkPair
void InitJointLinkPairs(Arm &arm) {
  // Set initial values
  Vector2 origin = BUTTON_MID;
  float length = 100.0f;
  float phi = std::numbers::pi / 2;
  // assignment
  for (int i = 0; i < arm.size(); i++) {
    arm[i] = {.origin = origin, .length = length, .phi = phi};
    origin = arm[i].end();
  }
}
