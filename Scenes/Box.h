#include <Renderer.h>

#ifndef PHYSICS_BOX
#define PHYSICS_BOX

class Box
{
public:
    glm::vec3 scale;
    glm::vec4 color;

    glm::mat4 modelMatrix;

    float mass;                         // m; m<0 is handled is m=Infinity
    glm::mat3x3 inertiaTensor;          // I
    glm::mat3x3 inverseInertiaTensor;   // I^-1

    glm::vec3 position;                 // xcm
    glm::quat rotation;                 // r

    glm::vec3 velocity;                 // v
    glm::vec3 angularMomentum;          // L

    glm::vec3 angularVelocity;

    Box() = default;
    Box(float w, float h, float d, float m);
    void ApplyForce(glm::vec3 f, glm::vec3 x, float dt);
    void ApplyImpuls(glm::vec3 jn, glm::vec3 x);
    void SimulateStep(float dt);
    void CalculateModelMatrix();
    glm::vec3 CalculateVelocityOfWorldPoint(glm::vec3 p);
    float GetInverseMass();
    glm::mat3 GetInverseInertiaTensor();
};


#endif