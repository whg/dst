#pragma once

#include "ofMain.h"
#include "ofxPanel.h"

class Fixture  {
    
public:
    Fixture(string name, int startAddress=0);
    
    virtual void setup() {}
    virtual void update() {}
    
    void setDmxStartAddress(int a) { mDmxStartAddress = a; }
    void setDmxUniverse(string u) { mDmxUniverse = u; }
    void setIsFixedAddress(bool b) { mFixedAddress = b; }
    
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

using RGBFixture = Colorado;

class FadoColumn : public Fixture {
public:
    FadoColumn(string name="FadoCol", int startAddress=0, bool callSetup=true);
    
    virtual void setup();
    void update();
    
protected:
    ofParameter<bool> mDoUpdate, mDoDecay;
    ofParameter<float> mAudioInput, mMaxMeterVal, mDecay;
    vector<shared_ptr<ofParameter<int>>> mMeters;
    float mAudioValue, mLastAudioValue;
    
};

class AnglepoiseSet : public FadoColumn {
public:
    AnglepoiseSet(string name="AnglepoiseSet", int startAddress=0);
    
    void setup();
    void update();
    vector<shared_ptr<ofParameter<int>>> mLongMeters;

};

class TableSet : public Fixture {
public:
    TableSet(string name="TableSet", int startAddress=0);
    
//    void setup();
//    void update();
    
};



class MainFloor : public Fixture {
public:
    MainFloor(string name="MainFloor", int startAddress=0);
    
    ofParameter<bool> mDoUpdate;

    ofParameter<ofColor> mPresetColour;
    
    void update();
    
};


class Stairs : public Fixture {
public:
    Stairs(string name="Stairs", int startAddress=0);
    
    void update();
    
    void presetChanged(int &i);
    void presetColourChanged(ofColor &col);

protected:
    ofParameter<bool> mDoUpdate;
    ofParameter<ofColor> mPresetColour;
    ofParameter<int> mCurrentPreset;
    vector<ofColor> mPresetColours;
    
    const int mPrefixOffset;

};


class SingleLED : public Fixture {
public:
    SingleLED(string name="SingleLED", int startAddress=0);
};



class Hob :public Fixture {
public:
    Hob(string name="Hob", int startAddress=0);
};

class Bathroom :public Fixture {
public:
    Bathroom(string name="Bathroom", int startAddress=0);
};

class Bulkheads :public Fixture {
public:
    Bulkheads(string name="Bulkheads", int startAddress=0);
};

class Overheads :public Fixture {
public:
    Overheads(string name="Overheads", int startAddress=0);
};

class Pendants :public Fixture {
public:
    Pendants(string name="Pendants", int startAddress=0);
    
    void update();
    void paramChanged(unsigned char &i);

    ofParameter<bool> mDoUpdate;
    ofParameter<float> mOnTime;
    ofParameter<unsigned char> mBrightness;
    vector<bool> states;
    vector<float> offTimes, onTimes;
    
    vector<shared_ptr<ofParameter<unsigned char>>> hits;
    void hitChanged(unsigned char &v);
};


class Kitchen :public Fixture {
public:
    Kitchen(string name="Kitchen", int startAddress=0);
    
    void update();
};
