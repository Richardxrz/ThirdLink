#include "raylib.h"
#include "render.h"
#include "settings.h"
#include <iostream>

void PrintInitInfo(const Module::Arm &arm);

int main() {
  // Init arm
  Module::Arm arm(Module::Vector2{400.0f, 300.0f});

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
    Render::DrawArm(arm);
    arm.rotateLink90(0);
    arm.update();
    arm.rotateLink90(1);
    arm.update();

    EndDrawing();
  }
  CloseWindow();

  return 0;
}

/*------------------------- Print Info ------------------------*/

void PrintInitInfo(const Module::Arm &arm) {
  for (auto &link : arm.getLinks()) {
    std::cout << "link.origin.x: " << link.origin.x << std::endl;
    std::cout << "link.origin.y: " << link.origin.y << std::endl;
    std::cout << "link.length: " << link.length << std::endl;
    std::cout << "link.angle: " << link.angle << std::endl;
  }
}
