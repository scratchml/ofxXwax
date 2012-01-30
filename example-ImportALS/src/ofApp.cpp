#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	ofxXmlSettings xml;
	xml.loadFile("ScratchProject/ScratchProject.als");
	
	string context = "Ableton LiveSet MasterTrack MasterChain Mixer CrossFade ArrangerAutomation Events";
	vector<string> tags = ofSplitString(context, " ");
	for(int i = 0; i < tags.size(); i++) {
		xml.pushTag(tags[i]);
	}
	
	all = FloatEvent::loadAll(xml);
	
	ofFile out("out.xml", ofFile::WriteOnly);
	out.setf(ios::fixed, ios::floatfield);
	out.precision(2);
	for(int i = 0; i < all.size(); i++) {
		out << "<event>";
		out << "<t>" <<	all[i].getTime() << "</t>";
		out << "<p>" <<	all[i].getValue() << "</p>";
		out << "</event>" << endl;
	}
}  

void ofApp::update() {

}

void ofApp::draw() {
	ofBackground(170, 178, 183);
	ofSetColor(255, 0, 97);
	ofNoFill();
	ofBeginShape();
	ofSetLineWidth(2);
	float padding = 10;
	float front = 0; //all.front().getTime(); // first value is bad
	float back = all.back().getTime();
	for(int i = 0; i < all.size(); i++) {
		float t = all[i].getTime();
		float v = all[i].getValue();
		float x = ofMap(t, front, back, 0, ofGetWidth());
		float y = ofMap(v, 1, 0, padding, ofGetHeight() - padding);
		ofVertex(x, y);
		ofCircle(x, y, 2);
	}
	ofEndShape();
}