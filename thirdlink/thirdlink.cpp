#include "module.h"
#include "raylib.h"
#include "render.h"
#include "settings.h"
#include <iostream>

void PrintInitInfo(const Arm &arm);

int main() {
  // Init arm
  Arm arm(KScreenMid);

  // Print arm init info
  PrintInitInfo(arm);

  // Init window
  InitWindow(KWidth, KHeight, "ThirdLink Simulation");
  SetTraceLogLevel(LOG_WARNING);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(KBackgroundColor);

    // Draw thriarm
    DrawArm(arm);

    EndDrawing();
  }
  CloseWindow();

  return 0;
}

/*------------------------- Print Info ------------------------*/

void PrintInitInfo(const Arm &arm) {
  for (auto &link : arm.getlinks()) {
    std::cout << "link.origin.x: " << link.origin.x << std::endl;
    std::cout << "link.origin.y: " << link.origin.y << std::endl;
    std::cout << "link.length: " << link.length << std::endl;
    std::cout << "link.angle: " << link.angle << std::endl;
  }
}
