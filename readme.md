# ofxXwax is an addon for [openFrameworks](http://openframeworks.cc/)

It uses the [xwax](www.xwax.co.uk/) library to decode a variety of digital vinyl control systems (Serato, Traktor, and Torq).

ScratchOSC, An example/app is provided that uses ofxXwax and openFrameworks to send OSC messages describing the position of the record. To customize the data coming from ScratchOSC, modify the file `data/settings.xml`. You can download a binary for OSX [here](https://github.com/downloads/scratchml/ofxXwax/ScratchOSC-osx.zip). The binary has been compiled with a special modification that allows you to use broadcast IP addresses.

If you wire up an Arduino to your crossfader and output a single byte at 115200 baud describing the crossfader position, ScratchOSC will also send the fader clicks.