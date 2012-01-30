# ofxXwax is an addon for [openFrameworks](http://openframeworks.cc/)

It uses the [xwax](www.xwax.co.uk/) library to decode a variety of digital vinyl control systems (Serato, Traktor, and Torq). The ofxXwax repository also contains some tools surrounding [ScratchML](http://scratchml.com/).

## ScratchOSC

ScratchOSC, An example/app is provided that uses ofxXwax and openFrameworks to send OSC messages describing the position of the record. To customize the data coming from ScratchOSC, modify the file `data/settings.xml`. You can download a binary for OSX [here](https://github.com/downloads/scratchml/ofxXwax/ScratchOSC-osx.zip). The binary has been compiled with a special modification that allows you to use broadcast IP addresses.

If you wire up an Arduino to your crossfader and output a single byte at 115200 baud describing the crossfader position, ScratchOSC will also send the fader clicks.

## AudioToScratchML and ImportALS

These two examples should be combined into a single app, they were written before ofxXwax. AudioToScratchML will convert a recording of a scratching performance to ScratchML positional data. ImportALS will take an Ableton Live set exported from Serato Scratch Live and convert the fader clicks into ScratchML position/time data.

AudioToScratchML requires libsndfile to be installed. To get libsndfile running, follow the instructions [here](http://ofdsp.blogspot.com/2011/07/installing-libsndfile-with-xcode.html).

## ScratchMLToOSC

This app will stream a ScratchML file over OSC. Click the app to re-trigger the playback. ScratchMLToOSC also contains an `example.xml` ScratchML file.