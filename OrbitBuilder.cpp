#include "OrbitBuilder.h"

OrbitBuilder::OrbitBuilder() {
    
}

void OrbitBuilder::setup() {
    
    buffer.allocate(1920, 1080, GL_RGBA);
    
    spacingX = buffer.getWidth()/3;
    startingY = buffer.getHeight()/2;
    
    startingXSmall = spacingX/2;
    startingXMedium = spacingX * 2 + (spacingX/2);
    startingXBig = spacingX * 3 + (spacingX/2);
    
    radius = spacingX/2 - 200;
    
    smallShapeCurrentRot = 0;
    mediumShapeCurrentRot = TWO_PI/3;
    bigShapeCurrentRot = mediumShapeCurrentRot + TWO_PI/3;
    
    rotStep = 0.005;
    
    maxVertex = 3600;
    
    updateRate = 0.0006;
    
    minOrbitDist = 5;
    maxOrbitDist = 100;
    
    orbitRotRange = 0.05;
    
    smallShapeMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    smallShapeMesh.enableIndices();
    
    mediumShapeMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mediumShapeMesh.enableIndices();
    
    bigShapeMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    bigShapeMesh.enableIndices();
    
    for(int i = 0; i < 3; i++) {
        smallShapeTargetRot.push_back(ofRandom(-orbitRotRange, orbitRotRange));
        mediumShapeTargetRot.push_back(ofRandom(-orbitRotRange, orbitRotRange));
        bigShapeTargetRot.push_back(ofRandom(-orbitRotRange, orbitRotRange));
        
        smallShapeTargetDist.push_back(ofRandom(50, 100));
        mediumShapeTargetDist.push_back(ofRandom(50, 100));
        bigShapeTargetDist.push_back(ofRandom(50, 100));
        
        smallShapeOrbitsCurrentRot.push_back(ofRandom(TWO_PI));
        mediumShapeOrbitsCurrentRot.push_back(ofRandom(TWO_PI));
        bigShapeOrbitsCurrentRot.push_back(ofRandom(TWO_PI));
        
        smallShapeOrbitsRotStep.push_back(0);
        mediumShapeOrbitsRotStep.push_back(0);
        bigShapeOrbitsRotStep.push_back(0);
        
        smallShapeOrbitsDistance.push_back(0);
        mediumShapeOrbitsDistance.push_back(0);
        bigShapeOrbitsDistance.push_back(0);
        
        smallShapeOrbitsAxis.push_back(floor(ofRandom(0, 3)));
        mediumShapeOrbitsAxis.push_back(floor(ofRandom(0, 3)));
        bigShapeOrbitsAxis.push_back(floor(ofRandom(0, 3)));
    }
    
    //Core motion colours
    
    bigShapeColour.push_back(ofColor::indigo);
    bigShapeColour.push_back(ofColor::darkBlue);
    bigShapeColour.push_back(ofColor::dimGray);
    
    //Medium motion colours
    
    mediumShapeColour.push_back(ofColor::fuchsia);
    mediumShapeColour.push_back(ofColor::teal);
    mediumShapeColour.push_back(ofColor::crimson);
    
    //Swift motion colours
    
    smallShapeColour.push_back(ofColor::orangeRed);
    smallShapeColour.push_back(ofColor::gold);
    smallShapeColour.push_back(ofColor::peachPuff);
    
}

