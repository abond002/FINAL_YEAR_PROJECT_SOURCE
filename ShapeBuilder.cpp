#include "ShapeBuilder.h"

ShapeBuilder::ShapeBuilder() {
    drawBuffer.allocate(1920,1080, GL_RGB);
    initialDistance = 40;
    startingX = 960;
    startingY = 540;
    globalRot = 0;
    viewRot = 0;
    lineLength = 1000;
    
    getNew = true;
    
    lerping = 0;
    lerpAmount = 0.01;
    currentShape = 0;
    
    smallShapeColour = ofColor::indigo;
    mediumShapeColour = ofColor::purple;
    bigShapeColour = ofColor::gold;
    
    smallShapeColour.a = 3;
    mediumShapeColour.a = 3;
    bigShapeColour.a = 3;
    
    smallMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    smallMesh.enableIndices();
    
    bigMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    bigMesh.enableIndices();
}

void ShapeBuilder::setup(deque<Classifier> tempClass) {
    lerping = 0;
    complete = false;
    currentShape = 0;
    lines.clear();
    for(int s = 0; s < 3; s++) {
        deque<int> tempLines;
        for(int i = 0; i < lineLength; i++) {
            tempLines.push_back(i);
        }
        lines.push_back(tempLines);
    }
    for(Classifier t : tempClass) {
        if(t.headings.size() > 1 && t.headings.size() < 5) {
            makeShape(t, 0);
        }
        else if(t.headings.size() > 5 && t.headings.size() < 10) {
            makeShape(t, 1);
        }
        else if(t.headings.size() > 10) {
            makeShape(t, 2);
        }
    }
}

void ShapeBuilder::makeShape(Classifier temp, int target) {
    float spacing;
    if(target == 0) {
        spacing = initialDistance;
    }
    else if(target == 1) {
        spacing = initialDistance * 2;
    }
    else {
        spacing = initialDistance * 3;
    }
    globalRot = 0;
    deque<ofVec3f> tempShape;
    rotSpeed.clear();
    currentRot.clear();
    distance.clear();
    rotationAxis.clear();
    for(int i = 0; i < 3; i++) {
        currentRot.push_back(0);
        if(i == 0) {
            distance.push_back(spacing);
        }
        else {
            distance.push_back(spacing/i);
        }
        
        if(temp.headings[i].x > temp.headings[i].y && temp.headings[i].x > temp.headings[i].z) {
            rotationAxis.push_back(0);
            rotSpeed.push_back(ofMap(temp.headings[i].x, -1, 1, -0.005, 0.005));
        }
        else if(temp.headings[i].y > temp.headings[i].x && temp.headings[i].y > temp.headings[i].z) {
            rotationAxis.push_back(1);
            rotSpeed.push_back(ofMap(temp.headings[i].y, -1, 1, -0.005, 0.005));
        }
        else if(temp.headings[i].z > temp.headings[i].x && temp.headings[i].z > temp.headings[i].y) {
            rotationAxis.push_back(2);
            rotSpeed.push_back(ofMap(temp.headings[i].z, -1, 1, -0.005, 0.005));
        }
    }
    globalRotStep = ofMap(rotSpeed.size(), 3, 10, 0.0002, 0.0001);
    
    
    ofVec3f currentLoc = ofVec3f(0,0,0);
    while (globalRot < TWO_PI) {
        ofVec3f previousLoc = ofVec3f(0,0,0);
        for(int i = 0; i < currentRot.size(); i++) {
            ofVec3f tempLoc ;
            
            float locX;
            float locY;
            float locZ;
            
            if(rotationAxis[i] == 0) {
                locX = sin(currentRot[i]) * distance[i];
                locY = cos(currentRot[i]) * distance[i];
            }
            else if(rotationAxis[i] == 1) {
                locY = sin(currentRot[i]) * distance[i];
                locZ = cos(currentRot[i]) * distance[i];
            }
            else if(rotationAxis[i] == 2) {
                locX = sin(currentRot[i]) * distance[i];
                locZ = cos(currentRot[i]) * distance[i];
            }
            
            if(i == 0) {
                tempLoc.operator=(ofVec3f(startingX + locX, startingY + locY, startingY + locZ));
                previousLoc.operator=(tempLoc);
            }
            else {
                tempLoc.operator=(ofVec3f(previousLoc.x + locX, previousLoc.y + locY, previousLoc.z + locZ));
                previousLoc.operator=(tempLoc);
            }
            
            if(i == currentRot.size()-1) {
                currentLoc.operator=(tempLoc);
            }
            
            currentRot[i] += rotSpeed[i];
        }
        tempShape.push_back(currentLoc);
        globalRot += globalRotStep;
        

    }
    if(target == 0) {
        smallShape.push_back(tempShape);
    }
    else if(target ==1) {
        mediumShape.push_back(tempShape);
    }
    else {
        bigShape.push_back(tempShape);
    }
}

