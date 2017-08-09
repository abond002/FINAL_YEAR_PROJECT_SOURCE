#include "ofMain.h"
#include "Cell.h"
#include "FlowField.h"

class GeneticAlgorithm {
public:
    
    GeneticAlgorithm();
    
    void setup(FlowField tTarget, int tPoolSize, int tIterations, int tGenerations);
    
    void update();
    
    void drawPool();
    
    void clearParticles();
    
    void drawBestFit(int popToDraw, int locX, int locY);
    
    int numOfPop;
    
    int numOfIterations;
    
    int numOfGenerations;
    
    int currentIteration;
    
    int currentGeneration;
    
    
    FlowField newFlowField;
    FlowField targetFlowField;
    FlowField bestFit;
    
    vector<FlowField> pool;
    
    vector<int> scores;
    
    int particleLifespan;
    vector<vector<ofVec3f>> particles, velocity, acceleration;
    vector<vector<bool>> alreadyHit;
    vector<vector<int>> pLifespan;
    
    int particleBoxSize;
    
    float mutationRange;
    float speedLimit;
    
    ofFbo poolBuffer;
    bool getNewFlowField;
    
};
