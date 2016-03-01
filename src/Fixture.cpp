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
    
//    addParameter("dimmer", 1, 0, 0, 255);
    addParameter("colour", 0, ofColor::black, ofColor(0), ofColor(255));
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
            mAudioValue = mAudioValue - mDecay;
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
    
    vector<string> names = { "g1", "g2", "y1", "y2", "r1", "r2" };
    int i = 0;
    
    vector<string> nameslong = { "g1", "g2", "g3", "g4", "y1", "y2", "y3", "y4","r1", "r2","r3", "r4" };
    for (auto &name : nameslong) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(make_shared<ofParameter<ofColor>>(name, ofColor(0, 0), ofColor(0), ofColor(255)), i*3);
        mLongMeters.push_back(param);

        i++;
    }


    for (auto &name : names) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(make_shared<ofParameter<ofColor>>(name, ofColor(0, 0), ofColor(0), ofColor(255)), i*3);
        mMeters.push_back(param);
        i++;
    }
    
    for (auto &name : names) {
        auto param = make_shared<ofParameter<int>>(name, 0, 0, 255);
        addParameter(make_shared<ofParameter<ofColor>>(name, ofColor(0, 0), ofColor(0), ofColor(255)), i*3);
        i++;
    }
    
    mNumChannels.set(names.size() * 3 * 2);

}

void AnglepoiseSet::update() {
    
    if (!mDoUpdate) return;
    
    FadoColumn::update();
    vector<ofColor> cols = {ofColor(0, 255, 0), ofColor(0, 255, 0), ofColor(255, 255, 0), ofColor(255, 255, 0), ofColor(255, 0, 0), ofColor(255, 0, 0) };

    int offset = 12 * 3;
    for (int i = 0; i < 5; i++) {
        auto col = cols[i % 5];
        col.a*= (mMeters[i]->get() / 255.0f);
        mParameters[offset+i*3]->cast<ofColor>().set(col);
        mParameters[offset+i*2*3]->cast<ofColor>().set(col);
    }
    
    
    float mappedVal = ofMap(mAudioValue, 0, mMaxMeterVal, 0, mLongMeters.size(), true);
    if (mappedVal >= mLongMeters.size()) {
        mappedVal = mLongMeters.size() - 0.001;
    }
    
    
    for (int i = 0; i < mLongMeters.size(); i++) {
        if (i <= mappedVal) {
            mLongMeters[i]->set(255);
        }
        else {
            mLongMeters[i]->set(0);
        }
    }
    
    int notFullIndex = floor(mappedVal);
    float amount = mappedVal - notFullIndex;
    
    mLongMeters[notFullIndex]->set(255 * amount);
    
    for (int i = 0; i < 12; i++) {
        auto col = cols[i /2];
        col.a*= (mLongMeters[i]->get() / 255.0f);
        mParameters[i*3]->cast<ofColor>().set(col);
//        mParameters[i*6+3]->cast<ofColor>().set(col);
        
    }
    
    
//    if (!mDoUpdate) return;
//    
//    int i = 0;
//    for (auto &pair : mParameters) {
//        auto &param = pair.second->cast<ofColor>();
//        float brightness = mMeters[i]->get() / 255.0f;
//        param.set(ofColor(param->r * brightness, param->g * brightness, param->b * brightness, 255));
//        i++;
//    }
    

}

TableSet::TableSet(string name, int startAddress): Fixture(name, startAddress) {
    
//    vector<ofColor> cols = {ofColor(0, 255, 0), ofColor(0, 255, 0), ofColor(255, 255, 0), ofColor(255, 255, 0), ofColor(255, 0, 0), ofColor(0, 0, 0) };
//    int i = 0;
//    for (auto &col : cols) {
//        addParameter(make_shared<ofColor>("col" + ofToString(i+1), ), i*3);
//    }
    
    
    for (int i = 0; i < 9; i++) {
        addParameter(make_shared<ofParameter<ofColor>>("col" + ofToString(i), ofColor(255, 255, 100), ofColor(0), ofColor(255)), i*3);
    }
}