void ShapeBuilder::update() {
    if(currentShape < smallShape.size() || currentShape < mediumShape.size() || currentShape < bigShape.size()) {
        if(lerping < 1) {
            if(smallShape.size() > 0) {
                if(currentShape < smallShape.size()) {
                    for(int i = 0; i < smallShape[0].size(); i++) {
                        smallShape[0][i].interpolate(smallShape[currentShape][i], lerpAmount);
                    }
                }
            }
            if(mediumShape.size() > 0) {
                if(currentShape < mediumShape.size()) {
                    for(int i = 0; i < mediumShape[0].size(); i++) {
                        mediumShape[0][i].interpolate(mediumShape[currentShape][i], lerpAmount);
                    }
                }
            }
            if(bigShape.size() > 0) {
                if(currentShape < bigShape.size()) {
                    for(int i = 0; i < bigShape[0].size(); i++) {
                        bigShape[0][i].interpolate(bigShape[currentShape][i], lerpAmount);
                    }
                }
            }
            lerping += lerpAmount;
        }
        else {
            lerping = 0;
            currentShape++;
        }
    }
    else {
        complete = true;
    }
    
    smallMesh.clear();
    bigMesh.clear();
    
    if(smallShape.size() > 0 || mediumShape.size() > 0 || bigShape.size() > 0) {
        if(smallShape.size() > 0 && mediumShape.size() > 0) {
            for(int i = 0; i < smallShape[0].size()-1; i++) {
                smallMesh.addVertex(smallShape[0][i]);
                smallMesh.addColor(smallShapeColour);
                smallMesh.addVertex(smallShape[0][i+1]);
                smallMesh.addColor(smallShapeColour);
                
                smallMesh.addVertex(mediumShape[0][i]);
                smallMesh.addColor(mediumShapeColour);
                smallMesh.addVertex(mediumShape[0][i+1]);
                smallMesh.addColor(mediumShapeColour);
                
                smallMesh.addIndex(i);
                smallMesh.addIndex(i+1);
                smallMesh.addIndex(i+2);
                
                smallMesh.addIndex(i+1);
                smallMesh.addIndex(i+2);
                smallMesh.addIndex(i+3);
            }
        }
        if(mediumShape.size() > 0 && bigShape.size() > 0) {
            for(int i = 0; i < mediumShape[0].size()-1; i++) {
                bigMesh.addVertex(mediumShape[0][i]);
                bigMesh.addColor(mediumShapeColour);
                bigMesh.addVertex(mediumShape[0][i+1]);
                bigMesh.addColor(mediumShapeColour);
                
                bigMesh.addVertex(bigShape[0][i]);
                bigMesh.addColor(bigShapeColour);
                bigMesh.addVertex(bigShape[0][i+1]);
                bigMesh.addColor(bigShapeColour);
                
                bigMesh.addIndex(i);
                bigMesh.addIndex(i+1);
                bigMesh.addIndex(i+2);
                
                bigMesh.addIndex(i+1);
                bigMesh.addIndex(i+2);
                bigMesh.addIndex(i+3);
            }
        }
    }
}

