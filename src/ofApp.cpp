#include "ofApp.h"




void ofApp::setup() {
    dmx.connect("/dev/tty.usbserial-EN129270", 512);
    ofBackground(0x10);
    ofSetWindowShape(300, 600);

    paramGroup.setName("Panel");
    
    midiIn.openPort("virtualMIDI");
    midiIn.addListener(this);
    
    
    lightMap["strobe-intensity"] = 1;
    lightMap["strobe-duration"]  = 2;
    lightMap["strobe-rate"]      = 3;
    
    lightMap["wash1-intensity"]  = 4;
    lightMap["wash1-shutter"]    = 5;
    
    lightMap["wash2-intensity"]  = 6;
    lightMap["wash2-shutter"]    = 7;
    
    lightMap["mirror-uv"]        = 10; // 4
    lightMap["blind-uv"]         = 13; // 5
    lightMap["bath-uv"]          = 16; // 6
    
    lightMap["mirror-white"]     = 19; // 7
    lightMap["blind-white"]      = 22; // 8
    lightMap["bath-white"]       = 25; // 9
    
    lightMap["panel1"]           = 28; // 10
    lightMap["panel2"]           = 31; // 11
    
    
    stringstream ss;
    for (const auto &pair : lightMap) {
        auto p = shared_ptr<ofParameter<int> >(new ofParameter<int>(pair.first + " : " + ofToString(pair.second) , 0, 0, 255));
        paramGroup.add(*(p.get()));
        channels[pair.first] = p;
    }
//
    
    autoCycle.set("AutoCycle",false);
    panel.setup(paramGroup);
    
    
    ofSetVerticalSync(false);
}

void ofApp::update() {
    int t = 200;
    
    for (const auto &chan : channels) {
        dmx.setLevel(lightMap[chan.first], *chan.second.get());
    }
    
    dmx.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));  
}

void ofApp::draw() {
    
    panel.draw();
    
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    if (msg.status == MIDI_CONTROL_CHANGE) {
        for (const auto &pair : lightMap) {
            auto lightIndex = pair.second;
            auto lightName = pair.first;
            if (msg.control == lightIndex) {
                channels[lightName]->set(msg.value * 2);
            }
        }
    }
    else if (msg.status == MIDI_NOTE_ON) {
        for (const auto &pair : lightMap) {
            auto lightIndex = pair.second;
            auto lightName = pair.first;
            if (msg.pitch == lightIndex) {
                channels[lightName]->set(msg.velocity * 2);
            }
        }
    }
    else if (msg.status == MIDI_NOTE_OFF) {
        for (const auto &pair : lightMap) {
            auto lightIndex = pair.second;
            auto lightName = pair.first;
            if (msg.pitch == lightIndex) {
                channels[lightName]->set(0);
            }
        }
    }
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {

}