MainFloor::MainFloor(string name, int startAddress): Fixture(name, startAddress) {
    
    mPanel->add(mDoUpdate.set("update", true));

    mPanel->add(mPresetColour.set("presetcolour", ofColor::black, ofColor(0, 0), ofColor(255, 255)));

    
    // 12 anglepoise, 12 lamp, 6 shoes
    // no anglepoise
    for (int i = 0; i < 18; i++) {
        addParameter(make_shared<ofParameter<ofColor>>("anglepoise" + ofToString(i+1), ofColor::black, ofColor(0, 0), ofColor(255)), i*3);
    }
    
}

void MainFloor::update() {
    if (mDoUpdate) {
        auto &currentCol = mPresetColour.get();
        for (auto &colourPair : mParameters) {
            auto &colour = colourPair.second->cast<ofColor>();
            colour.set(currentCol);
            
        }
    }

}

Stairs::Stairs(string name, int startAddress): Fixture(name, startAddress), mPrefixOffset(90) {
    
    mPanel->add(mDoUpdate.set("update", true));
    
    const int numPresets = 3;
    //mPresetColours.resize(numPresets);
    
    vector<ofColor> cols = { ofColor(255, 255, 150), ofColor(0, 255, 0), ofColor(0, 0, 255), ofColor(255, 0, 0) };
    for (auto col : cols) mPresetColours.push_back(col);
    
    
    mPresetColour.addListener(this, &Stairs::presetColourChanged);
    mCurrentPreset.addListener(this, &Stairs::presetChanged);
    mPanel->add(mCurrentPreset.set("currentpreset", mPrefixOffset, 0, 127));
    mPanel->add(mPresetColour.set("presetcolour", ofColor(mPresetColours[0].r, mPresetColours[0].g, mPresetColours[0].b, 0), ofColor(0, 0), ofColor(255, 255)));
    
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
        int index = ofClamp(mCurrentPreset-mPrefixOffset, 0, 3);
        auto &currentCol = mPresetColours[index];
        for (auto &colourPair : mParameters) {
            auto &colour = colourPair.second->cast<ofColor>();
            colour.set(ofColor(currentCol.r, currentCol.g, currentCol.b, colour.get().a));
            
        }
    }
    
}

void Stairs::presetChanged(int &i) {
    int q = mCurrentPreset;
    if (i >= 90 && i < 94) {
        cout << i-mPrefixOffset << endl;
        mPresetColour.set(mPresetColours[i-mPrefixOffset]);
    }
}

void Stairs::presetColourChanged(ofColor &col) {
    if (mCurrentPreset >= 90 && mCurrentPreset < 94) {
        int index = mPrefixOffset-mCurrentPreset;
//        mPresetColours[index].r = col.r;
//        mPresetColours[index].g = col.g;
//        mPresetColours[index].b = col.b;
    }
}


SingleLED::SingleLED(string name, int startAddress):Fixture(name, startAddress) {
    addParameter(make_shared<ofParameter<unsigned char>>("brightness", 0, 0, 255), 0);
}


Hob::Hob(string name, int startAddress):Fixture(name, startAddress) {
//    vector<int> addresses = {98, 99, 101, 102 };
    vector<int> addresses = { 0, 1, 3, 4 };
    int i = 1;
    for (auto address : addresses) {
        addParameter(make_shared<ofParameter<unsigned char>>("pad" + ofToString(i++), 0, 0, 255), address);
    }
    
    mFixedAddress = true;
}

Bathroom::Bathroom(string name, int startAddress):Fixture(name, startAddress) {
    
    vector<pair<string, int>> pairs = {
        make_pair("blind", 0),
        make_pair("bath1", 1),
        make_pair("bath2", 2),
        make_pair("uvmirror", 3),
        make_pair("whitemirror", 4),
    };
    
    for (auto &pair : pairs) {
        addParameter(make_shared<ofParameter<unsigned char>>(pair.first, 0, 0, 255), pair.second);
    }
    
    mFixedAddress = true;
}

