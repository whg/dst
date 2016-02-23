#include "Fixture.h"

map<string, int> Fixture::fixtureNames;

string Fixture::getUniqueName(const string &name) {

    try {
        fixtureNames.at(name)++;
    }
    catch (const out_of_range &e) {
        fixtureNames.emplace(name, 1);
    }

    return name + ofToString(fixtureNames[name]);
    
}

Fixture::Fixture(string name, int startAddress): mName(getUniqueName(name)), mFixedAddress(startAddress != 0) {
    
    mDmxGroup.setup("DMX");
    mDmxGroup.add(mDmxStartAddress.set("Start addr", startAddress, 0, 512));
    mDmxGroup.add(mNumChannels.set("Num chans", 0, 1, 32));
    mDmxGroup.add(mDmxUniverse.set("U", "-"));
    
    auto panel = getPanel();
    mPanel->add(&mDmxGroup);
    
}

shared_ptr<ofxPanel> Fixture::getPanel() {
    if (mPanel == nullptr) {
        mPanel = make_shared<ofxPanel>();
        mPanel->setup(mName, mName + ".xml");
    }
    return mPanel;
}

void Fixture::drawGui() {
    getPanel()->draw();
}

Colorado::Colorado(string name, int startAddress): Fixture(name, startAddress) {
    
    // in AR1.D mode (or tour as they call it)
    
    addParameter("dimmer", 1, 0, 0, 255);
    addParameter("colour", 2, ofColor::black, ofColor(0), ofColor(255));
}

FadoColumn::FadoColumn(string name, int startAddress, bool callSetup): Fixture(name, startAddress) {
    
    mPanel->add(mDoUpdate.set("update", true));
    mPanel->add(mAudioInput.set("audio input", 0, 0, 1));
    mPanel->add(mMaxMeterVal.set("max val", 1, 0, 1));
    mPanel->add(mDoDecay.set("decay", true));
    mPanel->add(mDecay.set("decay amount", 0.7, 0, 1));
    
    
    if (callSetup) setup();
}

void FadoColumn::setup() {
    vector<string> names = { "g1", "g2", "y1", "y2", "r1", "r2" };
    int address = 2, counter = 0;
    vector<int> offsets = { 2, 1, 5, 4, 8, 7 };
    for (auto &name : names) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(param, mDmxStartAddress + offsets[counter++]);
        mMeters.push_back(param);
    }
    
    mNumChannels.set(9);
}

void FadoColumn::update() {
    
    if (mDoUpdate) {
        
    
        if (mDoDecay && mAudioInput < mAudioValue) {
            mAudioValue = mAudioValue * mDecay;
        }
        else {
            mAudioValue = mAudioInput;
        }
        

        
        float mappedVal = ofMap(mAudioValue, 0, mMaxMeterVal, 0, mMeters.size(), true);
        if (mappedVal >= mMeters.size()) {
            mappedVal = mMeters.size() - 0.001;
        }
        
        
        for (int i = 0; i < mMeters.size(); i++) {
            if (i <= mappedVal) {
                mMeters[i]->set(255);
            }
            else {
                mMeters[i]->set(0);
            }
        }
        
        int notFullIndex = floor(mappedVal);
        float amount = mappedVal - notFullIndex;
        
        mMeters[notFullIndex]->set(255 * amount);
        
        mLastAudioValue = mAudioValue;

    }
    
}

AnglepoiseSet::AnglepoiseSet(string name, int startAddress): FadoColumn(name, startAddress, false) {
    
    setup();
}

void AnglepoiseSet::setup() {
    vector<string> names = { "g1", "g2", "g3", "g4",
                             "y1", "y2", "y3", "y4",
                             "r1", "r2", "r3", "r4" };
    int i = 1;
    for (auto &name : names) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(param, i++);
        mMeters.push_back(param);
    }
}

Stairs::Stairs(string name, int startAddress): Fixture(name, startAddress) {
    
    mPanel->add(mDoUpdate.set("update", true));
    
    const int numPresets = 3;
    mPresetColours.resize(numPresets);

    mPresetColour.addListener(this, &Stairs::presetColourChanged);
    mPanel->add(mCurrentPreset.set("current preset", 0, 0, numPresets));
    mPanel->add(mPresetColour.set("preset colour", ofColor::black, ofColor(0), ofColor(255)));
    
    int offset = startAddress;
    mDmxStartAddress = offset;
    for (int i = 0; i < 13; i++) {
        addParameter(make_shared<ofParameter<ofColor>>("step" + ofToString(i+1), ofColor::black, ofColor(0, 0), ofColor(255, 255)), i*3);
    }
    
    mNumChannels = 13 * 3;
    mFixedAddress = true;
    

}

void Stairs::update() {
    
    if (mDoUpdate) {
        
        auto &currentCol = mPresetColours[mCurrentPreset];
        for (auto &colourPair : mParameters) {
            auto &colour = colourPair.second->cast<ofColor>();
            colour.set(ofColor(currentCol.r, currentCol.g, currentCol.b, colour.get().a));
            
        }
    }
    
}

void Stairs::presetChanged(int &i) {
    
}

void Stairs::presetColourChanged(ofColor &col) {
    mPresetColours[mCurrentPreset].set(col);
}