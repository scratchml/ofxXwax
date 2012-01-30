#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main(int argc, char *argv[]) {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 360, OF_WINDOW);
	ofRunApp(new ofApp());
}
