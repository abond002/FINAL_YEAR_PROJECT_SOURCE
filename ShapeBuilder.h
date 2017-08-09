#include "ofMain.h"
#include "MarkovChain.h"

class ShapeBuilder {
public:
    
    ShapeBuilder();
    
    void setup(deque<Classifier> tempClass);
    void makeShape(Classifier temp, int target);
    void update();
    void draw(int x, int y);
    
    float initialDistance;
    float globalRot;
    float globalRotStep;
    
    int startingX, startingY;
    
    ofFbo drawBuffer;
    
    deque<float> rotSpeed;
    deque<float> currentRot;
    deque<float> distance;
    deque<int> rotationAxis;
    
    deque<deque<ofVec3f>> smallShape;
    deque<deque<ofVec3f>> mediumShape;
    deque<deque<ofVec3f>> bigShape;
    
    ofColor smallShapeColour;
    ofColor mediumShapeColour;
    ofColor bigShapeColour;
    
    float viewRot;
    
    deque<deque<int>> lines;
    
    int lineLength;
    
    bool getNew;
    
    float lerping;
    float lerpAmount;
    
    int currentShape;
    
    bool complete;
    
    ofMesh mesh;
};
