#include "ofMain.h"
#include "MarkovChain.h"

class OrbitBuilder {
public:
    
    OrbitBuilder();
    void setup();
    void update();
    void draw(int x, int y);
    
    void getNewValues(Classifier tempClassifier);
    
    ofFbo buffer;
    
    int maxVertex;
    
    float spacingX;
    float startingY, startingX;
    float startingXSmall, startingXMedium, startingXBig;
    float radius;
    
    float smallShapeCurrentRot, mediumShapeCurrentRot, bigShapeCurrentRot;
    float rotStep;
    
    float updateRate;
    float minOrbitDist;
    float maxOrbitDist;
    float orbitRotRange;
    
    deque<float> smallShapeOrbitsCurrentRot, mediumShapeOrbitsCurrentRot, bigShapeOrbitsCurrentRot, smallShapeOrbitsRotStep, mediumShapeOrbitsRotStep, bigShapeOrbitsRotStep, smallShapeOrbitsDistance, mediumShapeOrbitsDistance, bigShapeOrbitsDistance;
    
    deque<int> smallShapeOrbitsAxis, mediumShapeOrbitsAxis, bigShapeOrbitsAxis;
    
    deque<float> smallShapeTargetRot, mediumShapeTargetRot, bigShapeTargetRot, smallShapeTargetDist, mediumShapeTargetDist, bigShapeTargetDist;
    
    ofMesh smallShapeMesh, mediumShapeMesh, bigShapeMesh;
    
    deque<ofColor> smallShapeColour, mediumShapeColour, bigShapeColour;
};

