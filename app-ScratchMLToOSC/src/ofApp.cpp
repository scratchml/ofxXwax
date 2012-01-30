#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetBackgroundAuto(false);
	
	fader.setAddress("/scratch/mixer/fader");
	osc.setup("localhost", 8319);
	
	int sampleRate = 48000;
	bufferSize = 64;	
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
	
	ofxXmlSettings xml;
	xml.loadFile("example.xml");
	
	xml.pushTag("sml");
	xml.pushTag("performance");
	
	xml.pushTag("turntable");
	record.setAddress("/scratch/record/" + xml.getValue("deck", ""));
	float samplerate = xml.getValue("samplerate", 0.);
	xml.pushTag("data");
	int n = xml.getNumTags("p");
	float sampleLength = 1000. / samplerate;
	for(int i = 0; i < n; i++) {
		float position = xml.getValue("p", 0., i);
		float time = i * sampleLength;
		record.addEvent(position, time);
	}
	xml.popTag();
	xml.popTag();
	
	xml.pushTag("mixer");
	xml.pushTag("fader");
	xml.pushTag("data");
	n = xml.getNumTags("event");
	for(int i = 0; i < n; i++) {
		xml.pushTag("event", i);
		float position = xml.getValue("p", 0.);
		float time = xml.getValue("t", 0.);
		fader.addEvent(position, time);
		xml.popTag();
	}
	xml.popTag();
	xml.popTag();
	xml.popTag();
	
	ofBackground(0);
}

void ofApp::update() {
}

void ofApp::draw() {
	ofFill();
	
	ofSetColor(0);
	float x = ofGetFrameNum() % ofGetWidth();
	ofRect(x, 0, 1, ofGetHeight());
	
	ofSetColor(255);
	if(faderPosition) {
		float y = ofMap(recordPosition, 3000, 5000, ofGetHeight(), 0);
		ofRect(x, y, 1, 1);
	}
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels) {
	if(record.update(osc)) {
		recordPosition = record.getPosition();
	}
	if(fader.update(osc)) {
		faderPosition = fader.getPosition();
	}
}

void ofApp::keyPressed(int key) {
	record.reset();
	fader.reset();
}

void ofApp::mousePressed(int x, int y, int button) {
	record.reset();
	fader.reset();
}