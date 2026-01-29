#include "TestScene.h"
#include <glm/gtx/quaternion.hpp>

void TestScene::init() {
    box.rotation = glm::normalize(glm::quat(glm::vec3(0.785398f, 0, 0)));
    box.CalculateModelMatrix();
    CollInfo coll = CheckCollisionParticleBox(point, box.modelMatrix);
    printf("CollInfo: colliding: %s, normal: %f, %f, %f", coll.isColliding ? "TRUE" : "FALSE", coll.normal.x, coll.normal.y, coll.normal.z);
};