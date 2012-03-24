#pragma once

#include "ofMain.h"
#include <WacomMultiTouch/WacomMultiTouch.h>

#define MAX_ATTACHED_DEVICES 30

class ofxWacomFeelMultiTouch 
{

	public:

		ofxWacomFeelMultiTouch();
		~ofxWacomFeelMultiTouch();	
	
		bool init();
		void setCoordinateDimensions( float _w, float _h ); // What do we want the max width and height returned to be?
			
		void listAttachedDevices();
	
		void _deviceAttached( WacomMTCapability deviceInfo );
		void _deviceDetached( int deviceID );
	
		void _fingerCallBack( WacomMTFingerCollection *fingerPacket );
	
	private:	
	
		float coordinateWidth;
		float coordinateHeight;	
};
