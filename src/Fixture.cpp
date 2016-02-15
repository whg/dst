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
        mPanel->setup(mName, "fixture-settings.xml");
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