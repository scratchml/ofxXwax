#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main(int argc, char *argv[]) {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 100, 100, OF_WINDOW);
	ofRunApp(new ofApp());
}
