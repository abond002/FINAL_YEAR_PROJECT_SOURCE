#pragma once
#include "ofMain.h"
#include "Cell.h"

class FlowField {
    
public:
    
    //------ Methods -----
    
    FlowField();
    void setup(int width, int height, int depth, int cells);
    void update(vector<ofVec3f> allPoints);
    void generateFlowField();
    void generateNoise();
    void draw(int locX, int locY);
    void drawAverageMotion(int locX, int locY);
    ofVec3f getValue(int x, int y, int z);
    
    // ----- Variables ----
    
    int sizeX,sizeY,sizeZ;
    int cellsX, cellsY, cellsZ;
    int spacingX, spacingY, spacingZ;
    int startingX;
    float rot;
    
    bool record;
    int framesToRecord;
    int currentFrame;
    
    vector<vector<vector<Cell>>> cells;
    
    vector<vector<vector<vector<Cell>>>> recordedCells;
    
    //Drawing average motion across all cells
    ofFbo averageMotionBuffer;
    int averageMotionSteps;
    int currentAverageMotionStep;
    vector<ofVec3f> sampling;
    ofVec3f sampleAverage;
    ofVec3f previousState;
    
    ofFbo drawBuffer;
};

