/*
 ofxXwax provides a simple interface to the xwax library.
 All time values are in milliseconds.
*/

#pragma once

#include "ofMain.h"

#ifdef _MSC_VER
#include "timecoder.h"
#else
extern "C" {
#include "timecoder.h"
}
#endif

class ofxXwax {
public:
	ofxXwax();
	~ofxXwax();
	
	// sampleRate is in Hz, bufferSize is in samples. format can be serato_2a,
	// serato_2b, serato_cd, traktor_a, traktor_b, mixvibes_v2, mixvibes_7inch
	void setup(unsigned int sampleRate, unsigned int bufferSize, string format = "serato_2a");
	
	// *input is an interlaced 2-channel audio stream, which means it points to
	// an array that contains (bufferSize * 2) elements.
	void update(float* input);
	
	// if you need to check what the format is for display purposes
	string getFormat() const;
	
	// when the record is playing forward, the pitch is +1.0
	float getPitch() const;
	
	// velocity is the derivative of position, milliseconds since last update
	float getVelocity() const;
	
	// relative position does not change when you move the needle across the vinyl
	float getRelative() const;
	
	// absolute position knows where on the record you are, avoids sticker drift
	float getAbsolute() const;
	
	// absolute position can not always be read. check to see if it's valid
	bool isAbsoluteValid() const;
	
	// convert a time in milliseconds to an angle in degrees. this value is
	// unbounded, so it may be significantly higher than 360. to wrap it from
	// 0-360, use fmodf(ofxXwax::milliToDegrees(t), 360).
	static float millisToDegrees(float millis);
	
protected:
	struct timecoder timecoder;
	unsigned int sampleRate, bufferSize;
	vector<short> shortBuffer;
	float pitch, velocity, relativePosition, absolutePosition;
	bool absoluteValid;
	string format;
};
