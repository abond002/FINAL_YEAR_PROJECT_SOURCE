#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm() {
    mutationRange = 1;
    speedLimit = 2;
    getNewFlowField = false;
}

void GeneticAlgorithm::setup(FlowField tTarget, int tPoolSize, int tIterations, int tGenerations) {
    targetFlowField = tTarget;
    newFlowField = tTarget;
    numOfPop = tPoolSize;
    numOfIterations = tIterations;
    numOfGenerations = tGenerations;
    currentIteration = 0;
    currentGeneration = 0;
    for(int i = 0; i < numOfPop; i++) {
        FlowField temp;
        temp.setup(480, 480, 480, 20);
        temp.generateNoise();
        pool.push_back(temp);
        scores.push_back(0);
    }
    clearParticles();
    
    particleBoxSize = 470;
    poolBuffer.allocate(480, 480, GL_RGBA);
    particleLifespan = (tIterations/4) * 3;
}

void GeneticAlgorithm::update() {
    getNewFlowField = false;
    if(currentGeneration < numOfGenerations) {
        if(currentIteration < numOfIterations) {
            for(int f = 0; f < pool.size(); f++) {
                for(int i = 0; i < particles[f].size(); i++) {
                    acceleration[f][i] = pool[f].getValue(particles[f][i].x, particles[f][i].y, particles[f][i].z);
                    acceleration[f][i].operator*=(0.2);
                    velocity[f][i].operator+=(acceleration[f][i]);
                    velocity[f][i].limit(speedLimit);
                    particles[f][i].operator+=(velocity[f][i]);
                    if(particles[f][i].x > particleBoxSize || particles[f][i].x < 5 || particles[f][i].y > particleBoxSize || particles[f][i].y < 5 || particles[f][i].z > particleBoxSize || particles[f][i].z < 5 || pLifespan[f][i] < 1) {
                        particles[f].erase(particles[f].begin() +i);
                        velocity[f].erase(velocity[f].begin() +i);
                        acceleration[f].erase(acceleration[f].begin() +i);
                        alreadyHit[f].erase(alreadyHit[f].begin() +i);
                        pLifespan[f].erase(pLifespan[f].begin() +i);
                    }
                    int locX = floor(particles[f][i].x/targetFlowField.spacingX);
                    int locY = floor(particles[f][i].y/targetFlowField.spacingY);
                    int locZ = floor(particles[f][i].z/targetFlowField.spacingZ);
                    if(targetFlowField.cells[locX][locY][locZ].activated && !alreadyHit[f][i]) {
                        scores[f]++;
                        alreadyHit[f][i] = true;
                    }
                    if(!alreadyHit[f][i]) {
                        pLifespan[f][i]--;
                    }
                }
            }
            currentIteration++;
        }
    }
    
    if(currentIteration >= numOfIterations) {
        getNewFlowField = true;
        int activatedCellCount;
        for(int z = 0; z < newFlowField.cellsZ; z++) {
            for(int y = 0; y < newFlowField.cellsY; y++) {
                for(int x = 0; x < newFlowField.cellsX; x++) {
                    if(newFlowField.cells[x][y][z].activated) {
                        activatedCellCount++;
                    }
                }
            }
        }
        if(activatedCellCount > 20) {
            targetFlowField = newFlowField;
        }
        
        int firstHighest = 0;
        int secondHighest = 0;
        int candidate1;
        int candidate2;
        for(int i = 0; i < scores.size(); i++) {
            if(firstHighest < scores[i]) {
                firstHighest = scores[i];
                candidate1 = i;
            }
            else {
                candidate1 = 0;
            }
        }
        cout << firstHighest << " ";
        for(int i = 0; i < scores.size(); i++) {
            if(secondHighest < scores[i] && scores[i] != firstHighest) {
                secondHighest = scores[i];
                candidate2 = i;
            }
            else {
                candidate2 = 0;
            }
        }
        cout << secondHighest << "\n";
        
        FlowField mate1 = pool[candidate1];
        FlowField mate2 = pool[candidate2];
        FlowField newPop;
        newPop.setup(480, 480, 480, 20);
        
        vector<ofVec3f> cActivated;
        ofVec3f cActivatedAverage;
        for(int z = 0; z < mate1.cellsZ; z++) {
            for(int y = 0; y < mate1.cellsY; y++) {
                for(int x = 0; x < mate1.cellsZ; x++){
                    
                    if(targetFlowField.cells[x][y][z].activated) {
                        cActivated.push_back(ofVec3f(x * targetFlowField.spacingX + (targetFlowField.spacingX/2),y * targetFlowField.spacingY + (targetFlowField.spacingY/2),z * targetFlowField.spacingZ + (targetFlowField.spacingZ/2)));
                    }
                }
            }
        }
        
        if(cActivated.size() > 1) {
            cActivatedAverage.average(cActivated.data(), cActivated.size());
        }
        
        
        for(int z = 0; z < mate1.cellsZ; z++) {
            for(int y = 0; y < mate1.cellsY; y++) {
                for(int x = 0; x < mate1.cellsZ; x++){
                    if(!targetFlowField.cells[x][y][z].activated && cActivated.size() > 1) {
                    ofVec3f c1Loc = ofVec3f(x * mate1.spacingX + (mate1.spacingX/2), y * mate1.spacingY + (mate1.spacingY/2),z * mate1.spacingZ + (mate1.spacingZ/2));
                    float c1Distance = ofMap(c1Loc.distance(cActivatedAverage), 20, 480, 0, 0.4);
                    
                    c1Loc.operator-=(cActivatedAverage);
                    
                    //c1Loc.normalize();
                    c1Loc.limit(c1Distance);
                    
                    mate1.cells[x][y][z].heading.operator-=(c1Loc);
                    
                    ofVec3f c2Loc = ofVec3f(x * mate2.spacingX + (mate2.spacingX/2), y * mate2.spacingY + (mate2.spacingY/2),z * mate2.spacingZ + (mate2.spacingZ/2));
                    float c2Distance = ofMap(c2Loc.distance(cActivatedAverage), 20, 480, 0, 0.4);
                    
                    c2Loc.operator-=(cActivatedAverage);
                    
                    //c2Loc.normalize();
                    c2Loc.limit(c2Distance);
                    
                    mate2.cells[x][y][z].heading.operator-=(c2Loc);
                    }
                    newPop.cells[x][y][z].heading.interpolate(mate1.cells[x][y][z].heading, 0.5);
                    newPop.cells[x][y][z].heading.interpolate(mate2.cells[x][y][z].heading, 0.5);
                }
            }
        }
        
        
        
        pool.clear();
        bestFit = newPop;
        for(int i = 0; i < numOfPop; i++) {
            FlowField temp;
            temp.setup(480, 480, 480, 20);
            temp = newPop;
            for(int z = 0; z < temp.cellsZ; z++) {
                for(int y = 0; y < temp.cellsY; y++) {
                    for(int x = 0; x < temp.cellsZ; x++){
                        temp.cells[x][y][z].heading.operator+=(ofVec3f(ofRandom(-mutationRange,mutationRange),ofRandom(-mutationRange,mutationRange),ofRandom(-mutationRange,mutationRange)));
                    }
                }
            }
            pool.push_back(temp);
            scores[i] = 0;
        }
        
        clearParticles();
        currentIteration = 0;
        currentGeneration++;
    }
    
    if(currentGeneration >= numOfGenerations) {
            for(int i = 0; i < particles[0].size(); i++) {
                acceleration[0][i] = bestFit.getValue(particles[0][i].x, particles[0][i].y, particles[0][i].z);
                acceleration[0][i].operator*=(0.2);
                velocity[0][i].operator+=(acceleration[0][i]);
                velocity[0][i].limit(speedLimit);
                particles[0][i].operator+=(velocity[0][i]);
                if(particles[0][i].x > particleBoxSize || particles[0][i].x < 5 || particles[0][i].y > particleBoxSize || particles[0][i].y < 5 || particles[0][i].z > particleBoxSize || particles[0][i].z < 5 || pLifespan[0][i] < 1) {
                    particles[0].erase(particles[0].begin() +i);
                    velocity[0].erase(velocity[0].begin() +i);
                    acceleration[0].erase(acceleration[0].begin() +i);
                    alreadyHit[0].erase(alreadyHit[0].begin() +i);
                    pLifespan[0].erase(pLifespan[0].begin() +i);
                }
            }
            if(particles[0].size() < 1) {
                clearParticles();
            }
        }
}

