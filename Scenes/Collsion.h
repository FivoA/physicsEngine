#include <glm/glm.hpp>

struct CollInfo {
    bool isColliding;
    glm::vec3 normal;
};

CollInfo CheckCollisionParticleBox(glm::vec3 particlePos, glm::mat4 boxModelMatrix);