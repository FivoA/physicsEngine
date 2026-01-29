#ifndef TEST_SCENE
#define TEST_SCENE

#include "Scene.h"
#include "Box.h"
#include "Collsion.h"

class TestScene : public Scene {
    public:
    Box box = Box(1,1,1,1);
    glm::vec3 point = glm::vec3(-0.4f,0.0f,0.0f);

    //virtual void onDraw(Renderer &renderer) override;
    //virtual void simulateStep() override;
    virtual void init() override;
    //virtual void onGUI() override;
};

#endif