void OrbitBuilder::update() {
    
    float smallShapeX = sin(smallShapeCurrentRot) * radius;
    float smallShapeY = cos(smallShapeCurrentRot) * radius;
    
    float mediumShapeX = sin(mediumShapeCurrentRot) * radius;
    float mediumShapeY = cos(mediumShapeCurrentRot) * radius;
    
    float bigShapeX = sin(bigShapeCurrentRot) * radius;
    float bigShapeY = cos(bigShapeCurrentRot) * radius;
    
    if(smallShapeMesh.getNumVertices() < 1) {
        for(int i = 0; i < smallShapeOrbitsCurrentRot.size(); i++) {
            if(smallShapeOrbitsAxis[i] == 0) {
                smallShapeMesh.addVertex(ofVec3f(sin(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeX, cos(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeY, 0));
            }
            else if(smallShapeOrbitsAxis[i] == 1) {
               smallShapeMesh.addVertex(ofVec3f(smallShapeX, cos(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeY, sin(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i]));
            }
            else {
                smallShapeMesh.addVertex(ofVec3f(sin(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeX, smallShapeY, cos(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i]));
            }
            
            if(i == 0) {
                smallShapeMesh.addColor(smallShapeColour[i]);
            }
            else if(i == 1) {
                smallShapeMesh.addColor(smallShapeColour[i]);
            }
            else {
                smallShapeMesh.addColor(smallShapeColour[i]);
            }
        }
    }
    else {
        for(int i = 0; i < smallShapeOrbitsCurrentRot.size(); i++) {
            if(smallShapeOrbitsAxis[i] == 0) {
                smallShapeMesh.addVertex(ofVec3f(sin(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeX, cos(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeY, 0));
            }
            else if(smallShapeOrbitsAxis[i] == 1) {
                smallShapeMesh.addVertex(ofVec3f(smallShapeX, cos(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeY, sin(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i]));
            }
            else {
                smallShapeMesh.addVertex(ofVec3f(sin(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i] + smallShapeX, smallShapeY, cos(smallShapeOrbitsCurrentRot[i]) * smallShapeOrbitsDistance[i]));
            }
            if(i == 0) {
                smallShapeMesh.addColor(smallShapeColour[i]);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices());
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-3);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-2);
            }
            else if(i == 1) {
                
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-3);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices());
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-1);
                smallShapeMesh.addColor(smallShapeColour[i]);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices());
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-3);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-2);
            }
            else {
                
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-3);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices());
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-1);
                smallShapeMesh.addColor(smallShapeColour[i]);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices());
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-3);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-2);
                
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-3);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-2);
                smallShapeMesh.addIndex(smallShapeMesh.getNumVertices()-5);
                
            }
        }
    }
    
    if(smallShapeMesh.getNumVertices() > maxVertex) {
        smallShapeMesh.removeVertex(0);
        smallShapeMesh.removeVertex(1);
        smallShapeMesh.removeVertex(2);
    }
    
    if(mediumShapeMesh.getNumVertices() < 1) {
        for(int i = 0; i < mediumShapeOrbitsCurrentRot.size(); i++) {
            if(mediumShapeOrbitsAxis[i] == 0) {
                mediumShapeMesh.addVertex(ofVec3f(sin(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeX, cos(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeY, 0));
            }
            else if(mediumShapeOrbitsAxis[i] == 1) {
                mediumShapeMesh.addVertex(ofVec3f(mediumShapeX, cos(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeY, sin(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i]));
            }
            else {
                mediumShapeMesh.addVertex(ofVec3f(sin(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeX, mediumShapeY, cos(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i]));
            }
            
            if(i == 0) {
                mediumShapeMesh.addColor(mediumShapeColour[i]);
            }
            else if(i == 1) {
                mediumShapeMesh.addColor(mediumShapeColour[i]);
            }
            else {
                mediumShapeMesh.addColor(mediumShapeColour[i]);
            }
        }
    }
    else {
        for(int i = 0; i < mediumShapeOrbitsCurrentRot.size(); i++) {
            if(mediumShapeOrbitsAxis[i] == 0) {
                mediumShapeMesh.addVertex(ofVec3f(sin(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeX, cos(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeY, 0));
            }
            else if(mediumShapeOrbitsAxis[i] == 1) {
                mediumShapeMesh.addVertex(ofVec3f(mediumShapeX, cos(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeY, sin(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i]));
            }
            else {
                mediumShapeMesh.addVertex(ofVec3f(sin(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i] + mediumShapeX, mediumShapeY, cos(mediumShapeOrbitsCurrentRot[i]) * mediumShapeOrbitsDistance[i]));
            }
            
            if(i == 0) {
                mediumShapeMesh.addColor(mediumShapeColour[i]);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices());
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-3);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-2);
            }
            else if(i == 1) {
                
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-3);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices());
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-1);
                mediumShapeMesh.addColor(mediumShapeColour[i]);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices());
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-3);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-2);
            }
            else {
                
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-3);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-1);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices());
                mediumShapeMesh.addColor(mediumShapeColour[i]);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices());
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-3);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-2);
                
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-3);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-2);
                mediumShapeMesh.addIndex(mediumShapeMesh.getNumVertices()-5);
                
            }
        }
    }
    
    if(mediumShapeMesh.getNumVertices() > maxVertex) {
        mediumShapeMesh.removeVertex(0);
        mediumShapeMesh.removeVertex(1);
        mediumShapeMesh.removeVertex(2);
    }
    
    if(bigShapeMesh.getNumVertices() < 1) {
        for(int i = 0; i < bigShapeOrbitsCurrentRot.size(); i++) {
            if(bigShapeOrbitsAxis[i] == 0) {
                bigShapeMesh.addVertex(ofVec3f(sin(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeX, cos(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeY, 0));
            }
            else if(bigShapeOrbitsAxis[i] == 1) {
                bigShapeMesh.addVertex(ofVec3f(bigShapeX, cos(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeY, sin(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i]));
            }
            else {
                bigShapeMesh.addVertex(ofVec3f(sin(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeX, bigShapeY, cos(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i]));
            }
            
            if(i == 0) {
                bigShapeMesh.addColor(bigShapeColour[i]);
            }
            else if(i == 1) {
                bigShapeMesh.addColor(bigShapeColour[i]);
            }
            else {
                bigShapeMesh.addColor(bigShapeColour[i]);
            }
        }
    }
    else {
        for(int i = 0; i < bigShapeOrbitsCurrentRot.size(); i++) {
            if(bigShapeOrbitsAxis[i] == 0) {
                bigShapeMesh.addVertex(ofVec3f(sin(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeX, cos(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeY, 0));
            }
            else if(bigShapeOrbitsAxis[i] == 1) {
                bigShapeMesh.addVertex(ofVec3f(bigShapeX, cos(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeY, sin(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i]));
            }
            else {
                bigShapeMesh.addVertex(ofVec3f(sin(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i] + bigShapeX, bigShapeY, cos(bigShapeOrbitsCurrentRot[i]) * bigShapeOrbitsDistance[i]));
            }
            
            if(i == 0) {
                bigShapeMesh.addColor(bigShapeColour[i]);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices());
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-3);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-2);
            }
            else if(i == 1) {
                
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-3);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices());
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-1);
                bigShapeMesh.addColor(bigShapeColour[i]);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices());
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-3);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-2);
            }
            else {
                
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-3);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-1);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices());
                bigShapeMesh.addColor(bigShapeColour[i]);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices());
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-3);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-2);
                
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-3);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-2);
                bigShapeMesh.addIndex(bigShapeMesh.getNumVertices()-5);
                
            }
        }
    }
    
    if(bigShapeMesh.getNumVertices() > maxVertex) {
        bigShapeMesh.removeVertex(0);
        bigShapeMesh.removeVertex(1);
        bigShapeMesh.removeVertex(2);
    }
    
    for(int i = 0; i < mediumShapeMesh.getNumVertices(); i+=3) {
        float a = ofMap(i, 0, bigShapeMesh.getNumVertices(), 0, 255);
        
        for(int c = 0; c < smallShapeColour.size(); c++) {
            smallShapeColour[c].a = a;
            mediumShapeColour[c].a = a;
            bigShapeColour[c].a = a;
        }
        
        smallShapeMesh.setColor(i, smallShapeColour[0]);
        smallShapeMesh.setColor(i+1, smallShapeColour[1]);
        smallShapeMesh.setColor(i+2, smallShapeColour[2]);
        
        mediumShapeMesh.setColor(i, mediumShapeColour[0]);
        mediumShapeMesh.setColor(i+1, mediumShapeColour[1]);
        mediumShapeMesh.setColor(i+2, mediumShapeColour[2]);
        
        bigShapeMesh.setColor(i, bigShapeColour[0]);
        bigShapeMesh.setColor(i+1, bigShapeColour[1]);
        bigShapeMesh.setColor(i+2, bigShapeColour[2]);
    }
    
    for(int i = 0; i < smallShapeOrbitsCurrentRot.size(); i++) {
        smallShapeOrbitsCurrentRot[i] += smallShapeOrbitsRotStep[i];
        mediumShapeOrbitsCurrentRot[i] += mediumShapeOrbitsRotStep[i];
        bigShapeOrbitsCurrentRot[i] += bigShapeOrbitsRotStep[i];
        
        smallShapeOrbitsRotStep[i] = ofLerp(smallShapeOrbitsRotStep[i], smallShapeTargetRot[i], updateRate*10);
        mediumShapeOrbitsRotStep[i] = ofLerp(mediumShapeOrbitsRotStep[i], mediumShapeTargetRot[i], updateRate*10);
        bigShapeOrbitsRotStep[i] = ofLerp(bigShapeOrbitsRotStep[i], bigShapeTargetRot[i], updateRate*10);
        
        smallShapeOrbitsDistance[i] = ofLerp(smallShapeOrbitsDistance[i], smallShapeTargetDist[i], updateRate*10);
        mediumShapeOrbitsDistance[i] = ofLerp(mediumShapeOrbitsDistance[i], mediumShapeTargetDist[i], updateRate*10);
        bigShapeOrbitsDistance[i] = ofLerp(bigShapeOrbitsDistance[i], bigShapeTargetDist[i], updateRate*10);
        
        smallShapeTargetRot[i] = ofLerp(smallShapeTargetRot[i], rotStep, updateRate);
        mediumShapeTargetRot[i] = ofLerp(mediumShapeTargetRot[i], rotStep, updateRate);
        bigShapeTargetRot[i] = ofLerp(bigShapeTargetRot[i], rotStep, updateRate);
        
        smallShapeTargetDist[i] = ofLerp(smallShapeTargetDist[i], minOrbitDist, updateRate*3);
        mediumShapeTargetDist[i] = ofLerp(mediumShapeTargetDist[i], minOrbitDist, updateRate*3);
        bigShapeTargetDist[i] = ofLerp(bigShapeTargetDist[i], minOrbitDist, updateRate*3);
    }
    
    smallShapeCurrentRot += rotStep;
    mediumShapeCurrentRot += rotStep;
    bigShapeCurrentRot += rotStep;
}

