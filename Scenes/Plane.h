//
// Created by felly on 06/11/2025.
//

#ifndef GAMEPHYSICSTEMPLATE_PLANE_H
#define GAMEPHYSICSTEMPLATE_PLANE_H

#include "Particle.h"

class Plane {
public:
    glm::vec3 uniquePoint;
    glm::vec3 surfaceNormal; // is unitized

    Plane(glm::vec3 p, glm::vec3 n): uniquePoint(p), surfaceNormal(glm::normalize(n)){};

    void checkCollisionAndCorrectPosition(Particle* particle){
        //d is the distance from the origin along the normal
        float d = glm::dot(surfaceNormal, uniquePoint);
        // signed distance from particle to plane
        float distance = glm::dot(surfaceNormal, particle->position);

        if(distance < d){
            // particle is below plane and we push it back out
            particle->position += (d-distance) * surfaceNormal;
            //reflect particles velocity along the normal
            particle->velocity -= 2.0f * glm::dot(particle->velocity, surfaceNormal) * surfaceNormal;
        }
    }
    // I am adding another method here instead of adding options just for my own sanity
    void MIDcheckCollisionAndCorrectPosition(Particle* particle){
        //d is the distance from the origin along the normal
        float d = glm::dot(surfaceNormal, uniquePoint);
        // signed distance from particle to plane
        float distance = glm::dot(surfaceNormal, particle->positionMid);

        if(distance < d){
            // particle is below plane and we push it back out
            particle->positionMid += (d-distance) * surfaceNormal;
            //reflect particles velocity along the normal
            particle->velocityMid -= 2.0f * glm::dot(particle->velocityMid, surfaceNormal) * surfaceNormal;
        }
    }

    // adding another one here for leapfrog since it needs to access the prev velocity
    void LEAPcheckCollisionAndCorrectPosition(Particle* particle){
        //d is the distance from the origin along the normal
        float d = glm::dot(surfaceNormal, uniquePoint);
        // signed distance from particle to plane
        float distance = glm::dot(surfaceNormal, particle->position);

        if(distance < d){
            // particle is below plane and we push it back out
            particle->position += (d-distance) * surfaceNormal;
            //reflect particles velocity along the normal
            particle->velocity -= 2.0f * glm::dot(particle->velocity, surfaceNormal) * surfaceNormal;
            particle->velocityHalfPrev -= 2.0f * glm::dot(particle->velocityHalfPrev, surfaceNormal) * surfaceNormal;
        }
    }
};


#endif //GAMEPHYSICSTEMPLATE_PLANE_H
