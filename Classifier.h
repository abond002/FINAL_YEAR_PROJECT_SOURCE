#include "ofMain.h"

class Classifier {
public:
    
    Classifier();
    
    void setup();
    void update();
    
    vector<ofVec3f> headings;
    vector<ofVec3f> position;
    
    int matches;
};
