#pragma once
#include "ofMain.h"

class Cell {
public:
    
    Cell();
    void setup();
    void update();
    void addToCell(ofVec3f newPoint);
    
    vector<ofVec3f> currentPoints;
    vector<ofVec3f> newPoints;
    
    ofVec3f heading;
    
    bool activated;
    
    bool cellDamping;
    bool cellNormalise;
    
    float cellSense, cellSlowdown;
};
