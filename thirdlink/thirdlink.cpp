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

struct Link {
  Vector2 start;
  float length;
  float phi;
};

struct Joint {
  Vector2 origin;
};

struct JointLinkPair {
  Joint joint;
  Link link;
  Vector2 start;
  float length;
  float phi;
};

// struct Arm {
//   JointLinkPair *jlpairs;
//   int jlpair_num;
// };

struct Segment {
  Vector2 origin{};
  float length{};
  float phi{};

  [[nodiscard]] Vector2 end() const {
    return {origin.x + length * std::cos(phi),
            origin.y - length * std::sin(phi)};
  }
};

struct Arm {
  std::vector<Segment> segments;
  void draw() const;
};

Link CreateLink(Vector2 start, float length, float phi);
Joint CreateJoint(Vector2 origin);

void DrawJointLinkPair(const JointLinkPair &jlpair);
void DrawArm(const Arm &arm);

void PrintInfo(void);

void InitJointLinkPairs(JointLinkPair jlpairs[], Joint joints[], Link links[],
                        int count);
void InitArm(Arm &arm, JointLinkPair jlpairs[], int jlpair_num);

inline void PrintInitJointLinkPairInfo(const JointLinkPair jlpairs[]);
inline void PrintInitArmInfo(const Arm &arm);

// Create links and joints
Link links[3] = {};
Joint joints[3] = {};
JointLinkPair jlpairs[3] = {};
Arm arm = {};
int jlpair_num = 3;

int main() {
  // Init links and joints
  InitJointLinkPairs(jlpairs, joints, links, jlpair_num);
  InitArm(arm, jlpairs, jlpair_num);
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
Vector2 CalculateEndPos(Vector2 start, float length, float phi) {
  Vector2 end;
  end = {.x = start.x + length * std::cos(phi),
         .y = start.y - length * std::sin(phi)};
  return end;
}

void PrintInitJointLinkPairInfo(const JointLinkPair jlpairs[], int i) {
  std::cout << "jlpair.joint.origin.x: " << jlpairs[i].joint.origin.x
            << std::endl;
  std::cout << "jlpair.joint.origin.y: " << jlpairs[i].joint.origin.y
            << std::endl;
  std::cout << "jlpair.link.start.x: " << jlpairs[i].link.start.x << std::endl;
  std::cout << "jlpair.link.start.y: " << jlpairs[i].link.start.y << std::endl;
  std::cout << "jlpair.start.x: " << jlpairs[i].start.x << std::endl;
  std::cout << "jlpair.start.y: " << jlpairs[i].start.y << std::endl;
  std::cout << "jlpair.length: " << jlpairs[i].length << std::endl;
  std::cout << "jlpair.phi: " << jlpairs[i].phi << std::endl;
}

void PrintInitArmInfo(const Arm &arm) {
  for (int i = 0; i < arm.segments.size(); i++) {
    std::cout << "arm.jlpairs->joint.origin.x: "
              << arm.segments << std::endl;
    std::cout << "arm.jlpairs->joint.origin.y: "
              << arm.segments. << std::endl;
    std::cout << "arm.jlpairs->link.start.x: " << arm.jlpairs[i].link.start.x
              << std::endl;
    std::cout << "arm.jlpairs->link.start.y: " << arm.jlpairs[i].link.start.y
              << std::endl;
    std::cout << "arm.jlpairs->link.length: " << arm.jlpairs[i].link.length
              << std::endl;
    std::cout << "arm.jlpairs->link.phi: " << arm.jlpairs[i].link.phi
              << std::endl;
  }
}
} // namespace

/*------------------------- Print Info ------------------------*/

void PrintInfo(void) {
  for (int i = 0; i < arm.jlpair_num; i++) {

    PrintInitJointLinkPairInfo(jlpairs, i);
  }
  PrintInitArmInfo(arm);
}

/*-------------------------- Draw -----------------------------*/

// Draw a pair of joint and link
void DrawJointLinkPair(const JointLinkPair &jlpair) {
  Vector2 end = CalculateEndPos(jlpair.start, jlpair.length, jlpair.phi);
  DrawLineEx(jlpair.start, end, LINK_THICK, LINK_COLOR);
  Vector2 origin = jlpair.start;
  DrawCircleV(origin, RADIUS, JOINT_COLOR);
}

// Draw thriarms refferred to Joint0 origin
void DrawArm(const Arm &arm) {
  for (int i = 0; i < arm.jlpair_num; i++) {
    DrawJointLinkPair((arm.jlpairs[i]));
  }
}

/*------------------------- Assignment -------------------------*/

// Refferred to origin to create joint
Joint CreateJoint(Vector2 origin) { return Joint{origin}; }

// Refferred to startPos, length, phi to create link
Link CreateLink(Vector2 start, float length, float phi) {
  Link link = {};
  link.start = start;
  link.length = length;
  link.phi = phi;
  return link;
}

// Assignment JointLinkPair
void InitJointLinkPairs(JointLinkPair jlpairs[], Joint joints[], Link links[],
                        int count) {
  // Set initial values
  Vector2 start = BUTTON_MID;
  float length = 100.0f;
  float phi = std::numbers::pi / 2;
  Vector2 origin = start;
  // assignment
  for (int i = 0; i < count; i++) {
    links[i] = CreateLink(start, length, phi);
    joints[i] = CreateJoint(origin);
    jlpairs[i] = {.joint = joints[i],
                  .link = links[i],
                  .start = start,
                  .length = length,
                  .phi = phi};
    start = CalculateEndPos(start, length, phi);
    origin = start;
  }
}

// Init Arm
void InitArm(Arm &arm, JointLinkPair jlpairs[], int jlpair_num) {
  arm = {.jlpairs = jlpairs, .jlpair_num = jlpair_num};
}
