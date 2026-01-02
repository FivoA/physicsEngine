#ifndef GAMEPHYSICSTEMPLATE_SPRING_H
#define GAMEPHYSICSTEMPLATE_SPRING_H

#include "ForceGenerator.h"
#include "Particle.h"

class Spring : public ForceGenerator{
public:
    void updateForce(int option) override;
    Spring(float stiff, float rL, Particle* a, Particle* b): stiffness(stiff), restLength(rL), a(a),b(b){} // constructor for springs

    float stiffness;
    float restLength;
    Particle* a;
    Particle* b;
};


#endif //GAMEPHYSICSTEMPLATE_SPRING_H
