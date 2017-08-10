#include "MarkovChain.h"

MarkovChain::MarkovChain() {
    tolerance = 35;
    numOfIterations = 400;
    currentIteration = 0;
    numOfGenerations = 10;
    currentGeneration = 0;
    generatedClass = 0;
    currentSeqPos = 0;
    lerping = 0;
    previousSeqPos = 0;
    output.setup(480,480,480, 20);
    next.setup(480,480,480, 20);
    sequenceLength = 5;
    lerpAmount = 0.1;
    currentFlowField = 0;
    
    
    mcBuffer.allocate(480,480, GL_RGBA);
}

void MarkovChain::setup(int tNumOfClasses) {
    numOfClasses = tNumOfClasses;
    numOfHits.clear();
    flowFieldSequence.clear();
    classifiers.clear();
    sequence.clear();
    complete = false;
    currentIteration = 0;
}

bool MarkovChain::addToSequence(FlowField tFlowField) {
    int activatedCells = 0;
    for(int z = 0; z < tFlowField.cellsZ; z++) {
        for(int y = 0; y < tFlowField.cellsY; y++) {
            for(int x = 0; x < tFlowField.cellsX; x++) {
                if(tFlowField.cells[x][y][z].activated && tFlowField.cells[x][y][z].heading.length() > 3) {
                    activatedCells++;
                }
            }
        }
    }
    
    if(activatedCells > 3) {
        if(flowFieldSequence.size() < 200) {
            flowFieldSequence.push_back(tFlowField);
        }
        else {
            flowFieldSequence.push_back(tFlowField);
            flowFieldSequence.pop_front();
        }
        return true;
    }
    else{
        return false;
    }
}

void MarkovChain::update() {
    
    if(currentIteration < numOfIterations && !complete) {
        for(int i = 0; i < flowFieldSequence.size(); i++) {
            int activatedCells = 0;
            deque<ofVec3f> tClassifier;
            deque<ofVec3f> tPosition;
            for(int z = 0; z < flowFieldSequence[i].cellsZ; z++) {
                for(int y = 0; y < flowFieldSequence[i].cellsY; y++) {
                    for(int x = 0; x < flowFieldSequence[i].cellsX; x++) {
                        if(flowFieldSequence[i].cells[x][y][z].activated && flowFieldSequence[i].cells[x][y][z].heading.length() > 3) {
                            activatedCells++;
                            tClassifier.push_back(flowFieldSequence[i].cells[x][y][z].heading);
                            tPosition.push_back(ofVec3f(x, y, z));
                        }
                    }
                }
            }
            
            deque<int> targetClassifier;
            for(int i = 0; i < classifiers.size(); i++) {
                if(tClassifier.size() == classifiers[i].headings.size()) {
                    targetClassifier.push_back(i);
                    cout << "TempTarget: " << tClassifier.size() << " TargetClass: " << classifiers[i].headings.size() << "\n";
                }
            }
            
                if(tClassifier.size() > 3 && classifiers.size() > 1 && targetClassifier.size() > 1) {
                    
                    for(int t = 0; t < targetClassifier.size(); t++) {
                        for(int i = 0; i < tClassifier.size(); i++) {
                            if(tClassifier[i].align(classifiers[targetClassifier[t]].headings[i], tolerance)) {
                                classifiers[targetClassifier[t]].matches++;
                            }
                        }
                    }
                    
                    int highestMatches = 0;
                    int bestFit = 0;
                    for(int i = 0; i < targetClassifier.size(); i++) {
                        if(classifiers[targetClassifier[i]].matches > highestMatches && classifiers[targetClassifier[i]].matches > 0) {
                            highestMatches = classifiers[targetClassifier[i]].matches;
                            bestFit = targetClassifier[i];
                        }
                    }
                    
                    numOfHits[bestFit]++;
                    
                    for(int i = 0; i < tClassifier.size(); i++) {
                        classifiers[bestFit].headings[i].interpolate(tClassifier[i], 0.2);
                        classifiers[bestFit].position[i].interpolate(tPosition[i], 0.2);
                    }
                    cout << "Best fit: " << bestFit << "\n";
                }
                else if(numOfClasses > classifiers.size() && tClassifier.size() > 3) {
                    Classifier toPush;
                    toPush.headings = tClassifier;
                    toPush.position = tPosition;
                    classifiers.push_back(toPush);
                    numOfHits.push_back(0);
                    cout << "Number of Classes: " << classifiers.size() << "\n";
                }
            currentIteration++;
            if(tolerance > 20) {
                tolerance-=0.01;
            }
        }
    }
    else if(numOfIterations <= currentIteration && !complete){
        sequenceLength = flowFieldSequence.size()/10;
        getNewSequence();
        complete = true;
    }
}

void MarkovChain::updateFlowField() {
    
    if(classifiers.size() > 1) {
        
        if(sequence.size() < sequenceLength) {
            getNewSequence();
        }
        
        if(lerping < 1) {
            for(int z = 0; z < output.cellsZ; z++) {
                for(int y = 0; y < output.cellsY; y++) {
                    for(int x = 0; x < output.cellsX; x++) {
                        output.cells[x][y][z].heading.interpolate(next.cells[x][y][z].heading, lerpAmount);
                    }
                }
            }
            
            lerping += lerpAmount;
        }
        else {
            lerping = 0;
            next.generateFlowField();
            
            if(currentSeqPos < sequence.size() - 1) {
                currentSeqPos++;
            }
            else {
                getNewSequence();
                currentSeqPos = 0;
            }
        }
    }
}

void MarkovChain::draw(int x, int y) {
    if(classifiers.size() > 1) {
        output.draw(x,y);
    }
}

void MarkovChain::getNewSequence() {
    sequence.clear();
    int maxHits = 0;
    for(int i = 0; i < classifiers.size(); i++) {
        if(maxHits < numOfHits[i]) {
            maxHits = numOfHits[i];
        }
    }
    generatedClass = 0;
    while(sequence.size() < sequenceLength) {
        int selector = ofRandom(maxHits * classifiers.size());
        int cumulativeProb = 0;
        bool newVal = true;
        for (int i = 0; i < classifiers.size(); i++) {
            cumulativeProb += numOfHits[i];
            if(selector <= cumulativeProb && newVal) {
                cout << "Next sequence ID: " << i << "\n";
                sequence.push_back(classifiers[i]);
                newVal = false;
            }
        }
    
    }
}

int MarkovChain::returnCurrentSeq() {
    
}
