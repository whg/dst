#pragma once

#include "ofMain.h"
#include "ofxPanel.h"

class Fixture  {
    
public:
    Fixture(string name, int startAddress=0);
    
    virtual void update() {}
    
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
    void addParameter(shared_ptr<ofParameter<T>> param, int address) {
        
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
        else if (type == typeid(unsigned char).name()) {
            mNumChannels+= 1;
        }
        
    }
    
    template<typename T>
    void addParameter(string name, int address, T initial, T min, T max) {
        
        auto param = make_shared<ofParameter<T>>(name, initial, min, max);
        addParameter(param, address);
        
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

class FadoColumn : public Fixture {
public:
    FadoColumn(string name="FadoCol", int startAddress=0);
    
    void update();
<<<<<<< HEAD
    
protected:
    ofParameter<bool> mDoUpdate;
    ofParameter<float> mAudioInput, mMaxMeterVal;
    vector<shared_ptr<ofParameter<int>>> mMeters;
    
};

class AnglepoiseSet : public FadoColumn {
public:
    AnglepoiseSet(string name="AnglepoiseSet", int startAddress=0);
    
};
=======
};

>>>>>>> 5ed9a3d87f331e5379b8ad7224413cfbdfc61afc







