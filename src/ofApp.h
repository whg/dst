#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxMidi.h"


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    ofxMidiIn midiIn;
    
<<<<<<< HEAD
=======
    shared_ptr<Fixture> something;
    
    
//    ofxParameterMapper pm;
>>>>>>> 5ed9a3d87f331e5379b8ad7224413cfbdfc61afc
};
