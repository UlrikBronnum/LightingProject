#include "MoveCam.h"



MoveCam::MoveCam(float nearClip, float farClip, float movementSpeed)
{
	prevRotX = 0.0f;
	prevRotY = 0.0f;
	speed = movementSpeed;
	setNearClip(nearClip);
	setFarClip(farClip);
	registerEvent();

	setScale(1, 1, 1);
	cameraBody.setScale(1, 1, 1);
}
MoveCam::MoveCam()
{
	prevRotX = 0.0f;
	prevRotY = 0.0f;
	speed = 1000.0f;
	setNearClip(1.0f);
	setFarClip(20000.0f);
	registerEvent();

	setScale(1, 1, 1);
	cameraBody.setScale(1, 1, 1);
}


MoveCam::~MoveCam()
{
}
void MoveCam::setup(float nearClip, float farClip, float movementSpeed)
{
	speed = movementSpeed;
	setNearClip(nearClip);
	setFarClip(farClip);
}
void MoveCam::update()
{
	uint64_t	time = ofGetElapsedTimeMillis();
	
	unsigned int diff = time - lastTime;

	delta = (float)diff / 1000.0f;


	lastTime = time;
}
void MoveCam::keyPressed(ofKeyEventArgs & args)
{

	if (args.key == 357 || args.key == 119) {

		ofVec3f pos = getPosition();
		ofVec3f dir = -getZAxis();
		pos += dir * speed * delta;

		setPosition(pos);
		cameraBody.setPosition(pos);
	}

	if (args.key == 359 || args.key == 115) {
		ofVec3f pos = getPosition();
		ofVec3f dir = getZAxis();

		pos += dir * speed * delta;

		setPosition(pos);
		cameraBody.setPosition(pos);
	}

	if (args.key == 356 || args.key == 97)
	{
		ofVec3f pos = getPosition();
		ofVec3f dir = -getXAxis();
		pos += dir * speed * delta;

		setPosition(pos);
		cameraBody.setPosition(pos);
	}
	if (args.key == 358 || args.key == 100)
	{
		ofVec3f pos = getPosition();
		ofVec3f dir = getXAxis();
		pos += dir * speed * delta;

		setPosition(pos);
		cameraBody.setPosition(pos);
	}

	
}
void MoveCam::mouseMoved(ofMouseEventArgs & args)
{
	prevRotX = float(args.x);
	prevRotY = float(args.y);
}
void MoveCam::mouseDragged(ofMouseEventArgs & args)
{
	if(args.button == 2){
		float size = ofGetWindowHeight();
		float	rotX = (float(args.x) - prevRotX);
		rotate(-rotX / 2, cameraBody.getYAxis());
		cameraBody.rotate(-rotX / 2, cameraBody.getYAxis());
		prevRotX = float(args.x);

		float rotY = float(args.y) - prevRotY;
		rotate(-rotY / 2, getXAxis());
		prevRotY = float(args.y);
	}
}
// Key activate events
void MoveCam::registerEvent()
{
	ofAddListener(ofEvents().keyPressed, this, &MoveCam::keyPressed);
	ofAddListener(ofEvents().mouseDragged, this, &MoveCam::mouseDragged);
	ofAddListener(ofEvents().mouseMoved, this, &MoveCam::mouseMoved);
}

void MoveCam::rotateToPoint(ofVec3f position) 
{ 
	lookAt(position); 
	cameraBody.lookAt(position);
}

void MoveCam::setCamPosition(ofVec3f position)
{
	cameraBody.setGlobalPosition(position + ofVec3f(0, 0, 500));
	setGlobalPosition(position + ofVec3f(0, 0, 500));
}