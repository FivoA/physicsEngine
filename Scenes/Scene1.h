#include "Scene.h"
#include "imgui.h"
#include <random>
#include <imgui.h>
#include "Particle.h"



class Scene1: public Scene{
    std::vector<Particle> massPoints;

    float timeStep = 0.1f;

    // give this new scene its own onDraw and simulateStep method by overriding the parent Scene's one
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual void onGUI() override;

    void performEulerStep();
    void performMidpointEval();

    static void printInfoAboutParticles(std::vector<Particle> massPoints);
};

