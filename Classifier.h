#include "ofMain.h"

class Classifier {
public:
    
    Classifier();
    
    void setup();
    void update();
    
    deque<ofVec3f> headings;
    deque<ofVec3f> position;
    
    int matches;
};
