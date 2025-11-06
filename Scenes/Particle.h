//
// Created by felly on 05/11/2025.
//

#ifndef GAMEPHYSICSTEMPLATE_PARTICLE_H
#define GAMEPHYSICSTEMPLATE_PARTICLE_H

#include <utility>

#include "Renderer.h"


class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;

    glm::vec3 positionMid;
    glm::vec3 velocityMid;

    glm::vec3 velocityHalfPrev = glm::vec3(0);
    bool leapfroginit;

    glm::vec3 acceleration;

    glm::vec3 totalInternalForce;

    float mass;

    Particle(glm::vec3 position, glm::vec3 velocity, float mass):
    position(position),
    velocity(velocity),
    mass(mass),
    acceleration(glm::vec3(0)),
    totalInternalForce(glm::vec3(0)),
    positionMid(glm::vec3(0)),
    velocityMid(glm::vec3(0)),
    leapfroginit(false)
    {};

    void printInfo() const{
        printf("position: vec3(%f, %f, %f)\n", this->position.x, this->position.y, this->position.z);
        printf("velocity: vec3(%f, %f, %f)\n", this->velocity.x, this->velocity.y, this->velocity.z);
        printf("Mass: %f\n", this->mass);
    }
    void addInternalForce(glm::vec3 newForce){
        this->totalInternalForce += newForce;
    }
    void clearInternalForce(){
        this->totalInternalForce = glm::vec3(0);
    }
};

#endif //GAMEPHYSICSTEMPLATE_PARTICLE_H