void ShapeBuilder::draw(int x, int y) {
    drawBuffer.begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofClear(0);
    ofPushMatrix();
    ofTranslate(startingX, startingY, startingY);
    ofRotateY(viewRot);
    ofTranslate(-startingX, -startingY, -startingY);
    
//    if(smallShape.size() > 0) {
//        if(smallShape[0].size() >= lines[0].size() - 1) {
//            for(int i = 0; i < lines[0].size(); i++) {
//                if(lines[0][i] < smallShape[0].size() - 1) {
//                    ofPushStyle();
//                    ofSetColor(smallShapeColour, ofMap(smallShape[0][lines[0][i]].distance(ofVec3f(startingX, startingY, startingY)), 0, 320, 255, 0));
//                    ofSetLineWidth(ofMap(smallShape[0][lines[0][i]].distance(ofVec3f(startingX, startingY, startingY)), 0, 320, 5, 1));
//                    ofDrawLine(smallShape[0][lines[0][i]].x, smallShape[0][lines[0][i]].y, smallShape[0][lines[0][i]].z, smallShape[0][lines[0][i]+1].x, smallShape[0][lines[0][i]+1].y, smallShape[0][lines[0][i]+1].z);
//                    ofPopStyle();
//                }
//                
//                if(lines[0][i] < smallShape[0].size()) {
//                    lines[0][i]++;
//                }
//                else {
//                    lines[0][i] = 0;
//                }
//            }
//        }
//    }
//    
//    if(mediumShape.size() > 0) {
//        if(mediumShape[0].size() >= lines[1].size() - 1) {
//            for(int i = 0; i < lines[1].size(); i++) {
//                if(lines[1][i] < mediumShape[0].size() - 1) {
//                    ofPushStyle();
//                    ofSetColor(mediumShapeColour, ofMap(mediumShape[0][lines[1][i]].distance(ofVec3f(startingX, startingY, startingY)), 0, 320, 255, 0));
//                    ofSetLineWidth(ofMap(mediumShape[0][lines[1][i]].distance(ofVec3f(startingX, startingY, startingY)), 0, 320, 5, 1));
//                    ofDrawLine(mediumShape[0][lines[1][i]].x, mediumShape[0][lines[1][i]].y, mediumShape[0][lines[1][i]].z, mediumShape[0][lines[1][i]+1].x, mediumShape[0][lines[1][i]+1].y, mediumShape[0][lines[1][i]+1].z);
//                    ofPopStyle();
//                }
//                
//                if(lines[1][i] < mediumShape[0].size()) {
//                    lines[0][i]++;
//                }
//                else {
//                    lines[1][i] = 0;
//                }
//            }
//        }
//    }
//    
//    if(bigShape.size() > 0) {
//        if(bigShape[0].size() >= lines[2].size() - 1) {
//            for(int i = 0; i < lines[2].size(); i++) {
//                if(lines[2][i] < bigShape[0].size() - 1) {
//                    ofPushStyle();
//                    ofSetColor(bigShapeColour, ofMap(bigShape[0][lines[2][i]].distance(ofVec3f(startingX, startingY, startingY)), 0, 320, 255, 0));
//                    ofSetLineWidth(ofMap(bigShape[0][lines[2][i]].distance(ofVec3f(startingX, startingY, startingY)), 0, 320, 5, 1));
//                    ofDrawLine(bigShape[0][lines[2][i]].x, bigShape[0][lines[2][i]].y, bigShape[0][lines[2][i]].z, bigShape[0][lines[2][i]+1].x, bigShape[0][lines[2][i]+1].y, bigShape[0][lines[2][i]+1].z);
//                    ofPopStyle();
//                }
//                
//                if(lines[2][i] < bigShape[0].size()) {
//                    lines[2][i]++;
//                }
//                else {
//                    lines[2][i] = 0;
//                }
//            }
//        }
//    }
    
    smallMesh.draw();
    bigMesh.draw();
    
    
    drawBuffer.end();
    
    drawBuffer.draw(x,y);
    
    viewRot+=0.1;
    
}
