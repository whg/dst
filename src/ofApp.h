#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxGui.h"
#include "ofxMidi.h"

#include "ofxPanelManager.h"
#include "ofxParameterMapper.h"

class Something {
public:

    Something(): gui(nullptr), guiDirty(true) {
        
    }
    
    void buildPanel() {
        
        if (gui == nullptr) {
            gui = make_shared<ofxPanel>();
            gui->setup();
        }
    
        gui->clear();
        
        for (auto &pair : parameters) {
            auto param = pair.second;
            auto type = param->type();
//            cout << type << endl;
            
            if (type == "11ofParameterIiE") {
                gui->add(param->cast<int>());
            }
            else if (type == "11ofParameterIfE") {
                gui->add(param->cast<float>());
            }
            else if (type == "11ofParameterI8ofColor_IhEE") {
                gui->add(param->cast<ofColor>());
            }

        }
        guiDirty = false;
    }
    
    shared_ptr<ofxPanel> gui;
    
    template<typename T>
    void addParameter(string name, T initial, T min, T max) {
        parameters[name] = make_shared<ofParameter<T>>(name, initial, min, max);
        guiDirty = true;
    }
    
    void drawGui() {
        if (guiDirty) {
            buildPanel();
        }
        gui->draw();
    }
    
    
    
    map<string, shared_ptr<ofAbstractParameter>> parameters;
    bool guiDirty;
};



class ofApp : public ofBaseApp, public ofxMidiListener {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    ofxDmx dmx;
    int level;
    
//    shared_ptr<ofxPanel> panel;
    
    
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
    
    
    Something something;
    ofxParameterMapper pm;
};
