#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCV.h"
#include "ofxCv.h"

class KinectData {
    
public:
    
    //-----Methods-----
    KinectData();
    void setup();
    void update();
    vector<ofVec3f> getData();
    void draw(int x, int y);
    
    //-----Objects-----
    ofxKinect kinect;
    ofxCvGrayscaleImage gimg;
    ofxCvFloatImage fimg;
    
    //-----Variables-----
    float thresh;
    vector<ofVec3f> location;
    
    ofFbo buffer;
};
