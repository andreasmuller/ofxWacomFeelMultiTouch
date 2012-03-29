
ofxWacomFeelMultiTouch
======================

ofxWacomFeelMultiTouch is an Openframeworks add-on for the Wacom Feel Multi Touch SDK.

OSX only for now, but a Windows version should be possible.

The Wacom Feel SDK only supports a very limited number of tablets, so be very careful when  purchasing a tablet if you are planning on using it with this software.

From http://www.wacomeng.com/touch/faq.htm

- The Feel™ Multi-Touch SDK is available for free from Wacom and works with Wacom multi-finger touch tablets CTH470 and CTH670.



Using it in your program
------------------------

An example program is included, but to start from scratch simply:

- Include the WacomMultiTouch.framework, this will be available on your system once you have installed the drivers for your tablet.

- Run the init() on your ofxWacomFeelMultiTouch object.

- Attach listeners for ofEvents.touchDown, ofEvents.touchUp and ofEvents.touchMoved.
	
Known limitations
-----------------

- Touch area is limited to a sub-area of the touchpad, marked by lines on my Bamboo CTH-470.