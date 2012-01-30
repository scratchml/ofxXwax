#pragma once

#include "ofMain.h"

#ifdef _MSC_VER
#include "timecoder.h"
#else
extern "C" {
#include "timecoder.h"
}
#endif

#define MIXXX_VINYL_SCOPE_SIZE 100

class ofApp : public ofBaseApp {
public:
	void setup();
	void exit();
	void update();
	void draw();
	
	unsigned long bufferSize, bufferFrames, bufferPosition, ttmPosition;
	ofSoundStream soundStream;
	vector<float> floatBuffer;
	vector<short> curBuffer;
	vector<float> pitchTtm, relativeTtm, absoluteTtm;
	vector<float> wholeBuffer;
	float relativePosition, absolutePosition;
	void audioOut(float* output, int bufferSize, int nChannels);
	
	float getPitch();
	float getRelative();
	float getAbsolute();
	
	struct timecoder_t timecoder;
	
	bool exporting;
	void exportXml();
};
