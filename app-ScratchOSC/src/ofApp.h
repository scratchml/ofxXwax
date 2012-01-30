#pragma once

#include "ofMain.h"
#include "ofxXwax.h"

#include "ofxXmlSettings.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp {
public:
	void loadSettings();
	void setup();
	void update();
	void draw();
	
	void audioIn(float* input, int bufferSize, int nChannels); 
	
	ofxXwax xwax;
	ofxOscSender osc;
	ofSerial serial;
	
	bool serialReady;
	unsigned char serialThreshold;
	bool overThreshold;
	
	string recordFormat, recordSide, oscHost, serialPort;
	unsigned int oscPort, oscSubdivide, audioSamplerate, audioBuffersize;
	float oscRate;
	bool oscPitch, oscRelative, oscDegrees;
	
	void drawVinyl(float x, float y, float scale);
	void drawAudioInput(float x, float y, float scale);
	vector<float> middleAudioBuffer, frontAudioBuffer;
	ofMutex audioMutex;
	
	void drawCurve(deque<float> curve, float scale, float min = 0, float max = 0);
	deque<float> absolutePosition, relativePosition, faderPosition;
	
	unsigned int audioFrame;
};
