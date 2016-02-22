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
    
    if (callSetup) setup();
}

void FadoColumn::setup() {
    vector<string> names = { "g1", "g2", "y1", "y2", "r1", "r2" };
    int address = 2, counter = 0;
    vector<int> addresses = { 2, 3, 5, 6, 8, 9 };
    for (auto &name : names) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(param, addresses[counter++]);
        cout << address << endl;
        mMeters.push_back(param);
    }
    
    mNumChannels.set(9);
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

Stairs::Stairs(string name, int startAddress): Fixture(name, startAddress, false) {
    
    
}