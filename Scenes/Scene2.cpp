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

    forceGenerators = {
            Spring(40, 1, &massPoints[0], &massPoints[1]),
    };
}

void Scene2::onDraw(Renderer &renderer) {
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
void Scene2::onGUI() {
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.01f);
    auto startSim = ImGui::Button("Toggle Simulation");
    if(startSim){
        isSimulating = !isSimulating;
    }
}

void Scene2::simulateStep() {
    if(isSimulating) {
        accTime += ImGui::GetIO().DeltaTime;
        if (accTime >= timeStep) {
            performEulerStep();
            accTime = 0.0f;
        }
    }
}

void Scene2::performEulerStep() {
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    // perform it for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass);
        // 2. Update position
        massPoints[i].position = massPoints[i].position + massPoints[i].velocity * timeStep;
        // 3. Update velocity
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;

        // clear internal force
        massPoints[i].clearInternalForce();
    }
}