Bulkheads::Bulkheads(string name, int startAddress):Fixture(name, startAddress) {

    addParameter(make_shared<ofParameter<ofColor>>("one", ofColor::black, ofColor(0, 0), ofColor(255)), 0);
    addParameter(make_shared<ofParameter<ofColor>>("two", ofColor::black, ofColor(0, 0), ofColor(255)), 3);
    addParameter(make_shared<ofParameter<unsigned char>>("houseone", 0, 0, 255), 7);
    addParameter(make_shared<ofParameter<unsigned char>>("housetwo", 0, 0, 255), 8);
    
    
    mFixedAddress = true;
}

Overheads::Overheads(string name, int startAddress):Fixture(name, startAddress) {
    
    for (int i = 0; i < 3; i++) {
        addParameter(make_shared<ofParameter<unsigned char>>("light" + ofToString(i+1), 0, 0, 255), i);
    }
    
    mFixedAddress = true;
}

Pendants::Pendants(string name, int startAddress):Fixture(name, startAddress) {
    
    mPanel->add(mDoUpdate.set("update", true));
    mPanel->add(mOnTime.set("ontime", 0.2, 0.01, 0.5));
    mPanel->add(mBrightness.set("brightness", 255, 0, 255));
    
    for (int i = 0; i < 5; i++) {
        auto param = make_shared<ofParameter<unsigned char>>("light" + ofToString(i+1), 0, 0, 255);
//        param->addListener(this, &Pendants::paramChanged);
        addParameter(param, i);
        states.push_back(false);
        offTimes.push_back(0);
        onTimes.push_back(0);
        auto h = make_shared<ofParameter<unsigned char>>("hits" + ofToString(i+1), 0, 0, 127);
        h->addListener(this, &Pendants::hitChanged);
        mPanel->add(*h.get());
        hits.push_back(h);
    }
    
    mFixedAddress = true;
}

void Pendants::hitChanged(unsigned char &v) {
    
    if (v == 0) return;
    
    int i = 0;
    float timeNow = ofGetElapsedTimef();
    auto iter = mParameters.begin();
    for (auto &hit : hits) {
        if (&hit->get() == &v) {
            onTimes[i] = timeNow;
            iter->second->cast<unsigned char>().set(mBrightness);
//            cout << "on at time " << timeNow << endl;
            return;
        }
        i++;
        iter++;
    }
}

void Pendants::update() {
    
    if (!mDoUpdate) return;
    
    int i = 0;
    float timeNow = ofGetElapsedTimef();
    for (auto &pair : mParameters) {
        auto &param = pair.second->cast<unsigned char>();

        float timeSince = timeNow - onTimes[i];
        if (timeSince < mOnTime) {
            param.set(mBrightness );
//            cout << i << " staying on at " << timeNow << endl;
        }
        else {
            param.set(0);
            //cout << i << " off at time " << timeNow << endl;

        }
        i++;
    }
    
    
//    int i = 0;
//    float timeNow = ofGetElapsedTimef();
//    for (auto &pair : mParameters) {
//        if (states[i]) {
//            auto timeSince = (timeNow - onTimes[i]);
//            if (timeSince > mOnTime) {
//                auto onFor = (timeNow - offTimes[i]);
//                auto &param = pair.second->cast<unsigned char>();
//                param.removeListener(this, &Pendants::paramChanged);
//                param.set(0);
//                param.addListener(this, &Pendants::paramChanged);
//                states[i] = false;
//                cout << "closed after " << timeSince << " on " << i << endl;
//                cout << "closed at" << timeNow << endl;
//            }
//            else {
//                cout << "still on after " << timeSince << " on " << i << endl;
////                auto param = pair.second->cast<unsigned char>();
////                param.removeListener(this, &Pendants::paramChanged);
////                param.set(255);
////                param.addListener(this, &Pendants::paramChanged);
////                if (<#condition#>) {
////                    <#statements#>
////                }
//
//            }
//        }
//        
//         i++;
//    }
//    
//    cout << "frame at " << timeNow << endl;
    
}

