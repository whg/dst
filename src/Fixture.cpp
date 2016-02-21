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

FadoColumn::FadoColumn(string name, int startAddress): Fixture(name, startAddress) {
    
<<<<<<< HEAD
    mPanel->add(mDoUpdate.set("update", true));
    mPanel->add(mAudioInput.set("audio input", 0, 0, 1));
    mPanel->add(mMaxMeterVal.set("max val", 1, 0, 1));

    
    vector<string> names = { "one", "two", "three", "four", "five", "six" };
    int i = 1;
    for (auto &name : names) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(param, i++);
        mMeters.push_back(param);
    }
    
}

void FadoColumn::update() {
    
    if (mDoUpdate) {
    
        float mappedVal = ofMap(mAudioInput, 0, mMaxMeterVal, 0, mMeters.size(), true);
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
    }
    
}

AnglepoiseSet::AnglepoiseSet(string name, int startAddress): FadoColumn(name, startAddress) {
    
    vector<string> names = { "seven", "eight", "nine", "ten", "eleven", "twelve" };
    int i = 7;
    for (auto &name : names) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(param, i++);
        mMeters.push_back(param);
    }
    
=======
    addParameter("one", 1, 0, 0, 255);
    addParameter("two", 1, 0, 0, 255);
    addParameter("three", 1, 0, 0, 255);
    addParameter("four", 1, 0, 0, 255);
    addParameter("five", 1, 0, 0, 255);
    addParameter("six", 1, 0, 0, 255);
>>>>>>> 5ed9a3d87f331e5379b8ad7224413cfbdfc61afc
}