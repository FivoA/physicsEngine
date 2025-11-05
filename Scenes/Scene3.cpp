//
// Created by felly on 05/11/2025.
//

#include "Scene3.h"
#include <imgui.h>


void Scene3::init() {
    massPoints = {(Particle(glm::vec3(0.0), glm::vec3(-1.0,0.0,0.0), 4.0f)),
                  Particle(glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0,0.0,0.0), 4.0f)};

    massPoints[0].connectTo(&massPoints[1], 40, 1);
}

void Scene3::StarSimulation() {
    for (int i = 0; i < stepsToSimulate; ++i) {
        performMidPointSimulation();
        printf("On step %i:\n" , i+1);
        for (int j = 0; j < massPoints.size(); ++j) {
            massPoints[j].printInfo();
        }
    }
}
void Scene3::onGUI() {
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.01f);
    ImGui::SliderInt("Steps to simulate: ", &stepsToSimulate, 1, 100);
    auto startSim = ImGui::Button("Start Simulation");
    if(startSim){
        StarSimulation();
    }
}

void Scene3::performMidPointSimulation(){
    glm::vec3 acceleration;
    glm::vec3 totalInternalForce;
    // perform mid point step for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 0. Get acceleration
        totalInternalForce = massPoints[i].getTotalInternalForce(1);
        massPoints[i].acceleration = totalInternalForce / massPoints[i].mass;
        // 1. Calculate half timestep velocity and position
        massPoints[i].positionMid = massPoints[i].position + massPoints[i].velocity * (timeStep/2.0f);
        massPoints[i].velocityMid = massPoints[i].velocity + massPoints[i].acceleration * (timeStep/2.0f);
    }
    for (int i = 0; i < massPoints.size(); ++i) {
        // compute midpoint accelerationf for each particle
        totalInternalForce = massPoints[i].getTotalInternalForce(2);
        massPoints[i].acceleration = totalInternalForce / massPoints[i].mass;
    }
    // actually update every particle
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Update position with midpoint velocity
        massPoints[i].position = massPoints[i].position + massPoints[i].velocityMid*timeStep;
        // 2. Update velocity with midpoint acceleration
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;
    }
}

void Scene3::onDraw(Renderer &renderer) {
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    for (int i = 0; i < massPoints.size(); ++i) {
        // Draw the particle
        renderer.drawSphere(massPoints[i].position, 0.2, glm::vec4(1,1,0,1));
        // Draw all connections (YES I AM AWARE WE DO THIS TWICE PER CONNECTION BUT I DONT CARE SINCE WE DONT HAVE A LOT OF PARTICLES)
        for(const auto& kv: massPoints[i].myConnections){
            SpringPtr springPtr = kv.second;
            renderer.drawLine(springPtr->a->position, springPtr->b->position, glm::vec4(1,0,1,1));
        }
    }
}

void Scene3::simulateStep() {}

