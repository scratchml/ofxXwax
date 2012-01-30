#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

class TimedEvents {
protected:
	unsigned long offsetTime;
	vector<unsigned long> targetTimes;
	vector<double> targetPositions;
	int index;
	string address;
public:
	TimedEvents()
	:offsetTime(0)
	,index(0) {
		reset();
	}
	void setAddress(string address) {
		this->address = address;
	}
	bool update(ofxOscSender& osc) {
		unsigned long curTime = ofGetSystemTime();
		unsigned long relativeTime = curTime - offsetTime;
		bool newMessage = false;
		while(index < targetTimes.size() && targetTimes[index] < relativeTime) {
			ofxOscMessage message;
			message.setAddress(address);
			message.addFloatArg(targetPositions[index]);
			osc.sendMessage(message);
			index++;
			newMessage = true;
		}
		return newMessage;
	}
	double getPosition() {
		if(targetPositions.size() > 0) {
			return targetPositions[ofClamp(index, 0, targetPositions.size() - 1)];
		} else {
			return 0;
		}
	}
	void reset() {
		offsetTime = ofGetSystemTime();
		index = 0;
	}
	void addEvent(double position, double time) {
		targetTimes.push_back(time);
		targetPositions.push_back(position);
	}
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	unsigned long bufferSize;
	void audioOut(float* output, int bufferSize, int nChannels);
	
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	
	TimedEvents record, fader;
	ofSoundStream soundStream;
	
	float recordPosition, faderPosition;
	
	ofxOscSender osc;
};
