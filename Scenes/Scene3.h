//
// Created by felly on 05/11/2025.
//

#ifndef GAMEPHYSICSTEMPLATE_SCENE3_H
#define GAMEPHYSICSTEMPLATE_SCENE3_H

#include "Scene.h"
#include "Particle.h"
#include "Spring.h"

class Scene3: public Scene {
    std::vector<Particle> massPoints;
    std::vector<Spring> forceGenerators;

    float timeStep = 0.05f;
    float accTime = 0.0f;
    // give this new scene its own onDraw and simulateStep method by overriding the parent Scene's one
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual void onGUI() override;

    void performMidPointSimulation();

    static void printInfoAboutParticles(std::vector<Particle> massPoints);
};


#endif //GAMEPHYSICSTEMPLATE_SCENE3_H
