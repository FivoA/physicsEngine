//
// Created by felly on 05/11/2025.
//

#include "Scene3.h"
#include <imgui.h>


void Scene3::init() {
    massPoints = {(Particle(glm::vec3(0.0), glm::vec3(-1.0,0.0,0.0), 4.0f)),
                  Particle(glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0,0.0,0.0), 4.0f)};

    forceGenerators = {
            Spring(40.0f, 1.0f, &massPoints[0], &massPoints[1]),
    };
}

void Scene3::onGUI() {
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.01f);
    auto startSim = ImGui::Button("Toggle Simulation");
    if(startSim){
        isSimulating = !isSimulating;
    }
}

void Scene3::performMidPointSimulation(){
    glm::vec3 acceleration;
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    // perform mid point step for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 0. Get acceleration
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass);
        // 1. Calculate half timestep velocity and position
        massPoints[i].positionMid = massPoints[i].position + massPoints[i].velocity * (timeStep/2.0f);
        massPoints[i].velocityMid = massPoints[i].velocity + massPoints[i].acceleration * (timeStep/2.0f);

        // clear internal force
        massPoints[i].clearInternalForce();
    }
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(2);
    }
    for (int i = 0; i < massPoints.size(); ++i) {
        // compute midpoint accelerationf for each particle
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass);
    }
    // actually update every particle
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Update position with midpoint velocity
        massPoints[i].position = massPoints[i].position + massPoints[i].velocityMid*timeStep;
        // 2. Update velocity with midpoint acceleration
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;

        // clear internal force
        massPoints[i].clearInternalForce();
    }
}

void Scene3::onDraw(Renderer &renderer) {
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    for (int i = 0; i < massPoints.size(); ++i) {
        // Draw the particle
        renderer.drawSphere(massPoints[i].position, 0.2, glm::vec4(1,i * 0.1,0,1));
        // Draw all connections (YES I AM AWARE WE DO THIS TWICE PER CONNECTION BUT I DONT CARE SINCE WE DONT HAVE A LOT OF PARTICLES)
        for(Spring spr: forceGenerators){
            renderer.drawLine(spr.a->position, spr.b->position, glm::vec4(1,0,1,1));
        }
    }
}
void Scene3::simulateStep() {
    if(isSimulating) {
        accTime += ImGui::GetIO().DeltaTime;
        if (accTime >= timeStep) {
            performMidPointSimulation();
            accTime = 0.0f;
        }
    }
}

