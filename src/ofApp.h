#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxGui.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    ofxDmx dmx;
    int level;
    
    ofxPanel panel;
    ofParameterGroup paramGroup;
    
//    vector<shared_ptr<ofParameter<int>>> channels;
    ofParameter<bool> autoCycle;
    
    map<string, int> lightMap;
    map<string, shared_ptr<ofParameter<int>>> channels;
    
    ofxMidiIn midiIn;
    void newMidiMessage(ofxMidiMessage& msg);
};
