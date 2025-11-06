//
// Created by felly on 06/11/2025.
//

#include "Spring.h"

void Spring::updateForce(int option) {
    glm::vec3 posA = (option ==1) ? a->position : a->positionMid; // allow for midpoint!
    glm::vec3 posB = (option ==1) ? b->position : b->positionMid; // allow for midpoint!

    glm::vec3 dir = posB - posA; // from a to b
    float l = glm::length(dir);
    float stretch = l - this->restLength; // (l - L)

    float magnitude = this->stiffness * stretch;

    const float eps = 1e-6f;
    if(l < eps){
        printf("Stopping before diving by an l that's close to 0!");
        return;
    }

    dir = dir / l; // make it unit vector
    glm::vec3 forceOnA = magnitude * dir;
    glm::vec3 forceOnB = -forceOnA;
    // add to force of Particle A
    a->addInternalForce(forceOnA);
    // add to force of Particle B
    b->addInternalForce(forceOnB);
}
