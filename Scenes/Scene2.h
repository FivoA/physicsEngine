//
// Created by felly on 05/11/2025.
//

#ifndef GAMEPHYSICSTEMPLATE_SCENE2_H
#define GAMEPHYSICSTEMPLATE_SCENE2_H

#include "Scene.h"
#include "Particle.h"

class Scene2: public Scene {
    std::vector<Particle> massPoints;

    float timeStep = 0.05f;
    int stepsToSimulate = 1;

    // give this new scene its own onDraw and simulateStep method by overriding the parent Scene's one
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual void onGUI() override;

    void performEulerStep();

    static void printInfoAboutParticles(std::vector<Particle> massPoints);
    void StarSimulation();
};


#endif //GAMEPHYSICSTEMPLATE_SCENE2_H