void Pendants::paramChanged(unsigned char &v) {
    int i = 0;
    for (auto &pair : mParameters) {
        if (&pair.second->cast<unsigned char>().get() == &v) {
            if (v == 0) {
                if (states[i]) { // don't turn off
                    auto &param = pair.second->cast<unsigned char>();
                    param.removeListener(this, &Pendants::paramChanged);
                    param.set(255);
                    param.addListener(this, &Pendants::paramChanged);
                }

            }
            else {
                if (!states[i]) {
                    states[i] = true;
                    auto &param = pair.second->cast<unsigned char>();
                    param.removeListener(this, &Pendants::paramChanged);
                    param.set(255);
                    param.addListener(this, &Pendants::paramChanged);
                    onTimes[i] = ofGetElapsedTimef();
                    cout << "set for " << i << " with " << v << endl;
                    cout << "on at time " << onTimes[i] << endl;
                }
            }
        }
        i++;
    }
}

Kitchen::Kitchen(string name, int startAddress):Fixture(name, startAddress) {
    vector<int> hobNums = { 0, 1, 3, 4 };
    for (int i = 0; i < hobNums.size(); i++) {
        addParameter(make_shared<ofParameter<unsigned char>>("hob" + ofToString(i+1), 0, 0, 255), hobNums[i]);
    }
    addParameter(make_shared<ofParameter<unsigned char>>("ovenleds2", 0, 0, 255), 2);
    addParameter(make_shared<ofParameter<unsigned char>>("ovenleds1", 0, 0, 255), 5);
    addParameter(make_shared<ofParameter<unsigned char>>("ovenscreen", 0, 0, 255), 6);
    addParameter(make_shared<ofParameter<unsigned char>>("umbrella", 0, 0, 255), 7);
    addParameter(make_shared<ofParameter<unsigned char>>("toaster", 0, 0, 255), 8);
    addParameter(make_shared<ofParameter<ofColor>>("head", ofColor(0, 0), ofColor(0), ofColor(255)), 9);
    addParameter(make_shared<ofParameter<unsigned char>>("binbag1", 0, 0, 255), 12);
    addParameter(make_shared<ofParameter<unsigned char>>("binbag2", 0, 0, 255), 13);
    addParameter(make_shared<ofParameter<unsigned char>>("bindrum1", 0, 0, 255), 14);
    addParameter(make_shared<ofParameter<unsigned char>>("bindrum2", 0, 0, 255), 15);
    addParameter(make_shared<ofParameter<unsigned char>>("box1", 0, 0, 255), 16);
    addParameter(make_shared<ofParameter<unsigned char>>("pineapple", 0, 0, 255), 17);
    addParameter(make_shared<ofParameter<unsigned char>>("skull", 0, 0, 255), 18);
    addParameter(make_shared<ofParameter<unsigned char>>("bag", 0, 0, 255), 19);
    addParameter(make_shared<ofParameter<unsigned char>>("bag", 0, 0, 255), 20);
    addParameter(make_shared<ofParameter<unsigned char>>("extactor", 0, 0, 255), 21);
    addParameter(make_shared<ofParameter<unsigned char>>("plug", 0, 0, 255), 22);
    
    addParameter(make_shared<ofParameter<unsigned char>>("ear", 0, 0, 255), 24);
    addParameter(make_shared<ofParameter<unsigned char>>("hihat", 0, 0, 255), 25);
    addParameter(make_shared<ofParameter<unsigned char>>("far-right", 0, 0, 255), 26);
    
    addParameter(make_shared<ofParameter<unsigned char>>("far-left", 0, 0, 255), 27);
    addParameter(make_shared<ofParameter<unsigned char>>("snare", 0, 0, 255), 28);
    addParameter(make_shared<ofParameter<unsigned char>>("tom", 0, 0, 255), 29);
    
    addParameter(make_shared<ofParameter<unsigned char>>("topleds", 0, 0, 255), 30);
    addParameter(make_shared<ofParameter<unsigned char>>("oven-bottom", 0, 0, 255), 31);
    addParameter(make_shared<ofParameter<unsigned char>>("oven-top", 0, 0, 255), 32);
    
    addParameter(make_shared<ofParameter<unsigned char>>("kettle", 0, 0, 255), 23);

    
}

void Kitchen::update() {
    
}