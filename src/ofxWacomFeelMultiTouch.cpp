
#include "ofxWacomFeelMultiTouch.h"

#pragma mark -
#pragma mark Wacom Touch API C-Function Callbacks
#pragma mark -

//--------------------------------------------------------------
// Purpose:	A new device was connected.
//
void ofxWacomFeelMultiTouchAttachCallback(WacomMTCapability deviceInfo, void *userInfo)
{
	ofxWacomFeelMultiTouch* wacomFeelObj = (ofxWacomFeelMultiTouch* )userInfo;
	wacomFeelObj->_deviceAttached( deviceInfo );	
}



//--------------------------------------------------------------
// Purpose:	A device was unplugged.
//
void ofxWacomFeelMultiTouchDetachCallback(int deviceID, void *userInfo)
{
	ofxWacomFeelMultiTouch* wacomFeelObj = (ofxWacomFeelMultiTouch* )userInfo;
	wacomFeelObj->_deviceDetached( deviceID );	
}



//--------------------------------------------------------------
// Purpose:	Fingers are moving on one of the connected devices.
//
int ofxWacomFeelMultiTouchFingerCallback(WacomMTFingerCollection *fingerPacket, void *userInfo)
{
	ofxWacomFeelMultiTouch* wacomFeelObj = (ofxWacomFeelMultiTouch* )userInfo;
	wacomFeelObj->_fingerCallBack( fingerPacket );		
		
	return 0;
}

#pragma mark -
#pragma mark ofxWacomFeel Functions
#pragma mark -

//--------------------------------------------------------------
ofxWacomFeelMultiTouch::ofxWacomFeelMultiTouch()
{
	coordinateWidth		= 1.0f;
	coordinateHeight	= 1.0f;	
}


//--------------------------------------------------------------
ofxWacomFeelMultiTouch::~ofxWacomFeelMultiTouch()
{
}

//--------------------------------------------------------------
bool ofxWacomFeelMultiTouch::init()
{
	bool wasInitialised = false;
	
	// From the Wacom SDK Example: 
	// The WacomMultiTouch framework is weak-linked. That means the application 
	// can load if the framework is not present. However, we must take care not 
	// to call framework functions if the framework wasn't loaded. 
	//
	// You can set WacomMultiTouch.framework to be weak-linked in your own 
	// project by opening the Info window for your target, going to the General 
	// tab. In the Linked Libraries list, change the Type of 
	// WacomMultiTouch.framework to "Weak". 
	
	if(WacomMTInitialize != NULL)
	{
		WacomMTError err = WacomMTInitialize(WACOM_MULTI_TOUCH_API_VERSION);
		
		if(err == WMTErrorSuccess)
		{
			ofLogNotice() << "We have initialised the Wacom SDK.";
			
			// Listen for device connect/disconnect.
			// Note that the attach callback will be called for each connected device 
			// immediately after the callback is registered. 
			WacomMTRegisterAttachCallback(ofxWacomFeelMultiTouchAttachCallback, this);
			WacomMTRegisterDetachCallback(ofxWacomFeelMultiTouchDetachCallback, this);
			
			int   deviceIDs[MAX_ATTACHED_DEVICES]  = {};
			int   deviceCount    = 0;
			int   counter        = 0;
			
			// Add a viewer for each device's data
			deviceCount = WacomMTGetAttachedDeviceIDs(deviceIDs, sizeof(deviceIDs));
			if(deviceCount > MAX_ATTACHED_DEVICES)
			{
				// With a number as big as 30, this will never actually happen.
				ofLogError() << "More tablets connected than would fit in the supplied buffer. Will need to reallocate buffer!";
			}
			else
			{
				for(counter = 0; counter < deviceCount; counter++)
				{
					WacomMTRegisterFingerReadCallback(deviceIDs[counter], NULL, WMTProcessingModeNone, ofxWacomFeelMultiTouchFingerCallback, this);
				}
				
				wasInitialised = true;
			}
			
		}
	}
	else
	{
		// WacomMultiTouch.framework is not installed.
		ofLogError() << "ofxWacomFeel::init() was not able to initialize the Wacom SDK.";
	}

	coordinateWidth		= ofGetWidth();
	coordinateHeight	= ofGetHeight();	
	
	return wasInitialised;
}

//--------------------------------------------------------------
void ofxWacomFeelMultiTouch::_deviceAttached( WacomMTCapability deviceInfo )
{
	ofLogVerbose() << "ofxWacomFeelMultiTouch::_deviceAttached";
}

//--------------------------------------------------------------
void ofxWacomFeelMultiTouch::_deviceDetached( int deviceID )
{
	ofLogVerbose() << "ofxWacomFeelMultiTouch::_deviceDetached";	
}

