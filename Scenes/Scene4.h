//
// Created by felly on 05/11/2025.
//

#ifndef GAMEPHYSICSTEMPLATE_SCENE4_H
#define GAMEPHYSICSTEMPLATE_SCENE4_H

#include "Scene.h"
#include "Particle.h"
#include "ForceGenerator.h"
#include "Spring.h"
#include "Plane.h"

class Scene4: public Scene {

    std::vector<Particle> massPoints;
    std::vector<Spring> forceGenerators;
    std::vector<Plane> boundingPlanes;

    float timeStep = 0.05f;

    bool gravityActive = false;
    glm::vec3 gravity = glm::vec3(0.0f,0.0f,-9.81f);
    int simulationIndex = 0; // 0 = Euler, 1 = Midpoint, 2 = LeapFrog
    const char* simulations[3] = { "Euler Step", "Midpoint Evaluation", "LeapFrog Integrator"};


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
