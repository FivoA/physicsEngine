#include "Collsion.h"
#include <limits>

CollInfo CheckCollisionParticleBox(glm::vec3 particlePos, glm::mat4 boxModelMatrix) {
    glm::vec3 normal = glm::vec3(0);
    float surfaceDistance = std::numeric_limits<float>::max();

    glm::vec3 planePoint = boxModelMatrix[3];

    for (int i = 0; i < 3; i++) {
        glm::vec3 planeNormal = glm::normalize(boxModelMatrix[i]);
        float extend = 0.5f * glm::length(boxModelMatrix[i]);
        // dist(point, plane(p, n)) = dot(plane.n, point - plane.p)
        float sgnD = glm::dot(planeNormal, particlePos - planePoint);
        float absD = std::abs(sgnD);
        if (absD > extend) return CollInfo{false, glm::vec3(0)};

        float dist2surf = extend - absD;
        if (dist2surf < surfaceDistance) {
            surfaceDistance = dist2surf;
            normal = planeNormal * (std::signbit(sgnD) ? -1.0f : 1.0f);
        }
    }

    return CollInfo{true, normal};
};