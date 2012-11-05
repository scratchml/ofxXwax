#include "ofxXwax.h"

const int nChannels = 2;
const float msPerSecond = 1000;
const float msPerRotation = 1800;
const float invalidPosition = -1;
	
ofxXwax::ofxXwax()
:absolutePosition(0)
,absoluteValid(false)
,relativePosition(0) {
}

ofxXwax::~ofxXwax() {
	timecoder_clear(&timecoder); // class
	//timecoder_free_lookup(); // static
}

void ofxXwax::setup(unsigned int sampleRate, unsigned int bufferSize, string format) {
	this->sampleRate = sampleRate;
	this->bufferSize = bufferSize;
	this->format = format;
	
    cout<<bufferSize<<endl;
	float speed = 1.0; // 1.0 is 33 1/3, 1.35 is 45 rpm
    struct timecode_def *timecoderDef;
    timecoderDef = timecoder_find_definition("serato_cd");
	timecoder_init(&timecoder, timecoderDef, speed, sampleRate);
    //timecoder_init(&timecoder, <#struct timecode_def *def#>, speed, sampleRate);
	
	shortBuffer.resize(nChannels * bufferSize);
}

void ofxXwax::update(float* input) {
	// convert from -1 to 1 to a 16-byte signed short integer
	for (int i = 0; i < bufferSize*nChannels; i++) {
		shortBuffer[i] = input[i] * (1<<15);
	}
	
	timecoder_submit(&timecoder, &shortBuffer[0], bufferSize*nChannels);
	
	double when;
	float curPosition = timecoder_get_position(&timecoder, &when);
	cout<<curPosition<<endl;
	pitch = timecoder_get_pitch(&timecoder);
	velocity = (msPerSecond * bufferSize / sampleRate) * pitch;
	relativePosition += velocity;

	if(curPosition == invalidPosition) {
		absoluteValid = false;
		absolutePosition += velocity;
	} else {
		absoluteValid = true;
		absolutePosition = curPosition;
	}
}

string ofxXwax::getFormat() const {
	return format;
}

float ofxXwax::getPitch() const {
	return pitch;
}

float ofxXwax::getVelocity() const {
	return velocity;
}

float ofxXwax::getRelative() const {
	return relativePosition;
}

float ofxXwax::getAbsolute() const {
	return absolutePosition;
}

bool ofxXwax::isAbsoluteValid() const {
	return absoluteValid;
}

float ofxXwax::millisToDegrees(float millis) {
	return (millis / msPerRotation) * 360;
}
