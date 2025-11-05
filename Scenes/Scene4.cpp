//
// Created by felly on 05/11/2025.
//

#include "Scene4.h"
#include <imgui.h>


void Scene4::init() {
    massPoints = {
            Particle(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 4.0f), // front top right 0
            Particle(glm::vec3(0.0f, -2.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 4.0f), // front top left 1
            Particle(glm::vec3(0.0f, -2.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 4.0f), // front bottom left 2
            Particle(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 4.0f), // front bottom right 3
            Particle(glm::vec3(-2.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), 4.0f), // back top right 4
            Particle(glm::vec3(-2.0f, -2.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 4.0f), // back top left 5
            Particle(glm::vec3(-2.0f, -2.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 4.0f), // back bottom left 6
            Particle(glm::vec3(-2.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 4.0f), // back bottom right 7
            Particle(glm::vec3(-1.0f, -1.0f, -2.0f), glm::vec3(0.0f, 0.0f, 1.0f), 4.0f), // Bottom peak 8
            Particle(glm::vec3(-1.0f, -1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 1.0f), 4.0f) // Top peak 9
    };

    // NOTE: connectTo also adds the reverse spring conncetion so if i add 0->1 then 1->0 is also added for particle 1 automatically!
    massPoints[0].connectTo(&massPoints[1], 40, 1);
    massPoints[0].connectTo(&massPoints[3], 40, 1);
    massPoints[0].connectTo(&massPoints[4], 40, 1);
    massPoints[0].connectTo(&massPoints[9], 40, 1);

    massPoints[1].connectTo(&massPoints[2], 40, 1);
    massPoints[1].connectTo(&massPoints[5], 40, 1);
    massPoints[1].connectTo(&massPoints[9], 40, 1);

    massPoints[2].connectTo(&massPoints[3], 40, 1);
    massPoints[2].connectTo(&massPoints[6], 40, 1);
    massPoints[2].connectTo(&massPoints[8], 40, 1);

    massPoints[3].connectTo(&massPoints[7], 40, 1);
    massPoints[3].connectTo(&massPoints[8], 40, 1);

    massPoints[4].connectTo(&massPoints[5], 40, 1);
    massPoints[4].connectTo(&massPoints[7], 40, 1);
    massPoints[4].connectTo(&massPoints[9], 40, 1);

    massPoints[5].connectTo(&massPoints[9], 40, 1);

    massPoints[6].connectTo(&massPoints[5], 40, 1);
    massPoints[6].connectTo(&massPoints[7], 40, 1);
    massPoints[6].connectTo(&massPoints[8], 40, 1);

    massPoints[7].connectTo(&massPoints[8], 40, 1);
}

void Scene4::StarSimulation() {
    for (int i = 0; i < stepsToSimulate; ++i) {
        if(simulationIndex == 0){
            performEulerStep();
        }else if(simulationIndex == 1){
            performMidPointSimulation();
        }else{
            performLeapFrog();
        }
        printf("On step %i:\n" , i+1);
        for (int j = 0; j < massPoints.size(); ++j) {
            massPoints[j].printInfo();
        }
    }
}
void Scene4::onGUI() {
    ImGui::Text("Using: ");
    ImGui::Text(simulations[simulationIndex]);
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.01f);
    ImGui::SliderInt("Steps to simulate: ", &stepsToSimulate, 1, 100);
    ImGui::Combo("Select Simulation", &simulationIndex, simulations, 3);
    ImGui::Checkbox("Toggle Gravity", &gravityActive);
    auto startSim = ImGui::Button("Start Simulation");
    if(startSim){
        StarSimulation();
    }
}

void Scene4::performMidPointSimulation(){
    glm::vec3 acceleration;
    glm::vec3 totalInternalForce;
    // perform mid point step for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 0. Get acceleration
        totalInternalForce = massPoints[i].getTotalInternalForce(1);
        massPoints[i].acceleration = (totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
        // 1. Calculate half timestep velocity and position
        massPoints[i].positionMid = massPoints[i].position + massPoints[i].velocity * (timeStep/2.0f);
        massPoints[i].velocityMid = massPoints[i].velocity + massPoints[i].acceleration * (timeStep/2.0f);
    }
    for (int i = 0; i < massPoints.size(); ++i) {
        // compute midpoint accelerationf for each particle
        totalInternalForce = massPoints[i].getTotalInternalForce(2);
        massPoints[i].acceleration = (totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
    }
    // actually update every particle
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Update position with midpoint velocity
        massPoints[i].position = massPoints[i].position + massPoints[i].velocityMid*timeStep;
        // 2. Update velocity with midpoint acceleration
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;
    }
}
void Scene4::performEulerStep() {
    // perform it for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        glm::vec3 acceleration;
        glm::vec3 totalInternalForce = massPoints[i].getTotalInternalForce(1);
        massPoints[i].acceleration = (totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
        // 2. Update position
        massPoints[i].position = massPoints[i].position + massPoints[i].velocity * timeStep;
        // 3. Update velocity
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;
    }
}

void Scene4::performLeapFrog() {
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        glm::vec3 acceleration;
        glm::vec3 totalInternalForce = massPoints[i].getTotalInternalForce(1);
        massPoints[i].acceleration = (totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));

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
    }
}

void Scene4::onDraw(Renderer &renderer) {
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    for (int i = 0; i < massPoints.size(); ++i) {
        // Draw the particle
        renderer.drawSphere(massPoints[i].position, 0.2, glm::vec4(1,i * 0.1,0,1));
        // Draw all connections (YES I AM AWARE WE DO THIS TWICE PER CONNECTION BUT I DONT CARE SINCE WE DONT HAVE A LOT OF PARTICLES)
        for(const auto& kv: massPoints[i].myConnections){
            SpringPtr springPtr = kv.second;
            renderer.drawLine(springPtr->a->position, springPtr->b->position, glm::vec4(1,0,1,1));
        }
    }
}


void Scene4::simulateStep() {}


