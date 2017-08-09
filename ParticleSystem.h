#pragma once
#include "ofMain.h"
#include "FlowField.h"

class ParticleSystem {
public:
    
    ParticleSystem();
    
    void setup(int size, int amount);
    
    void update(FlowField &temp);
    void draw(int x, int y);
    
    void checkBorder();
    
    int particleBoxSize;
    int particleNumber;
    float particleSpacing;
    float rot;
    
    float particleSense, particleSpeedLimit, particleSlowdown, particleKillLimit;
    
    deque<ofVec3f> particleLoc, particleAcc, particleVel;
    
    ofFbo buffer;
    
    ofMesh mesh;
};
