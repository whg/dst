#include "ofApp.h"

const int METER_CONTROL_NUMBER = 60;

#include "ofxPanelManager.h"
#include "ofxOscCenter.h"
#include "ofxMidiMapper.h"
#include "ofxDmxCenter.h"
#include "ofxParameterMapper.h"
#include "Fixture.h"
#include "ofxTools.h"

void ofApp::setup() {
    ofBackground(0x10);
    ofSetWindowShape(1000, 800);
//    ofSetEscapeQuitsApp(false);

    ofxMidiIn::listPorts();
    midiIn.openPort("Arturia BeatStep");
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    
    for (int i = 0; i < 5; i++) {
        auto fadoColumn = make_shared<FadoColumn>();
        fadoColumn->setDmxStartAddress(i * 9 + 1);
        fadoColumn->setIsFixedAddress(true);
        ofxDmxCenter::get().addFixture(fadoColumn);
    }
    for (int i = 0; i < 5; i++) {
        auto fadoColumn = make_shared<FadoColumn>();
        fadoColumn->setDmxStartAddress(i * 9 + 49);
        fadoColumn->setIsFixedAddress(true);
        ofxDmxCenter::get().addFixture(fadoColumn);
    }
    
    for (int i = 0; i < 3; i++) ofxDmxCenter::get().addFixture(make_shared<AnglepoiseSet>());

    ofxDmxCenter::get().addFixture(make_shared<Stairs>("stairs", 3 * 48 + 1));
    
//    ofxDmxCenter::get().addFixture(make_shared<SingleLED>("umbrella", 97));
    ofxDmxCenter::get().addFixture(make_shared<Hob>("hob", 97));

    ofxDmxCenter::get().addFixture(make_shared<Bathroom>("bathroom", 193));
    ofxDmxCenter::get().addFixture(make_shared<Bulkheads>("bulkheads", 199));
    ofxDmxCenter::get().addFixture(make_shared<Overheads>("overheads", 271));
    ofxDmxCenter::get().addFixture(make_shared<Pendants>("pendants", 274));
    ofxDmxCenter::get().addFixture(make_shared<MainFloor>("mainfloor", 288 + 1));
//    ofxDmxCenter::get().addFixture(make_shared<MainFloor>("mainfloor", 244));
    
    ofxDmxCenter::get().addFixture(make_shared<SingleLED>("icecream", 7*48 + 15 * 3 + 1));
    ofxDmxCenter::get().addFixture(make_shared<TableSet>("tableset", 241));
    ofxDmxCenter::get().addFixture(make_shared<RGBFixture>("desk", 7*48 + 14 * 3 + 1));
    ofxDmxCenter::get().addFixture(make_shared<RGBFixture>("novelty", 268));


    ofxParameterMapper::get();
    
    ofxOscCenter::get();
    
    ofxDmxCenter::get().assignAddresses();
    
    ofxDmxCenter::get().openFixturesGui();
    
    ofxMidiMapper::get().setup(midiIn);
    ofxMidiMapper::get().load();
    
    
    ofAddListener(ofxOscCenter::newMessageEvent, this, &ofApp::newOscMessage);
}

void ofApp::update() {

//    ofxDmxCenter::get().updateFixtures();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::draw() {
    

    ofxPanelManager::get().draw();
}

void ofApp::keyPressed(int key) {
    
    KEY('d', ofxDmxCenter::get().openFixturesGui());
    
    KEY('s', {
//        ofxDmxCenter::get().saveFixtureData();
        ofxMidiMapper::get().save();
    });
    KEY('l', {
//        ofxDmxCenter::get().loadFixtureData();
        ofxMidiMapper::get().load();
    });
    
    KEY('f', ofToggleFullscreen());
}

void ofApp::keyReleased(int key) {

}

void ofApp::newOscMessage(ofxOscCenterNewMessageArgs &args) {

    auto message = args.message;
    auto address = message.getAddress();
    if (address.find("/MIDI/note") != string::npos) {
        ofxMidiMessage midiMessage;
        midiMessage.pitch = message.getArgAsInt(1);
        midiMessage.velocity = message.getArgAsInt(2);
        if (midiMessage.velocity > 0) midiMessage.status = MIDI_NOTE_ON;
        else midiMessage.status = MIDI_NOTE_OFF;
        
        ofxMidiMapper::get().newMidiMessage(midiMessage);
        
        cout << "new midi with velocity " << midiMessage.velocity << endl;
    }
    else if (address.find("/MIDI/cc") != string::npos) {
        ofxMidiMessage midiMessage;
        midiMessage.control = message.getArgAsInt(1);
        midiMessage.value = message.getArgAsInt(2);
        midiMessage.status = MIDI_CONTROL_CHANGE;
        
        ofxMidiMapper::get().newMidiMessage(midiMessage);
    }
    
}
