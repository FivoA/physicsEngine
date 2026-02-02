#ifndef GAMEPHYSICSTEMPLATE_SCENE4_H
#define GAMEPHYSICSTEMPLATE_SCENE4_H

#include "Scene.h"
#include "Particle.h"
#include "ForceGenerator.h"
#include "Spring.h"
#include "Plane.h"
#include "Box.h"

class Scene4: public Scene {

    std::vector<Particle> massPoints;
    std::vector<Spring> forceGenerators;
    std::vector<Plane> boundingPlanes;
    std::vector<glm::vec3> faceReferences;
    float timeStep = 0.05f;

    glm::vec3 crashVelocity;

    Box box;
    float c;

    bool gravityActive = false;
    glm::vec3 gravity = glm::vec3(0.0f,0.0f,-9.81f);
    int simulationIndex = 0; // 0 = Euler, 1 = Midpoint
    const char* simulations[2] = { "Euler Step", "Midpoint Evaluation"};

    int accelerationIndex = 0; // 0=spatial grid, 1= sdf
    const char* accelerations[2] = { "Spatial Grid", "SDF"};

    float cellSize;

    struct gridKey {
        int x, y, z;
        bool operator==(const gridKey& other) const {
            return x==other.x && y == other.y && z ==other.z;
        }
    };
    struct gridHasher {
        std::size_t operator()(const gridKey& k) const{
            return ((std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1)) >> 1) ^ (std::hash<int>()(k.z)<<1); //just some hashing
        }
    };
    std::unordered_map<gridKey, std::vector<int>, gridHasher> spatialGrid;


    void updateSpacialGrid();
    std::vector<int> getParticlesNearBox();
    float boxSDF(glm::vec3 p);
    // give this new scene its own onDraw and simulateStep method by overriding the parent Scene's one
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void init() override;
    virtual void onGUI() override;

    void performEulerStep();
    void performMidPointSimulation();
    void StarSimulation();
    void CheckCollisionsBox2Particle(const std::vector<int>& indices);

    virtual void loadObj(std::string path);
};


#endif //GAMEPHYSICSTEMPLATE_SCENE4_H
