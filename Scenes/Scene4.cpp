//
// Created by felly on 05/11/2025.
//

#include "Scene4.h"
#include <imgui.h>
#include "Spring.h"
#include "Plane.h"

void Scene4::init() {
    boundingPlanes = {
        Plane(glm::vec3(0.0f, 0.0f, -2.5f), glm::vec3(0.0f, 0.0f, 1.0f)), //bottom plane
        Plane(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3(0.0f, 0.0f, -1.0f)), // top plane
        Plane(glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // left plane
        Plane(glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)), // right plane
        Plane(glm::vec3(2.5f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), // front plane
        Plane(glm::vec3(-2.5f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), // back plane
    };
    massPoints = {
            Particle(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 4.0f), // front top right 0
            Particle(glm::vec3(0.0f, -2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // front top left 1
            Particle(glm::vec3(0.0f, -2.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // front bottom left 2
            Particle(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // front bottom right 3
            Particle(glm::vec3(-2.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // back top right 4
            Particle(glm::vec3(-2.0f, -2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // back top left 5
            Particle(glm::vec3(-2.0f, -2.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // back bottom left 6
            Particle(glm::vec3(-2.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // back bottom right 7
            Particle(glm::vec3(-1.0f, -1.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f), // Bottom peak 8
            Particle(glm::vec3(-1.0f, -1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f) // Top peak 9
    };
    forceGenerators = {
            Spring(40, glm::length(massPoints[0].position - massPoints[1].position), &massPoints[0], &massPoints[1]),
            Spring(40, glm::length(massPoints[0].position - massPoints[1].position), &massPoints[0], &massPoints[3]),
            Spring(40, glm::length(massPoints[0].position - massPoints[4].position), &massPoints[0], &massPoints[4]),
            Spring(40, glm::length(massPoints[0].position - massPoints[9].position), &massPoints[0], &massPoints[9]),
            Spring(40, glm::length(massPoints[1].position - massPoints[2].position), &massPoints[1], &massPoints[2]),
            Spring(40, glm::length(massPoints[1].position - massPoints[5].position), &massPoints[1], &massPoints[5]),
            Spring(40, glm::length(massPoints[1].position - massPoints[9].position), &massPoints[1], &massPoints[9]),
            Spring(40, glm::length(massPoints[2].position - massPoints[3].position), &massPoints[2], &massPoints[3]),
            Spring(40, glm::length(massPoints[2].position - massPoints[6].position), &massPoints[2], &massPoints[6]),
            Spring(40, glm::length(massPoints[2].position - massPoints[8].position), &massPoints[2], &massPoints[8]),
            Spring(40, glm::length(massPoints[3].position - massPoints[7].position), &massPoints[3], &massPoints[7]),
            Spring(40, glm::length(massPoints[3].position - massPoints[8].position), &massPoints[3], &massPoints[8]),
            Spring(40, glm::length(massPoints[4].position - massPoints[5].position), &massPoints[4], &massPoints[5]),
            Spring(40, glm::length(massPoints[4].position - massPoints[7].position), &massPoints[4], &massPoints[7]),
            Spring(40, glm::length(massPoints[4].position - massPoints[9].position), &massPoints[4], &massPoints[9]),
            Spring(40, glm::length(massPoints[5].position - massPoints[9].position), &massPoints[5], &massPoints[9]),
            Spring(40, glm::length(massPoints[6].position - massPoints[5].position), &massPoints[6], &massPoints[5]),
            Spring(40, glm::length(massPoints[6].position - massPoints[7].position), &massPoints[6], &massPoints[7]),
            Spring(40, glm::length(massPoints[6].position - massPoints[8].position), &massPoints[6], &massPoints[8]),
            Spring(40, glm::length(massPoints[7].position - massPoints[8].position), &massPoints[7], &massPoints[8]),
    };
}

void Scene4::onGUI() {
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.01f);
    ImGui::Combo("Select Simulation", &simulationIndex, simulations, 3);
    ImGui::Checkbox("Toggle Gravity", &gravityActive);
    auto startSim = ImGui::Button("Toggle Simulation");
    if(startSim){
        isSimulating = !isSimulating;
    }
}

void Scene4::performMidPointSimulation(){
    glm::vec3 acceleration;
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    // perform mid point step for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 0. Get acceleration
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
        // 1. Calculate half timestep velocity and position
        massPoints[i].positionMid = massPoints[i].position + massPoints[i].velocity * (timeStep/2.0f);
        massPoints[i].velocityMid = massPoints[i].velocity + massPoints[i].acceleration * (timeStep/2.0f);

        // Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].MIDcheckCollisionAndCorrectPosition(&massPoints[i]);
        }

        // clear internal force
        massPoints[i].clearInternalForce();
    }
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(2);
    }
    for (int i = 0; i < massPoints.size(); ++i) {
        // compute midpoint accelerationf for each particle
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
    }
    // actually update every particle
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Update position with midpoint velocity
        massPoints[i].position = massPoints[i].position + massPoints[i].velocityMid*timeStep;
        // 2. Update velocity with midpoint acceleration
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;

        // Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].checkCollisionAndCorrectPosition(&massPoints[i]);
        }

        // clear internal force
        massPoints[i].clearInternalForce();
    }
}
void Scene4::performEulerStep() {
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    // perform it for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
        // 2. Update position
        massPoints[i].position = massPoints[i].position + massPoints[i].velocity * timeStep;
        // 3. Update velocity
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;

        // 4 Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].checkCollisionAndCorrectPosition(&massPoints[i]);
        }
        // clear internal force
        massPoints[i].clearInternalForce();
    }
}

void Scene4::performLeapFrog() {
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));

        // 2. init v(t-h/2) if not done before
        if(!massPoints[i].leapfroginit){
            massPoints[i].velocityHalfPrev = massPoints[i].velocity - massPoints[i].acceleration * timeStep  * 0.5f;
            massPoints[i].leapfroginit = true;
        }
        // 2. Update velocity
        glm::vec3 halfStepVelocity = massPoints[i].velocityHalfPrev + timeStep * massPoints[i].acceleration;
        massPoints[i].velocityHalfPrev = halfStepVelocity;
        massPoints[i].velocity = halfStepVelocity;
        // 3. Update position
        massPoints[i].position = massPoints[i].position + halfStepVelocity * timeStep;

        // Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].LEAPcheckCollisionAndCorrectPosition(&massPoints[i]);
        }

        // clear internal force
        massPoints[i].clearInternalForce();
    }
}

void Scene4::onDraw(Renderer &renderer) {
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    for (int i = 0; i < massPoints.size(); ++i) {
        // Draw the particle
        renderer.drawSphere(massPoints[i].position, 0.1, glm::vec4(1,i * 0.1,0,1));
        // Draw all connections (YES I AM AWARE WE DO THIS TWICE PER CONNECTION BUT I DONT CARE SINCE WE DONT HAVE A LOT OF PARTICLES)
        for(Spring spr: forceGenerators){
            renderer.drawLine(spr.a->position, spr.b->position, glm::vec4(1,0,1,1));
        }
    }
    for (int i = 0; i < boundingPlanes.size(); ++i) {
        renderer.drawSphere(boundingPlanes[i].uniquePoint, 0.05, glm::vec4(1,1,0, 0.15));
    }
}

float accTime = 0.0f;
void Scene4::simulateStep() {
    if(isSimulating) {
        accTime += ImGui::GetIO().DeltaTime;
        if (accTime >= timeStep) {
            if (simulationIndex == 0) {
                performEulerStep();
            } else if (simulationIndex == 1) {
                performMidPointSimulation();
            } else {
                performLeapFrog();
            }

            accTime = 0.0f;
        }
    }
}


