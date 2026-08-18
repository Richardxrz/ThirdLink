#include "module.h"
#include "raylib.h"
#include "render.h"
#include "settings.h"
#include <iostream>

void DrawArm(const Arm &arm);
void PrintInitInfo(const Arm &arm);

int main() {
  // Init arm
  Arm arm(KScreenMid);
  // Arm arm{Link{{}, 100.0f, std::numbers::pi_v<float> / 5.0f},
  //         Link{{}, 100.0f, std::numbers::pi_v<float> * 4.0f / 5.0f},
  //         Link{{}, 45.0f, std::numbers::pi_v<float>}};
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
  for (auto &link : arm.myLinks()) {
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
  for (const Link &link : arm.myLinks()) {
    DrawLink(link);
  }
}
