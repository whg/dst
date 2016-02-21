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
    
    
    ofxMidiIn::listPorts();
    midiIn.openPort("Arturia BeatStep");
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    
    for (int i =0; i < 10; i++) ofxDmxCenter::get().addFixture(make_shared<FadoColumn>());
    for (int i = 0; i < 3; i++) ofxDmxCenter::get().addFixture(make_shared<AnglepoiseSet>());

    
    ofxParameterMapper::get();
    
    ofxOscCenter::get();
    
    ofxDmxCenter::get().assignAddresses();
    
    ofxDmxCenter::get().openFixturesGui();
    
    ofxMidiMapper::get().setup(midiIn);
}

void ofApp::update() {
    
    ofxDmxCenter::get().updateFixtures();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::draw() {
    

    ofxPanelManager::get().draw();
    
}

void ofApp::keyPressed(int key) {
    
    KEY('d', ofxDmxCenter::get().openFixturesGui());
    
    KEY('s', ofxDmxCenter::get().saveFixtureData());
    KEY('l', ofxDmxCenter::get().loadFixtureData());
    
    KEY('f', ofToggleFullscreen());
}

void ofApp::keyReleased(int key) {

}
