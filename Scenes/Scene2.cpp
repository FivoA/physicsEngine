//
// Created by felly on 05/11/2025.
//

#include "Scene2.h"
#include <imgui.h>
#include <unistd.h>
#include "Particle.h"

void Scene2::init() {
    massPoints = {(Particle(glm::vec3(0.0), glm::vec3(-1.0,0.0,0.0), 4.0f)),
                  Particle(glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0,0.0,0.0), 4.0f)};

    massPoints[0].connectTo(&massPoints[1], 40, 1);

}

void Scene2::onDraw(Renderer &renderer) {
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
void Scene2::onGUI() {
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.01f);
    ImGui::SliderInt("Steps to simulate: ", &stepsToSimulate, 1, 100);
    auto startSim = ImGui::Button("Start Simulation");
    if(startSim){
        StarSimulation();
    }
}

void Scene2::simulateStep() {}

void Scene2::StarSimulation(){
    for (int i = 0; i < stepsToSimulate; ++i) {
        performEulerStep();
        printf("On step %i:\n" , i+1);
        for (int j = 0; j < massPoints.size(); ++j) {
            massPoints[j].printInfo();
        }
    }
}
void Scene2::performEulerStep() {
    // perform it for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        glm::vec3 acceleration;
        glm::vec3 totalInternalForce = massPoints[i].getTotalInternalForce(1);
        massPoints[i].acceleration = totalInternalForce / massPoints[i].mass;
        // 2. Update position
        massPoints[i].position = massPoints[i].position + massPoints[i].velocity * timeStep;
        // 3. Update velocity
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;
    }
}
