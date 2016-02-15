#pragma once

#include "ofMain.h"
#include "ofxPanel.h"

//class BaseHasPanel {
//public:
//    
//    BaseHasPanel(): guiDirty(true) {}
//    virtual ~BaseHasPanel() {}
//    
//    virtual void buildPanel() {
//        
//        if (panel == nullptr) {
//            panel = make_shared<ofxPanel>();
//            panel->setup("", "fixture-settings.xml");
//        }
//        
//        guiDirty = false;
//    }
//    
//    
//    virtual shared_ptr<ofxPanel> getPanel() {
//        if (guiDirty) {
//            buildPanel();
//        }
//        return panel;
//    }
//    
//    template<typename T>
//    void addParameter(string name, int address, T initial, T min, T max) {
//        parameters[address] = make_shared<ofParameter<T>>(name, initial, min, max);
////        guiDirty = true;
//        
//        auto param = parameters[address];
//        auto type = param->type();
//        
//        if (type == "11ofParameterIiE") {
//            panel->add(param->cast<int>());
//        }
//        else if (type == "11ofParameterIfE") {
//            panel->add(param->cast<float>());
//        }
//        else if (type == "11ofParameterI8ofColor_IhEE") {
//            panel->add(param->cast<ofColor>());
//        }
//
//    }
//    
//    virtual void drawGui() {
//        getPanel()->draw();
//    }
//    
//    
//    
//    map<int, shared_ptr<ofAbstractParameter>> parameters;
//    bool guiDirty;
//
//
//protected:
//    shared_ptr<ofxPanel> panel;
//
//    
//};




class Fixture  {
    
public:
    Fixture(string name, int startAddress=0);
    
    void setDmxStartAddress(int a) { mDmxStartAddress = a; }
    void setDmxUniverse(string u) { mDmxUniverse = u; }
    
    int getDmxStartAddress() { return mDmxStartAddress; }
    string getDmxUniverse() { return mDmxUniverse; }
    int getNumChannels() { return mNumChannels; }
    string getName() { return mName; }
    
    virtual shared_ptr<ofxPanel> getPanel() {
        if (mPanel == nullptr) {
            mPanel = make_shared<ofxPanel>();
            mPanel->setup(mName, "fixture-settings.xml");
        }
        return mPanel;
    }
    
    template<typename T>
    void addParameter(string name, int address, T initial, T min, T max) {
        
        auto param = make_shared<ofParameter<T>>(name, initial, min, max);
        
        mParameters[address] = param;
        mPanel->add(*param.get());
        
        auto type = typeid(T).name();
        if (type == typeid(ofColor).name()) {
            mNumChannels+= 3;
        }
        else if (type == typeid(int).name()) {
            mNumChannels+= 1;
        }
        else if (type == typeid(float).name()) {
            mNumChannels+= 1;
        }
        
    }
    
    virtual void drawGui() {
        getPanel()->draw();
    }
    
    
    const map<int, shared_ptr<ofAbstractParameter>>& getParameters() const { return mParameters; }
    
protected:
    string mName;
    ofxGuiGroup mDmxGroup;
    ofParameter<int> mDmxStartAddress, mNumChannels;
    ofParameter<string> mDmxUniverse;
  
    shared_ptr<ofxPanel> mPanel;
    
    map<int, shared_ptr<ofAbstractParameter>> mParameters;
    
    static map<string, int> fixtureNames;
    string getUniqueName(const string &name);

};













