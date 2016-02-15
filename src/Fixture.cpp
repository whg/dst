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

Fixture::Fixture(string name, int startAddress): mName(getUniqueName(name)) {
    
    mDmxGroup.setup("DMX");
    mDmxGroup.add(mDmxStartAddress.set("Start addr", startAddress, 0, 512));
    mDmxGroup.add(mNumChannels.set("Num chans", 0, 1, 32));
    mDmxGroup.add(mDmxUniverse.set("U", "-"));
    
    auto panel = getPanel();
//    panel->setName(getUniqueName(name));
    mPanel->add(&mDmxGroup);
    
    
}