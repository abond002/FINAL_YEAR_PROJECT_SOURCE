#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
    rot = 0;
    buffer.allocate(1920, 1080, GL_RGBA);
    mesh.enableIndices();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

void ParticleSystem::setup(int size, int amount) {
    particleBoxSize = size;
    particleNumber = amount;
    particleSpacing = particleBoxSize/particleNumber;
}

void ParticleSystem::update(FlowField &temp) { //Fix to use pointers
    for(int z = 0; z < temp.cellsZ; z++) {
        for(int y = 0; y < temp.cellsY; y++) {
            for(int x = 0; x < temp.cellsX; x++) {
                if(temp.cells[x][y][z].heading.length() > particleSense) {
                    particleLoc.push_back(ofVec3f(x * particleSpacing, y * particleSpacing, z * particleSpacing));
                    particleAcc.push_back(temp.cells[x][y][z].heading);
                    particleVel.push_back(temp.cells[x][y][z].heading);
                }
            }
        }
    }
    for(int i = 0; i < particleLoc.size(); i++) {
        particleAcc[i] = temp.getValue(particleLoc[i].x, particleLoc[i].y, particleLoc[i].z);
        particleVel[i].operator-=(particleAcc[i]);
        particleVel[i].limit(particleSpeedLimit);
        particleLoc[i].operator+=(particleVel[i]);
        particleVel[i].operator*=(particleSlowdown);
        if(particleLoc[i].x > particleBoxSize || particleLoc[i].x < 0 || particleLoc[i].y > particleBoxSize || particleLoc[i].y < 0 || particleLoc[i].z > particleBoxSize || particleLoc[i].z < 0 || particleVel[i].length() < particleKillLimit) {
            particleLoc.erase(particleLoc.begin()+i);
            particleAcc.erase(particleAcc.begin()+i);
            particleVel.erase(particleVel.begin()+i);
        }
    }
    
    mesh.clear();
    for(int i = 0; i < particleLoc.size(); i++) {
        ofColor colour;
        if(particleVel[i].length() < particleSpeedLimit/4) {
            colour = ofColor::indigo;
        }
        else if(particleVel[i].length() >= particleSpeedLimit/4 && particleVel[i].length() <= (particleSpeedLimit/4)*2) {
            colour = ofColor::red;
        }
        else {
            colour = ofColor::yellow;
        }
        mesh.addVertex(particleLoc[i]);
        mesh.addColor(colour);
        colour.a = ofMap(particleVel[i].length(), particleKillLimit, particleSpeedLimit, 0, 255);
        bool twoPoints = false;
        int previousPoint;
        for(int t = 0; t < particleLoc.size(); t++) {
            if(t != 1 &&particleLoc[i].distance(particleLoc[t]) < 80) {
                if(!twoPoints) {
                    previousPoint = t;
                    twoPoints = true;
                }
                else {
                    mesh.addIndex(i);
                    mesh.addIndex(t);
                    mesh.addIndex(previousPoint);
                    twoPoints = false;
                }
            }
        }
    }
}

void ParticleSystem::checkBorder() { //Redundant
    for(int i = 0; i < particleLoc.size(); i++) {
        if(particleLoc[i].x > particleBoxSize) {
            particleLoc[i].x = 0;
        }
        else if(particleLoc[i].x < 0) {
            particleLoc[i].x = particleBoxSize;
        }
        else if(particleLoc[i].y > particleBoxSize) {
            particleLoc[i].y = 0;
        }
        else if(particleLoc[i].y < 0) {
            particleLoc[i].y = particleBoxSize;
        }
        else if(particleLoc[i].z > particleBoxSize) {
            particleLoc[i].z = 0;
        }
        else if(particleLoc[i].z < 0) {
            particleLoc[i].z = particleBoxSize;
        }
    }
}

void ParticleSystem::draw(int x, int y) { //Fix magic numbers
    buffer.begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofClear(0);
    ofPushMatrix();
//    ofTranslate(320, 0, -320);
//    ofRotateY(rot);
//    ofTranslate(-320, 0, 320);
    ofTranslate((buffer.getWidth()/2) - (particleBoxSize/2), 0, -(particleBoxSize/2));
    mesh.draw();
    ofPopMatrix();
    buffer.end();
    buffer.draw(x, y);
}
