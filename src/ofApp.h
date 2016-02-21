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

};
