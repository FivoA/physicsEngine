#include "Scene1.h"
#include "glm/gtx/quaternion.hpp"
#include <imgui.h>
#include "Particle.h"

void Scene1::init() {
    massPoints = {(Particle(glm::vec3(0.0), glm::vec3(-1.0,0.0,0.0), 4.0f)),
                  Particle(glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0,0.0,0.0), 4.0f)};

    forceGenerators = {
            Spring(40, 1, &massPoints[0], &massPoints[1]),
    };

    printf("Initial Values:\n");
    printInfoAboutParticles(massPoints);

    // call euler method
    performEulerStep();
    printf("\nAfter 0.1s using Euler Method:\n");
    printInfoAboutParticles(massPoints);

    // Just resetting here, cant be asked to save two or reverse it or whatever
    massPoints = {(Particle(glm::vec3(0.0), glm::vec3(-1.0,0.0,0.0), 4.0f)),
                  Particle(glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0,0.0,0.0), 4.0f)};

    forceGenerators = {
            Spring(40, 1, &massPoints[0], &massPoints[1]),
    };

    // call midpoint method
    performMidPointSimulation();
    printf("\nAfter 0.1s using Midpoint Method:\n");
    printInfoAboutParticles(massPoints);
}

void Scene1::onDraw(Renderer &renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

}

void Scene1::simulateStep() {
    //pitch+= pitch_increment;
    //yaw += yaw_increment;
    //roll += roll_increment;
    float realtimeDT = ImGui::GetIO().DeltaTime;

    glm::vec3 gravityAccel = glm::vec3(0,0,-9.81f);

}

void Scene1::onGUI() {
    auto launch = ImGui::Button("Launch");
}

void Scene1::performEulerStep() {
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

void Scene1::performMidPointSimulation(){
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

void Scene1::printInfoAboutParticles(std::vector<Particle> massPoints){
    for (int i = 0; i < massPoints.size(); ++i) {
        printf("Masspoint %i:\n", i);
        massPoints[i].printInfo();

    }
}

