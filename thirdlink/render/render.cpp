#include "render.h"
#include "raylib.h"
#include "settings.h"

// Draw a pair of joint and link
void Render::DrawLink(const Module::Link &link) {
  Vector2 origin{.x = link.origin.x, .y = link.origin.y};
  Vector2 end{.x = link.end().x, .y = link.end().y};
  DrawLineEx(origin, end, KLinkThick, KLinkColor);
  DrawCircleV(origin, KRadius, KJointColor);
}

// Draw thriarms refferred to Joint0 origin
void Render::DrawArm(const Module::Arm &arm) {
  for (const Module::Link &link : arm.getLinks()) {
    Render::DrawLink(link);
  }
}
