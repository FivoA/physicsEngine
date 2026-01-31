#include "Scene4.h"
#include <imgui.h>
#include "Spring.h"
#include "Plane.h"
#include "Collsion.h"

void Scene4::init() {
    massPoints = {};
    // TO USE, REPLACE WITH OWN PATH OF OBJ FILE!!
    loadObj(R"(C:\Users\felly\CLionProjects\game-physics-template\Scenes\Car.obj)");
    box = Box(1.0f, 10.0f, 10.0f, -1.0f); //made mass negative to simulate as wall!
    box.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    box.rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));
    box.position = glm::vec3(0.0f, 5.0f, 0.0f);
    crashVelocity = glm::vec3(0.0f, 15.0f, 0.0f);
    for(auto& p: massPoints){
        p.velocity = crashVelocity;
    }

    c = 0.25f;
    cellSize = 2.0f;
}

void Scene4::onGUI() {
    ImGui::SliderFloat("Timestep: ", &timeStep, 0.001f, 0.005f);
    ImGui::Combo("Select Simulation", &simulationIndex, simulations, 3);
    ImGui::Combo("Select Collision Acceleration", &accelerationIndex, accelerations, 2);

    ImGui::InputFloat("Gravity X:", &gravity.x);
    ImGui::InputFloat("Gravity Y:", &gravity.y);
    ImGui::InputFloat("Gravity Z:", &gravity.z);
    ImGui::Checkbox("Toggle the selected gravity", &gravityActive);

    ImGui::InputFloat("Vehicle Velocity (ONLY INPUT POSITIVE VALUE):", &crashVelocity.y);
    ImGui::InputFloat("Coefficient of Restitution:", &c);
    ImGui::InputFloat("Grid Cell Size:", &cellSize);

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

    for (int i = 0; i < massPoints.size(); ++i) {
        // Draw the particle
        renderer.drawSphere(massPoints[i].position, 0.05, glm::vec4(1,i * 0.1,0,1));
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
            std::vector<int> candidates;
            if(accelerationIndex==0) {
                //using spatial grid as acceleration to create candidates
                updateSpacialGrid();
                candidates = getParticlesNearBox();
            }else{
                //using sdf as acceleration
                //pre filtering indices where sdf < threshold
                for (int i = 0; i < massPoints.size(); ++i) {
                    if (boxSDF(massPoints[i].position) < 1.0f) { // 1.0f is small buffer
                        candidates.push_back(i);
                    }
                }
            }
            CheckCollisionsBox2Particle(candidates);

            // box step; Note: we dont need this really since box is static anyway
            //if (gravityActive) box.ApplyForce(gravity * box.mass, box.position, timeStep);
            //box.SimulateStep(timeStep);


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

void Scene4::CheckCollisionsBox2Particle(const std::vector<int>& particleIndices) {
    box.CalculateModelMatrix();
    for (int i : particleIndices) {
        Particle& p = massPoints[i];
        CollInfo collision = CheckCollisionParticleBox(p.position, box.modelMatrix);
        if (collision.isColliding) {
            glm::vec3 v_rel = p.velocity - box.CalculateVelocityOfWorldPoint(p.position);
            glm::vec3 xBox = p.position - box.position; // XBOX OMG xDDDDDDDDDDDDDDDDDDD
            float dvn = glm::dot(v_rel, collision.normal);
            if (dvn <= 0) {
                float q = -(1.f + c) * glm::dot(v_rel, collision.normal);
                float d = (1.f / p.mass) + box.GetInverseMass() +
                glm::dot(
                    (glm::cross(box.GetInverseInertiaTensor() * glm::cross(xBox, collision.normal), xBox)),
                    collision.normal
                );
                float impuls = q / d;
                p.velocity += impuls * collision.normal / p.mass;
                box.ApplyImpuls(-impuls * collision.normal, p.position);
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
            int vs[3] = {v1, v2, v3};
            // for each triangle edge, calculate the actual distance for rL
            for (int i = 0; i < 3; i++) {
                int a = vs[i];
                int b = vs[(i + 1) % 3];

                float restLength = glm::distance(massPoints[a].position, massPoints[b].position);
                forceGenerators.emplace_back(50.0f, restLength, &massPoints[a], &massPoints[b]);
            }
            faceReferences.emplace_back(v1,v2,v3); // Purpose: to keep track of faces for rotation recomputation
        }
    }
    
    file.close();
}

void Scene4::updateSpacialGrid() {
    spatialGrid.clear();
    for (int i = 0; i < massPoints.size(); ++i) {
        gridKey key = {
                static_cast<int>(std::floor(massPoints[i].position.x / cellSize)),
                static_cast<int>(std::floor(massPoints[i].position.y / cellSize)),
                static_cast<int>(std::floor(massPoints[i].position.z / cellSize))
        };
        spatialGrid[key].push_back(i);
    }
}
std::vector<int> Scene4::getParticlesNearBox() {
    std::vector<int> nearbyIndices;
    // calculate Box AABB in world space
    // since wall is a 10x10 wall at Y=15, we check that. if we cahnged that, we would of course need to change these hard coded values
    glm::vec3 minB = box.position - (box.scale * 1.5f);
    glm::vec3 maxB = box.position + (box.scale * 1.5f);

    int minX = std::floor(minB.x / cellSize);
    int maxX = std::floor(maxB.x / cellSize);
    int minY = std::floor(minB.y / cellSize);
    int maxY = std::floor(maxB.y / cellSize);
    int minZ = std::floor(minB.z / cellSize);
    int maxZ = std::floor(maxB.z / cellSize);

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            for (int z = minZ; z <= maxZ; ++z) {
                gridKey key = {x, y, z};
                if (spatialGrid.count(key)) {
                    auto& cellParticles = spatialGrid[key];
                    nearbyIndices.insert(nearbyIndices.end(), cellParticles.begin(), cellParticles.end());
                }
            }
        }
    }
    return nearbyIndices;
}
float Scene4::boxSDF(glm::vec3 p) {
    //1. transform point into local space of box
    glm::vec3 localP = glm::conjugate(box.rotation) * (p-box.position);
    // 2. calcuate distance to box bounds
    glm::vec3 d = glm::abs(localP) - (box.scale * 0.5f);
    // 3. combine axis distances
    float externalDistance = glm::length(glm::max(d, 0.0f));
    float internatDistance = glm::min(glm::max(d.x, glm::max(d.y, d.z)), 0.0f);
    return externalDistance + internatDistance;
}