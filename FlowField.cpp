#include "FlowField.h"

FlowField :: FlowField() {
    startingX = 80; //Fix this later
    rot = 0;
    currentFrame = 0;
    framesToRecord = 0;
    averageMotionBuffer.allocate(640, 480, GL_RGBA);
    averageMotionSteps = 500;
    currentAverageMotionStep = 0;
    drawBuffer.allocate(480, 480, GL_RGBA);
}

void FlowField::setup(int width, int height, int depth, int cellNum) {
    //Initialise the flow field grid
    sizeX = width;
    sizeY = height;
    sizeZ = depth;
    
    spacingX = sizeX/cellNum;
    spacingY = sizeY/cellNum;
    spacingZ = sizeZ/cellNum;
    cellsX = cellNum;
    cellsY = cellNum;
    cellsZ = cellNum;
    generateFlowField();
}

void FlowField::generateFlowField() {
    //Clear the volume and put new cells
    cells.clear();
    for(int z = 0; z < cellsZ; z++) {
        vector<vector<Cell>> layer;
        for(int y = 0; y < cellsY; y++) {
            vector<Cell> row;
            for(int x = 0; x < cellsX; x++) {
                Cell t;
                row.push_back(t);
            }
            layer.push_back(row);
        }
        cells.push_back(layer);
    }
}

void FlowField::generateNoise() {
    cells.clear();
    for(int z = 0; z < cellsZ; z++) {
        vector<vector<Cell>> layer;
        for(int y = 0; y < cellsY; y++) {
            vector<Cell> row;
            for(int x = 0; x < cellsX; x++) {
                Cell t;
                t.heading = ofVec3f(ofRandom(-0.1, 0.1), ofRandom(-0.1,0.1), ofRandom(-0.1,0.1));
                row.push_back(t);
            }
            layer.push_back(row);
        }
        cells.push_back(layer);
    }
}

void FlowField::update(vector<ofVec3f> allPoints) {
    
    for(ofVec3f t : allPoints) {
        int locX = floor((t.x - startingX) / spacingX);
        int locY = floor(t.y / spacingY);
        int locZ = floor(t.z / spacingX);
        cells[locX][locY][locZ].addToCell(t);
    }
    
    for(int z = 0; z < cellsZ; z++) {
        for(int y = 0; y < cellsY; y++) {
            for(int x = 0; x < cellsX; x++) {
                cells[x][y][z].update();
            }
        }
    }
    
    if(record && currentFrame < framesToRecord) {
        if(currentFrame == 0) {
            recordedCells.clear();
        }
        if(currentFrame == framesToRecord) {
            record = false;
            currentFrame = 0;
        }
        recordedCells.push_back(cells);
    }
    
}

void FlowField::draw(int locX, int locY) {
    drawBuffer.begin();
    ofClear(0);
    ofPushMatrix();
    ofTranslate(0,0, -480);
    for(int z = 0; z < cellsZ; z++) {
        for(int y = 0; y < cellsY; y++) {
            for(int x = 0; x < cellsX; x++) {
                ofVec3f pos = ofVec3f((spacingX*x)+(spacingX/2), (spacingY*y)+(spacingY/2), (spacingZ*z)+(spacingZ/2));
                ofVec3f head = pos + (cells[x][y][z].heading);
                ofSetColor(255);
                if(cells[x][y][z].activated) {
                    ofDrawCircle(pos.x, pos.y, pos.z, 1);
                }
                ofDrawLine(pos.x, pos.y, pos.z, head.x, head.y, head.z);
            }
        }
    }
    ofPopMatrix();
    drawBuffer.end();
    drawBuffer.draw(locX, locY);
}

void FlowField::drawAverageMotion(int locX, int locY) {
    ofVec3f averageFlowFieldMotion;
    vector<ofVec3f> allFlowFieldHeadings;
    for(int z = 0; z < cellsZ; z++) {
        for(int y = 0; y < cellsY; y++) {
            for(int x = 0; x < cellsX; x++) {
                allFlowFieldHeadings.push_back(cells[x][y][z].heading);
            }
        }
    }
    averageFlowFieldMotion.average(allFlowFieldHeadings.data(), allFlowFieldHeadings.size());
    averageFlowFieldMotion.normalize();
    
    float drawSpacingX = averageMotionBuffer.getWidth() / averageMotionSteps;
    
    if(sampling.size() < 5) {
        sampling.push_back(averageFlowFieldMotion);
    }
    
    averageMotionBuffer.begin();
    if(currentAverageMotionStep < averageMotionSteps && sampling.size() >= 5) {
        sampleAverage.average(sampling.data(), sampling.size());
        sampling.clear();
        ofSetColor(255, 0, 0);
        ofDrawLine(currentAverageMotionStep * drawSpacingX, averageMotionBuffer.getHeight()/2 + (previousState.x * (averageMotionBuffer.getHeight()/5)), (currentAverageMotionStep + 1) * drawSpacingX, averageMotionBuffer.getHeight()/2 + (sampleAverage.x * (averageMotionBuffer.getHeight()/5)));
        
        ofSetColor(0, 255, 0);
        ofDrawLine(currentAverageMotionStep * drawSpacingX, averageMotionBuffer.getHeight()/2 + (previousState.y * (averageMotionBuffer.getHeight()/5)), (currentAverageMotionStep + 1) * drawSpacingX, averageMotionBuffer.getHeight()/2 + (sampleAverage.y * (averageMotionBuffer.getHeight()/5)));
        
        ofSetColor(0, 0, 255);
        ofDrawLine(currentAverageMotionStep * drawSpacingX, averageMotionBuffer.getHeight()/2 + (previousState.z * (averageMotionBuffer.getHeight()/5)), (currentAverageMotionStep + 1) * drawSpacingX, averageMotionBuffer.getHeight()/2 + (sampleAverage.z * (averageMotionBuffer.getHeight()/5)));
        
        currentAverageMotionStep++;
        previousState = sampleAverage;
    }
    else if(currentAverageMotionStep >= averageMotionSteps){
        currentAverageMotionStep = 0;
        
        ofClear(0);
    }
    averageMotionBuffer.end();
    
    averageMotionBuffer.draw(locX, locY);
}

ofVec3f FlowField::getValue(int x, int y, int z) {
    int locX = floor(x / spacingX);
    int locY = floor(y / spacingY);
    int locZ = floor(z / spacingX);
    if(locX < cellsX && locY < cellsY && locZ < cellsZ) {
        return cells[locX][locY][locZ].heading;
    }
    
    
}


