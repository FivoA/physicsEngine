#include "Scene4.h"
#include <imgui.h>
#include "Spring.h"
#include "Plane.h"
#include "Collsion.h"

void Scene4::init() {
    massPoints = {};
    // TO USE, REPLACE WITH OWN PATH OF OBJ FILE!!
    loadObj(R"(C:\Users\felly\CLionProjects\game-physics-template\Scenes\Chevrolet_Camaro_SS_Low.obj)");
    //massPoints.push_back(Particle(glm::vec3(1,0,0.1f), glm::vec3(-1,0,0), 2.f));
    box.rotation = glm::normalize(glm::quat(glm::vec3(0, 0.785398f, 0)));
}

void Scene4::onGUI() {
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.01f);
    ImGui::Combo("Select Simulation", &simulationIndex, simulations, 3);
    ImGui::InputFloat("Gravity X:", &gravity.x);
    ImGui::InputFloat("Gravity Y:", &gravity.y);
    ImGui::InputFloat("Gravity Z:", &gravity.z);
    ImGui::Checkbox("Toggle the selected gravity", &gravityActive);
    auto startSim = ImGui::Button("Toggle Simulation");
    if(startSim){
        isSimulating = !isSimulating;
    }
}

void Scene4::performMidPointSimulation(){
    glm::vec3 acceleration;
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    // perform mid point step for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 0. Get acceleration
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
        // 1. Calculate half timestep velocity and position
        massPoints[i].positionMid = massPoints[i].position + massPoints[i].velocity * (timeStep/2.0f);
        massPoints[i].velocityMid = massPoints[i].velocity + massPoints[i].acceleration * (timeStep/2.0f);

        // Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].MIDcheckCollisionAndCorrectPosition(&massPoints[i]);
        }

        // clear internal force
        massPoints[i].clearInternalForce();
    }
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(2);
    }
    // actually update every particle
    for (int i = 0; i < massPoints.size(); ++i) {
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
        // 1. Update position with midpoint velocity
        massPoints[i].position = massPoints[i].position + massPoints[i].velocityMid*timeStep;
        // 2. Update velocity with midpoint acceleration
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;

        // Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].checkCollisionAndCorrectPosition(&massPoints[i]);
        }

        // clear internal force
        massPoints[i].clearInternalForce();
    }
}
void Scene4::performEulerStep() {
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    // perform it for every particle!
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));
        // 2. Update position
        massPoints[i].position = massPoints[i].position + massPoints[i].velocity * timeStep;
        // 3. Update velocity
        massPoints[i].velocity = massPoints[i].velocity + massPoints[i].acceleration * timeStep;

        // 4 Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].checkCollisionAndCorrectPosition(&massPoints[i]);
        }
        // clear internal force
        massPoints[i].clearInternalForce();
    }
}

void Scene4::performLeapFrog() {
    // let each spring add its force
    for (int i = 0; i < forceGenerators.size(); ++i) {
        forceGenerators[i].updateForce(1);
    }
    for (int i = 0; i < massPoints.size(); ++i) {
        // 1. Calculate accerleration from forces
        massPoints[i].acceleration = (massPoints[i].totalInternalForce / massPoints[i].mass) + (gravityActive ? gravity: glm::vec3(0));

        // 2. Init v(t-h/2) if not done before;
        // NOTE FOR TUTOR: We don't assume that v(t-h/2) is v(0), we decided to make a reverse prediction here to calculate it :)
        if(!massPoints[i].leapfroginit){
            massPoints[i].velocityHalfPrev = massPoints[i].velocity - massPoints[i].acceleration * timeStep  * 0.5f;
            massPoints[i].leapfroginit = true;
        }
        // 2. Update velocity
        glm::vec3 halfStepVelocity = massPoints[i].velocityHalfPrev + timeStep * massPoints[i].acceleration;
        massPoints[i].velocityHalfPrev = halfStepVelocity;
        massPoints[i].velocity = halfStepVelocity;
        // 3. Update position
        massPoints[i].position = massPoints[i].position + halfStepVelocity * timeStep;

        // Check for collision with bounding planes and reset position/velocity if needed
        for (int j = 0; j < boundingPlanes.size(); ++j) {
            boundingPlanes[j].LEAPcheckCollisionAndCorrectPosition(&massPoints[i]);
        }

        // clear internal force
        massPoints[i].clearInternalForce();
    }
}