void GeneticAlgorithm::clearParticles() {
    particles.clear();
    velocity.clear();
    acceleration.clear();
    alreadyHit.clear();
    pLifespan.clear();
    for(int f = 0; f < pool.size(); f++) {
        vector<ofVec3f> tParticles;
        vector<ofVec3f> tVelocity;
        vector<ofVec3f> tAcceleration;
        vector<bool> tAlreadyHit;
        vector<int> tpLifespan;
    for(int z = 0; z < targetFlowField.cellsZ; z++) {
        for(int y = 0; y < targetFlowField.cellsY; y++) {
            for(int x = 0; x < targetFlowField.cellsX; x++) {
                ofVec3f tParticle = ofVec3f(x*targetFlowField.spacingX+(targetFlowField.spacingX/2), y*targetFlowField.spacingY+(targetFlowField.spacingY/2), z*targetFlowField.spacingZ+(targetFlowField.spacingZ/2));
                
                tParticles.push_back(tParticle);
                tVelocity.push_back(ofVec3f(0,0,0));
                tAcceleration.push_back(ofVec3f(0,0,0));
                tAlreadyHit.push_back(false);
                tpLifespan.push_back(particleLifespan);
                
            }
        }
    }
        particles.push_back(tParticles);
        velocity.push_back(tVelocity);
        acceleration.push_back(tAcceleration);
        alreadyHit.push_back(tAlreadyHit);
        pLifespan.push_back(tpLifespan);
    }
}

void GeneticAlgorithm::drawBestFit(int popToDraw, int locX, int locY) {
    poolBuffer.begin();
    ofClear(0);
    ofPushMatrix();
    ofTranslate(0,0,-480);
    for(int p = 0; p < particles[popToDraw].size(); p++) {
        ofSetColor(255, ofMap(pLifespan[popToDraw][p], 0, particleLifespan, 0, 255));
        ofDrawCircle(particles[popToDraw][p].x, particles[popToDraw][p].y, particles[popToDraw][p].z, 1);
    }
    ofPopMatrix();
    poolBuffer.end();
    poolBuffer.draw(locX, locY);
}
