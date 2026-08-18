#include "raylib.h"
#include "render.h"
#include "settings.h"

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
