//
// Created by felly on 05/11/2025.
//

#ifndef GAMEPHYSICSTEMPLATE_PARTICLE_H
#define GAMEPHYSICSTEMPLATE_PARTICLE_H

#include <utility>

#include "Renderer.h"

class Particle;

struct Spring {
    int stiffness;
    int restLength;
    Particle* a;
    Particle* b;
    Spring(int stiff, int rL, Particle* a, Particle* b): stiffness(stiff), restLength(rL), a(a),b(b){} // constructor for springs
};

using SpringPtr = std::shared_ptr<Spring>;

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;

    glm::vec3 positionMid;
    glm::vec3 velocityMid;

    glm::vec3 velocityHalfPrev = glm::vec3(0);
    bool leapfroginit = false;

    glm::vec3 acceleration;

    float mass;
    std::map<Particle*, SpringPtr> myConnections;

    Particle(glm::vec3 position, glm::vec3 velocity, float mass): position(position), velocity(velocity), mass(mass){}; // constructor for particles

    glm::vec3 getTotalInternalForce(int option){
        glm::vec3 total = glm::vec3(0);
        for(const auto& kv: myConnections){
            Particle* other = kv.first;
            SpringPtr springPtr = kv.second;
            if(!other || !springPtr) continue;

            // get direction from this to other particle
            glm::vec3 dir;
            if (option == 1){
                // use current position
                dir = other->position - this->position;
            }else if(option == 2){
                // use mid position
                dir = other->positionMid - this->positionMid;
            }
            // get small l aka distance
            float l = dir.length();

            total+= ((-1.0f)* springPtr->stiffness * (l - springPtr->restLength)) * (dir / l);
        }
        return total;
    }
    void connectTo(Particle* other, int stiffness, int restLength){
        if(!other || other == this) return;
        SpringPtr spr = std::make_shared<Spring>(stiffness, restLength, this, other);
        myConnections[other] = spr;
        other->myConnections[this]  = spr;
    }

    void printInfo(){
        printf("position: vec3(%f, %f, %f)\n", this->position.x, this->position.y, this->position.z);
        printf("velocity: vec3(%f, %f, %f)\n", this->velocity.x, this->velocity.y, this->velocity.z);
        printf("Mass: %f\n", this->mass);
    }

};

#endif //GAMEPHYSICSTEMPLATE_PARTICLE_H
