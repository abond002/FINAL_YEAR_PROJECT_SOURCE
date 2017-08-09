#include "Cell.h"

Cell::Cell() {
    activated = false;
    heading = ofVec3f(0,0,0);
}

void Cell::setup() {
    
}

void Cell::update() {
    activated = false;
    
    if(cellDamping) {
        heading.operator*=(cellSlowdown);
    }
    else {
        heading = ofVec3f(0,0,0);
    }
    
    if(heading.length() < 0.1) {
        heading = ofVec3f(0,0,0);
    }

    if(currentPoints.size() < 1 && newPoints.size() > 0) {
        currentPoints = newPoints;
        newPoints.clear();
    }
    else if(currentPoints.size() > 0 && newPoints.size() > 0) {
        activated = true;
        ofVec3f *tempPointer;
        ofVec3f averageCurrent;
        ofVec3f averageNew;
        
        tempPointer = currentPoints.data();
        averageCurrent.average(tempPointer, currentPoints.size());
        
        tempPointer = newPoints.data();
        averageNew.average(tempPointer, newPoints.size());
        
        if(averageCurrent.distance(averageNew) > cellSense) {
        
            heading = averageCurrent - averageNew;
            
            if(cellNormalise) {
                heading.normalize();
            }
            
        }
        
        currentPoints.clear();
        
    }
    
    
}

void Cell::addToCell(ofVec3f newPoint) {
    newPoints.push_back(newPoint);
}
