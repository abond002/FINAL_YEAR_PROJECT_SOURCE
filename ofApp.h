#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "KinectData.h"
#include "FlowField.h"
#include "ParticleSystem.h"
#include "GeneticAlgorithm.h"
#include "MarkovChain.h"
#include "ShapeBuilder.h"
#include "OrbitBuilder.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void keyPressed(int key);
    
    KinectData kinectdata;
    
    FlowField flowfield;
    
    ParticleSystem particles;
    
    ofxToggle cellDamping, cellNormalise, drawKinectData, drawFlowfieldData, drawParticles, drawGA, drawMC;
    
    ofxFloatSlider cellSlowdown, cellSense, particleSlowdown, particleSense, particleKillLimit, particleSpeedLimit, orbitBuilderY, orbitBuilderXSmall,orbitBuilderXMedium, orbitBuilderXBig;
    
    ofxPanel gui;
    
    bool debug;
    
    GeneticAlgorithm ga;
    
    MarkovChain markovChain;
    
    ShapeBuilder shapeBuilder;
    
    OrbitBuilder orbitBuilder;
    
    int timeOut, currentSequencePos;
    
    int stage;
		
};
