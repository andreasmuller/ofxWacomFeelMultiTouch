#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(0, 0, 0);
	
	ofSetLogLevel( OF_LOG_VERBOSE );
	
	wacomTablet.init();
	
	wacomTablet.listAttachedDevices();
	
	ofAddListener(ofEvents.touchDown, 	this, &testApp::touchDown);
	ofAddListener(ofEvents.touchUp, 	this, &testApp::touchUp);
	ofAddListener(ofEvents.touchMoved, 	this, &testApp::touchMoved);
	
	maxFingers = 100; // When you put your palms on the pad it registers odd touches
	fingers = new Finger[maxFingers];
	for( int i = 0; i < maxFingers; i++ )
	{
		fingers[i].active = false;
		fingers[i].angle  = 0.0f;	
		//fingers[i].color;			
	}
	
	fontSmall.loadFont("DIN.otf", 12);
}

//--------------------------------------------------------------
void testApp::update()
{

}

//--------------------------------------------------------------
void testApp::draw()
{
	float dimensionScale = 5.0f;
	
	for( int i = 0; i < maxFingers; i++ )
	{
		if( fingers[i].active )
		{
			// Scale the width and height up a little bit
			float tmpW = fingers[i].dimensions.x*dimensionScale;
			float tmpH = fingers[i].dimensions.y*dimensionScale;
			
			ofSetColor(fingers[i].color);
			ofEllipse(fingers[i].pos, tmpW, tmpH );

			ofSetColor(255,255,255);	
			fontSmall.drawString( ofToString(i), fingers[i].pos.x + tmpW*0.7, fingers[i].pos.y + 5 );
			
			// The angle field is always 0 on my Bamboo Touch
			/*
			ofPushMatrix();
				ofTranslate(fingers[i].pos);
				ofRotate(fingers[i].angle);
				ofLine( -10, 0, 10, 0);
			ofPopMatrix();
			 */
		}
	}
	
	ofSetColor( 255, 255, 255 );	
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch) 
{
	if( touch.id < maxFingers )	
	{
		int i = touch.id;
		fingers[i].active = true;
		fingers[i].angle  = 0.0f;	
		fingers[i].color.setHsb(ofRandom(255.0f), 255.0f, 255.0f);
	}
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch) 
{
	//cout << "Touch MOVED " <<  touch.id << endl;
	if( touch.id < maxFingers )	
	{
		int i = touch.id;

		fingers[i].pos.set(touch.x, touch.y);	
		fingers[i].dimensions.set( touch.width, touch.height );
		fingers[i].angle  = touch.angle;			
	}
	
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch) 
{
	//cout << "Touch UP " <<  touch.id << endl;
	if( touch.id < maxFingers )	
	{
		int i = touch.id;
		fingers[i].active = false;
	}	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{	
}