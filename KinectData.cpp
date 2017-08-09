#include "KinectData.h"

KinectData::KinectData() { //Put controls in GUI
    //Initialise the Kinect
    kinect.setDepthClipping(1000,3000);
    kinect.init();
    kinect.setRegistration(true);
    kinect.open();
    kinect.setCameraTiltAngle(3);
    //Depth threshold
    thresh = 0.0;
    buffer.allocate(480,480, GL_RGBA);
}

void KinectData::setup() {
    
}

void KinectData::update() {
    //Get new data and pass it into openCv object
    kinect.update();
    gimg.setFromPixels(kinect.getDepthPixels());
    fimg = gimg;
    
    location.clear();
    
    
    //Get pointers for pixel data
    float *pixels = fimg.getPixelsAsFloats();
    
    //Compare the brightness of pixels against threshold and put them in a vector
    for(int y = 5; y < 475; y++) {
        for(int x = 5; x < 635; x++) {
            if(x > 85 && x < 555) {
                float bright = pixels[x+fimg.width*y];
                if(bright > thresh) {
                    int z = ofMap(bright, thresh, 1, 5, 475);
                    location.push_back(ofVec3f(x,y,z));
                }
            }
        }
    }
}

vector<ofVec3f> KinectData::getData() {
    //Parse all the locations of pixels in a location vector
    return location;
}

void KinectData::draw(int x, int y) {
    buffer.begin();
    ofClear(0);
    for(ofVec3f t : location) {
        ofPushStyle();
        ofColor c = kinect.getColorAt(t.x, t.y);
        ofSetColor(c);
        ofDrawCircle(t.x, t.y, -400 + t.z, 1);
        ofPopStyle();
    }
    buffer.end();
    buffer.draw(x, y);
}