void OrbitBuilder::draw(int x, int y) {
    buffer.begin();
    ofClear(0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofPushMatrix();
    ofTranslate(startingXSmall,startingY);
//    ofRotateY(smallShapeCurrentRot*4);
    smallShapeMesh.draw();
    ofPopMatrix();
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    ofPushMatrix();
    ofTranslate(startingXMedium, startingY);
//    ofRotateY(mediumShapeCurrentRot*4);
    mediumShapeMesh.draw();
    ofPopMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPushMatrix();
    ofTranslate(startingXBig, startingY);
//    ofRotateY(bigShapeCurrentRot*4);
    bigShapeMesh.draw();
    ofPopMatrix();
    buffer.end();
    buffer.draw(x, y);
    
}

void OrbitBuilder::getNewValues(Classifier tempClassifier) {
    vector<ofVec3f> tempHeading;
    tempHeading.assign(tempClassifier.headings.begin(), tempClassifier.headings.end());
    
    ofVec3f averageHeading;
    averageHeading.average(tempHeading.data(), tempHeading.size());
    
    int targetShape;
    
    if(averageHeading.length() > 1 && averageHeading.length() < 2) {
        targetShape = 0;
    }
    else if(averageHeading.length() > 2 && averageHeading.length() < 4) {
        targetShape = 1;
    }
    else {
        targetShape = 2;
    }
    
    for(int i = 0; i < 3; i++) {
        int orbitAxis;
        float rotationAmount;
        
        if(tempClassifier.headings[i].x > tempClassifier.headings[i].y && tempClassifier.headings[i].z) {
            orbitAxis = 0;
            rotationAmount = ofMap(tempClassifier.headings[i].x , -1, 1, -orbitRotRange, orbitRotRange);
        }
        else if(tempClassifier.headings[i].y > tempClassifier.headings[i].x && tempClassifier.headings[i].y > tempClassifier.headings[i].z) {
            orbitAxis = 1;
            rotationAmount = ofMap(tempClassifier.headings[i].y , -1, 1, -orbitRotRange, orbitRotRange);
        }
        else {
            orbitAxis = 2;
            rotationAmount = ofMap(tempClassifier.headings[i].z , -1, 1, -orbitRotRange, orbitRotRange);
        }
        
        if(targetShape == 0) {
            smallShapeOrbitsAxis[i] = orbitAxis;
            smallShapeTargetRot[i] = rotationAmount;
            smallShapeTargetDist[i] = maxOrbitDist;
        }
        else if(targetShape == 1) {
            mediumShapeOrbitsAxis[i] = orbitAxis;
            mediumShapeTargetRot[i] = rotationAmount;
            mediumShapeTargetDist[i] = maxOrbitDist;
        }
        else {
            bigShapeOrbitsAxis[i] = orbitAxis;
            bigShapeTargetRot[i] = rotationAmount;
            bigShapeTargetDist[i] = maxOrbitDist;
        }
    }
}
