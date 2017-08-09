#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    flowfield.setup(480, 480, 480, 20);
    particles.setup(1080, 20);
    
    gui.setup();
    gui.add(cellDamping.setup("Cell damping", false));
    gui.add(cellSlowdown.setup("Cell slowdown", 0.4, 0.0, 1.0));
    gui.add(cellSense.setup("Cell sense", 10.0, 0.0, 20.0));
    gui.add(cellNormalise.setup("Cell norm", false));
    gui.add(particleSpeedLimit.setup("Part Speed Lim", 3.0, 1.0, 10.0));
    gui.add(particleSlowdown.setup("Particle Slowdown", 0.92, 0.0, 1.0));
    gui.add(particleSense.setup("Particle sense", 1.0, 0.0, 2));
    gui.add(particleKillLimit.setup("Particle kill limit", 0.1, 0.0, 0.5));
    gui.add(drawKinectData.setup("Kinect Data", false));
    gui.add(drawFlowfieldData.setup("Draw Flowfield", false));
    gui.add(drawParticles.setup("Draw Particles", true));
    gui.add(drawGA.setup("Draw GA", false));
    gui.add(drawMC.setup("Draw MC", false));
    
    debug = false;
            
    ga.setup(flowfield, 5, 100, 1000);
    
    markovChain.setup(50);
    
    stage = 0;
    
    timeOut = 0;

    
    //ofSetFrameRate(24);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //GUI controls
    for(int z = 0; z < flowfield.cellsZ; z++) {
        for(int y = 0; y < flowfield.cellsY; y++) {
            for(int x = 0; x <flowfield.cellsX; x++) {
                flowfield.cells[x][y][z].cellDamping = cellDamping;
                flowfield.cells[x][y][z].cellSlowdown = cellSlowdown;
                flowfield.cells[x][y][z].cellSense = cellSense;
                flowfield.cells[x][y][z].cellNormalise = cellNormalise;
            }
        }
    }
    
    particles.particleSpeedLimit = particleSpeedLimit;
    particles.particleSlowdown = particleSlowdown;
    particles.particleSense = particleSense;
    particles.particleKillLimit = particleKillLimit;
    
    kinectdata.update();
    flowfield.update(kinectdata.getData());
    
    
    
    if(drawParticles) {
        particles.update(flowfield);
    }

    
    if(drawGA) {
        ga.update();
        if(ga.getNewFlowField) {
            ga.newFlowField = flowfield;
        }
    }
    
    switch(stage) {
            
        case 0 :
            if(markovChain.addToSequence(flowfield)) {
                timeOut = 0;
            }
            else {
                timeOut++;
            }
            
            if(timeOut > 50 && markovChain.flowFieldSequence.size() < 5) {
                timeOut = 0;
                markovChain.flowFieldSequence.clear();
            }
            else if(timeOut > 50 && markovChain.flowFieldSequence.size() > 5) {
                timeOut = 0;
                stage = 1;
            }
            
            cout << "Timeout: " << timeOut << "\n";
            
            break;
            
        case 1 :
            
            if(!markovChain.complete) {
                markovChain.update();
            }
            else {
                shapeBuilder.setup(markovChain.sequence);
                stage = 2;
            }
            
            break;
            
        case 2 :
            if(!shapeBuilder.complete) {
                shapeBuilder.update();
            }
            else {
                stage = 0;
                markovChain.setup(50);
            }
            
            break;
            
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(debug) {
        gui.draw();
    }
    if(drawKinectData) {
        kinectdata.draw(0,0);
    }
    if(drawFlowfieldData) {
        flowfield.draw(0,0);
    }
    if(drawParticles) {
        particles.draw(0,0);
    }
            
            //flowfield.drawAverageMotion(ofGetWidth()/2, 0);

    if(drawGA) {
        ga.drawBestFit(0, 0, 0);
        ga.drawBestFit(1, 480, 0);
        ga.drawBestFit(2, 0, 480);
        ga.drawBestFit(3, 480, 480);
    }
    
    if(drawMC) {
        markovChain.draw(640, 0);
    }
    
    if(stage == 2) {
        shapeBuilder.draw(0, 0);
    }
    
    
}

void ofApp::keyPressed(int key) {
    if(key == 'd') {
        debug = !debug;
    }
    if(key == 'h') {
        ofHideCursor();
    }
    if(key == 's') {
        ofShowCursor();
    }
}