//--------------------------------------------------------------
void ofxWacomFeelMultiTouch::_fingerCallBack( WacomMTFingerCollection *fingerPacket )
{
	
	//cout << "fingerPacket->FingerCount: " << fingerPacket->FingerCount << endl;
	
	int timeMillis = ofGetElapsedTimeMillis();
	
	for( int fingerIndex = 0; fingerIndex < fingerPacket->FingerCount; fingerIndex++ )
	{
		WacomMTFinger* finger = &fingerPacket->Fingers[fingerIndex];
		
		// From http://www.wacomeng.com/touch/WacomFeelMulti-TouchAPI.htm
		// Confidence: If true the driver believes this is a valid touch from a finger.  
		// If false the driver thinks this may be an accidental touch, forearm or palm.
		
		if( finger->Confidence )
		{
			ofTouchEventArgs touchEventArgs;
			touchEventArgs.id 			= finger->FingerID;
			touchEventArgs.time 		= timeMillis;
			touchEventArgs.x			= finger->X;
			touchEventArgs.y			= finger->Y;			
			touchEventArgs.numTouches	= fingerPacket->FingerCount; // is this valid?
			touchEventArgs.width		= finger->Width;
			touchEventArgs.height		= finger->Height;
			touchEventArgs.angle		= finger->Orientation; // From the manual, Orientation: The orientation of the contact point in degrees. (Always 0 on my Bamboo touch)
			touchEventArgs.minoraxis	= 0;
			touchEventArgs.majoraxis	= 0;
			touchEventArgs.pressure		= 0;
			touchEventArgs.xspeed		= 0;
			touchEventArgs.yspeed		= 0;
			touchEventArgs.xaccel		= 0;
			touchEventArgs.yaccel		= 0;
			
			touchEventArgs.x			*= coordinateWidth;
			touchEventArgs.y			*= coordinateHeight;			

			touchEventArgs.width		*= coordinateWidth;
			touchEventArgs.height		*= coordinateHeight;		

			if( finger->TouchState == WMTFingerStateNone )
			{
				//cout << "State None" << endl;
			}
			else if( finger->TouchState == WMTFingerStateDown )
			{
				ofNotifyEvent( ofEvents.touchDown, touchEventArgs );
			}
			else if( finger->TouchState == WMTFingerStateHold )
			{
				ofNotifyEvent( ofEvents.touchMoved, touchEventArgs );				
			}	
			else if( finger->TouchState == WMTFingerStateUp )
			{					
				ofNotifyEvent( ofEvents.touchUp, touchEventArgs );					
			}
		}
	}
}


//--------------------------------------------------------------
// We scale the normalized input by these coordinates
void ofxWacomFeelMultiTouch::setCoordinateDimensions( float _w, float _h )
{
	coordinateWidth  = _w;
	coordinateHeight = _h;
}

//--------------------------------------------------------------
void ofxWacomFeelMultiTouch::listAttachedDevices()
{
	int   deviceIDs[MAX_ATTACHED_DEVICES]  = {};
	int   deviceCount    = 0;
	int   counter        = 0;
	
	// Ask the Wacom API for all connected touch API-capable devices.
	// Pass a comfortably large buffer so you don't have to call this method 
	// twice. 
	deviceCount = WacomMTGetAttachedDeviceIDs(deviceIDs, sizeof(deviceIDs));
	
	if(deviceCount > MAX_ATTACHED_DEVICES)
	{
		// With a number as big as MAX_ATTACHED_DEVICES, this will never actually happen.
		ofLogError() << "More tablets connected than would fit in the supplied buffer. Will need to reallocate buffer!";
	}
	else
	{		
		for(counter = 0; counter < deviceCount; counter++)
		{
			int                  deviceID       = deviceIDs[counter];
			WacomMTCapability    capabilities   = {};
			
			WacomMTGetDeviceCapabilities(deviceID, &capabilities);
			
			cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
			cout << "Device ID: " << deviceID << "  "; 
			if( capabilities.Type == WMTDeviceTypeIntegrated ) 
			{ 
				cout << "Type: Integrated." << endl; 
			} 
			else if( capabilities.Type == WMTDeviceTypeOpaque ) 
			{ 
				cout << "Type: Opaque." << endl; 
			} 
			else 
			{ 
				cout << "Type: Unknown." << endl; 
			}
			cout << "Max Fingers: " << capabilities.FingerMax << "  Scan size: " << capabilities.ReportedSizeX << ", " << capabilities.ReportedSizeY << endl;
			cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		}
	}

}


