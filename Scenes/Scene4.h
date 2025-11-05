//
// Created by felly on 05/11/2025.
//

#ifndef GAMEPHYSICSTEMPLATE_SCENE4_H
#define GAMEPHYSICSTEMPLATE_SCENE4_H

#include "Scene.h"
#include "Particle.h"

class Scene4: public Scene {

    std::vector<Particle> massPoints;

    float timeStep = 0.05f;

    int stepsToSimulate = 1;
    bool gravityActive = false;
    glm::vec3 gravity = glm::vec3(0.0f,0.0f,-9.81f);
    int simulationIndex = 0; // 0 = Euler, 1 = Midpoint, 2 = LeapFrog
    const char* simulations[3] = { "Euler Step", "Midpoint Evaluation", "LeapFrog Integrator"};

    float floorY = -2.5f;

    // give this new scene its own onDraw and simulateStep method by overriding the parent Scene's one
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual void onGUI() override;

    void performEulerStep();
    void performMidPointSimulation();
    void performLeapFrog();
    void StarSimulation();
};


#endif //GAMEPHYSICSTEMPLATE_SCENE4_H
