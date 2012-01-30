#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

class FloatEvent {
public:
	FloatEvent(ofxXmlSettings& xml, int which) {
		stringstream TimeStr(xml.getAttribute("FloatEvent", "Time", "", which));
		stringstream ValueStr(xml.getAttribute("FloatEvent", "Value", "", which));
		TimeStr >> Time;
		ValueStr >> Value;
	}
	static vector<FloatEvent> loadAll(ofxXmlSettings& xml) {
		vector<FloatEvent> all;
		int n = xml.getNumTags("FloatEvent");
		for(int i = 0; i < n; i++) {
			all.push_back(FloatEvent(xml, i));
		}
		return all;
	}
	double getTimeRaw() {
		return Time;
	}
	double getValueRaw() {
		return Value;
	}
	double getValue() {
		return ofClamp(Value, 0, 1);
	}
	double getTime() {
		return MAX(0, 1000 * (Time / 2));
	}
private:
	double Time, Value;
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	vector<FloatEvent> all;
};
