#pragma once

#include "ofMain.h"
#include "ofxPanel.h"

class Fixture  {
    
public:
    Fixture(string name, int startAddress=0);
    
    void update() {}
    
    void setDmxStartAddress(int a) { mDmxStartAddress = a; }
    void setDmxUniverse(string u) { mDmxUniverse = u; }
    
    int getDmxStartAddress() { return mDmxStartAddress; }
    string getDmxUniverse() { return mDmxUniverse; }
    int getNumChannels() { return mNumChannels; }
    string getName() { return mName; }
    bool getIsAddressFixed() { return mFixedAddress; }
    
    const map<int, shared_ptr<ofAbstractParameter>>& getParameters() const { return mParameters; }
    
    virtual shared_ptr<ofxPanel> getPanel();
    virtual void drawGui();
    
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

    
protected:
    string mName;
    ofxGuiGroup mDmxGroup;
    ofParameter<int> mDmxStartAddress, mNumChannels;
    bool mFixedAddress;
    ofParameter<string> mDmxUniverse;
  
    shared_ptr<ofxPanel> mPanel;
    
    map<int, shared_ptr<ofAbstractParameter>> mParameters;
    
    static map<string, int> fixtureNames;
    string getUniqueName(const string &name);

};


class Colorado : public Fixture {
public:
    Colorado(string name="Colorado", int startAddress=0);
};










