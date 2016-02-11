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
    
    map<int, string> lightMap;
    map<int, shared_ptr<ofParameter<int>>> channels;
    
    vector<int> meters;
    
    ofxMidiIn midiIn;
    void newMidiMessage(ofxMidiMessage& msg);
    
    
    void setMeter(int value);
    ofParameter<float> maxMeterVal;
    ofParameter<bool> useDecay;
    ofParameter<float> decayAmount;
    float audioValue;
    
    vector<int> washAddresses;
    ofParameter<ofColor> washCol;
    
    ofParameter<int> spotBrightness;
    
};
