#pragma once
#include "ofMain.h"
#include "FlowField.h"
#include "Classifier.h"

class MarkovChain {
public:
    
    MarkovChain();
    void setup(int tNumOfClasses);
    bool addToSequence(FlowField tFlowField);
    void update();
    void updateFlowField();
    void draw(int x, int y);
    void getNewSequence();
    int returnCurrentSeq();
    
    vector<Classifier> classifiers;
    vector<int> numOfHits;
    vector<float> tempProb;
    
    deque<FlowField> flowFieldSequence;
    
    int currentFlowField;
    
    int numOfClasses;
    
    float tolerance;
    
    int numOfIterations;
    int currentIteration;
    
    int numOfGenerations;
    int currentGeneration;
    
    int generatedClass;
    
    deque<Classifier> sequence;
    int sequenceLength;
    int currentSeqPos;
    int previousSeqPos;
    float lerping;
    float lerpAmount;
    FlowField output;
    FlowField next;
    bool nextStep;
    
    bool complete;
    
    ofFbo mcBuffer;
};
