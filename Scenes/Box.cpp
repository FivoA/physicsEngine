#include "Box.h"
#include <glm/gtx/quaternion.hpp>

Box::Box(float w, float h, float d, float m) {
    color = glm::vec4(1);

    scale = glm::vec3{w,h,d};

    mass = m;
    if (m < 0) inverseInertiaTensor = glm::mat3(1);
    else inverseInertiaTensor = glm::mat3x3{1 / (m * (h*h + d*d) / 12.f), 0, 0, 0, 1 / (m * (w*w + d*d) / 12.f), 0, 0, 0, 1 / (m * (w*w + h*h) / 12.f)};

    position = glm::vec3(0);
    rotation = glm::quat(glm::vec3(0));

    velocity = glm::vec3(0);
    angularMomentum = glm::vec3(0);
    angularVelocity = glm::vec3(0);
}

// jn: directional Impuls
// x: world position of collision point
void Box::ApplyImpuls(glm::vec3 jn, glm::vec3 x) {
    if (mass < 0) return;
    velocity += jn / mass;
    angularMomentum += glm::cross(x - position, jn);

};

void Box::SimulateStep(float dt) {    
    glm::mat3 rot = glm::toMat3(rotation);                                      // r -> R
    glm::mat3x3 inertia = rot * inverseInertiaTensor * glm::transpose(rot);     // R*I^-1*R^T
    angularVelocity = inertia * angularMomentum;                                // angular velocity

    position += velocity * dt;
    rotation += dt * .5f * glm::quat{0, angularVelocity} * rotation;
    rotation = glm::normalize(rotation);
};

void Box::ApplyForce(glm::vec3 f, glm::vec3 x, float dt) {
    if (mass < 0) return;
    velocity += f * dt / mass;
    angularMomentum += glm::cross(x - position, f) * dt;
}

void Box::CalculateModelMatrix() {
    glm::mat4 rot = glm::toMat4(rotation);
    glm::mat4 scl = glm::scale(glm::mat4(1), scale);
    glm::mat4 trns = glm::translate(glm::mat4(1), position);
    modelMatrix = trns * rot * scl;
}

glm::vec3 Box::CalculateVelocityOfWorldPoint(glm::vec3 p) {
    return velocity + glm::cross(angularVelocity, p - position);
}

float Box::GetInverseMass() {
    if (mass < 0) return 0.f;
    return 1.f / mass;
}

glm::mat3 Box::GetInverseInertiaTensor() {
    if (mass < 0) return glm::mat3(0);
    glm::mat3 rot = glm::toMat3(rotation);
    glm::mat3x3 inertia = rot * inverseInertiaTensor * glm::transpose(rot);
    return inertia;
}