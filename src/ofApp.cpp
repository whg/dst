#include "ofApp.h"

const int METER_CONTROL_NUMBER = 60;

#include "ofxOscCenter.h"

void ofApp::setup() {
//    dmx.connect("/dev/tty.usbserial-EN129270", 512);
    ofBackground(0x10);
    ofSetWindowShape(300, 600);
    
    paramGroup.setName("Panel");
    
//    midiIn.openPort("virtualMIDI");
//    midiIn.addListener(this);

    lightMap[1] = "chandelier";

    meters = { 4, 7, 10, 13, 16, 19 };
    
    int count = 1;
    for (auto meter : meters) {
        lightMap[meter] = "meter-" + ofToString(count++);
    }
    
    washAddresses = { 27, 30 };
    
    lightMap[55] = "spot brightness";
    
//    lightMap[90] = "wash-red";
//    lightMap[91] = "wash-green";
//    lightMap[92] = "wash-blue";
    
    stringstream ss;
    for (const auto &pair : lightMap) {
        auto p = shared_ptr<ofParameter<int> >(new ofParameter<int>(ofToString(pair.first) + " : " + pair.second , 0, 0, 255));
        paramGroup.add(*(p.get()));
        channels[pair.first] = p;
    }
    
    autoCycle.set("AutoCycle",false);

    shared_ptr<ofxPanel> panel = make_shared<ofxPanel>();
    panel->setup(paramGroup);
    panel->add(maxMeterVal.set("maxMeterVal", 127, 0, 127));
    panel->add(useDecay.set("use decay", true));
    panel->add(decayAmount.set("decay", 1, 0.95, 1));
    panel->add(washCol.set("wash colour", ofColor::red, ofColor(0), ofColor(255)));
//    panel->add(spotBrightness.set("spotBrightness", 0, 0, 255));


    ofxPanelManager::get().addPanel(panel);
    

    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    something.addParameter<float>("test-float100", 50.0, 0, 100);
    something.addParameter<ofColor>("test-color", ofColor::red, ofColor(0), ofColor(255));
//    something.parameters["test-float100"] = make_shared<ofParameter<float>>(<#_Args &&__args...#>)
    
    pm.setup();
    
    ofxOscCenter::get();
}

void ofApp::update() {
    
    if (useDecay) {
        audioValue *= decayAmount;
        setMeter(audioValue);
    }
    
//    for (auto address : washAddresses) {
//        dmx.setLevel(address, washCol.get().r);
//        dmx.setLevel(address+1, washCol.get().g);
//        dmx.setLevel(address+2, washCol.get().b);
//    }
//    
//    dmx.setLevel(50, 52);
//    dmx.setLevel(52, 86);
////    dmx.setLevel(55, spotBrightness);
//    dmx.setLevel(56, 12);
//    dmx.setLevel(61, 15);
//    dmx.setLevel(62, 255);
//    
//    for (const auto &chan : channels) {
//        dmx.setLevel(chan.first, *chan.second.get());
//    }
//    
//    dmx.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));  
}

void ofApp::draw() {
    
//    panel->draw();

    ofxPanelManager::get().draw();
    
//    something.drawGui();
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    if (msg.status == MIDI_CONTROL_CHANGE) {
        if (msg.control == METER_CONTROL_NUMBER) {
            if (useDecay) {
                if (msg.value > audioValue) {
                    setMeter(msg.value);
                    audioValue = msg.value;
                }
            }
            else {
                setMeter(msg.value);
            }
        }
    }
    else if (msg.status == MIDI_NOTE_ON) {
        for (const auto &pair : lightMap) {
            auto lightIndex = pair.first;
            auto lightName = pair.second;
            if (msg.pitch == lightIndex) {
                channels[lightIndex]->set(msg.velocity * 2);
            }
        }
        if (msg.channel == 3) washCol.set(ofColor(msg.velocity * 2, washCol->g, washCol->b));
        if (msg.channel == 4) washCol.set(ofColor(washCol->r, msg.velocity * 2, washCol->b));
        if (msg.channel == 5) washCol.set(ofColor(washCol->r, washCol->g, msg.velocity * 2));
        
        if (msg.channel == 6) {
            channels[55]->set(msg.velocity*2);
        }
    }
    else if (msg.status == MIDI_NOTE_OFF) {
        for (const auto &pair : lightMap) {
            auto lightIndex = pair.first;
            auto lightName = pair.second;
            if (msg.pitch == lightIndex) {
                channels[lightIndex]->set(0);
            }
        }
        if (msg.channel == 3) washCol.set(ofColor(0, washCol->g, washCol->b));
        if (msg.channel == 4) washCol.set(ofColor(washCol->r, 0, washCol->b));
        if (msg.channel == 5) washCol.set(ofColor(washCol->r, washCol->g, 0));

        if (msg.channel == 6) {
            channels[55]->set(0);
        }
    }
}

// set the 6 meter lights with one MIDI value
void ofApp::setMeter(int value) {
    
    float mappedVal = ofMap(value, 0, maxMeterVal, 0, meters.size(), true);
    if (mappedVal >= 6) {
        mappedVal = 5.999;
    }
    
    for (int i = 0; i < meters.size(); i++) {
        if (i <= mappedVal) {
            channels[meters[i]]->set(255);
        }
        else {
            channels[meters[i]]->set(0);
        }
    }
    
    int notFullIndex = floor(mappedVal);
    float amount = mappedVal - notFullIndex;
    
    channels[meters[notFullIndex]]->set(255 * amount);
    
    if (value > 10) {
        cout << value << " : " << mappedVal << " [ ";
        for (auto index : meters) {
            cout << channels[index]->get() << " ";
        }
        cout << "]" << endl;
    }
    
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {

}