void Scene4::onDraw(Renderer &renderer) {
    renderer.drawCube(box.position, box.rotation, box.scale, box.color);

    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    for (int i = 0; i < massPoints.size(); ++i) {
        // Draw the particle
        renderer.drawSphere(massPoints[i].position, 0.1, glm::vec4(1,i * 0.1,0,1));
    }
    for(Spring spr: forceGenerators){
        renderer.drawLine(spr.a->position, spr.b->position, glm::vec4(1,0,1,1));
    }
    for (int i = 0; i < boundingPlanes.size(); ++i) {
        renderer.drawSphere(boundingPlanes[i].uniquePoint, 0.05, glm::vec4(1,1,0, 0.15));
    }
}

float accTime = 0.0f;
void Scene4::simulateStep() {
    if(isSimulating) {       
        accTime += ImGui::GetIO().DeltaTime;
        if (accTime >= timeStep) {
            // perform collision detection
            CheckCollisionsBox2Particle(massPoints);
            // TODO: replace this with correct particle set determined by accelaration algorithm

            // perform physics steps:
            // box step
            if (gravityActive) box.ApplyForce(gravity * box.mass, box.position, timeStep);
            box.SimulateStep(timeStep);


            // mass spring step
            if (simulationIndex == 0) {
                performEulerStep();
            } else if (simulationIndex == 1) {
                performMidPointSimulation();
            } else {
                performLeapFrog();
            }
            accTime = 0.0f;
        }
    }
}

void Scene4::CheckCollisionsBox2Particle(std::vector<Particle> &particleSet) {
    box.CalculateModelMatrix();
    for (int i = 0; i < particleSet.size(); i++) {
        
        CollInfo collision = CheckCollisionParticleBox(particleSet[i].position, box.modelMatrix);
        if (collision.isColliding) {
            glm::vec3 v_rel = particleSet[i].velocity - box.CalculateVelocityOfWorldPoint(particleSet[i].position);
            glm::vec3 xBox = particleSet[i].position - box.position; // XBOX OMG xDDDDDDDDDDDDDDDDDDD
            float dvn = glm::dot(v_rel, collision.normal);
            if (dvn <= 0) {
                float q = -(1.f + c) * glm::dot(v_rel, collision.normal);
                float d = (1.f / particleSet[i].mass) + box.GetInverseMass() +
                glm::dot(
                    (glm::cross(box.GetInverseInertiaTensor() * glm::cross(xBox, collision.normal), xBox)),
                    collision.normal
                );
                float impuls = q / d;
                particleSet[i].velocity += impuls * collision.normal / particleSet[i].mass;
                box.ApplyImpuls(-impuls * collision.normal, particleSet[i].position);
            }
        }
    }
}

void Scene4::loadObj(std::string path) {
    std::string text;
    std::ifstream file(path);
    if (!file.is_open()) {
        printf("Failed to open file");
        return;
    }
    while(std::getline(file, text)){
        if(text.rfind("v ", 0) == 0) {
            printf("Vertex:\n");
            float x, y, z;
            char tag;
            std::istringstream iss(text);
            iss >> tag >> x >> y >> z;
            std::swap(y,z); // swap to align rotation since coordinate systems might be weird. This is hardcoded but all models i could find so far had this issue
            printf("x: %f , y: %f, z: %f \n", x,y,z);
            massPoints.emplace_back(glm::vec3(x,y,z), glm::vec3(0), 1.0f);
        }
    }
    file.clear();
    file.seekg(0, std::ios::beg);
    while(std::getline(file, text)){
        if(text.rfind("f ", 0) == 0) {
            printf("Face\n");
            std::string x, y, z;
            char tag;
            std::istringstream iss(text);
            //float stiff, float rL, Particle* a, Particle* b
            iss >> tag >> x >> y >> z;
            auto parseIndex = [](const std::string& s){
                return std::stoi(s.substr(0,s.find('/'))) -1;
            };
            int v1 = parseIndex(x);
            int v2 = parseIndex(y);
            int v3 = parseIndex(z);
            forceGenerators.emplace_back(1.0f, 2.0f, &massPoints[v1], &massPoints[v2]);
            forceGenerators.emplace_back(1.0f, 2.0f, &massPoints[v2], &massPoints[v3]);
            forceGenerators.emplace_back(1.0f, 2.0f, &massPoints[v3], &massPoints[v1]);
            faceReferences.emplace_back(v1,v2,v3); // Purpose: to keep track of faces for rotation recomputation
        }
    }
    
    file.close();
}