#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxMidi.h"

class ofxOscCenterNewMessageArgs;

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    ofxMidiIn midiIn;
    
    void newOscMessage(ofxOscCenterNewMessageArgs &args);

};
