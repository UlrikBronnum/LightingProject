#pragma once

#include "ofMain.h"

class MoveCam : public ofCamera
{
public:
	MoveCam(float nearClip, float farClip, float movementSpeed);
	MoveCam();
	~MoveCam();

	void setup(float nearClip, float farClip, float movementSpeed);
	void update();

	// Mouse interactions
	void mouseMoved(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	// Key interactions
	void keyPressed(ofKeyEventArgs & args);


	void rotateToPoint(ofVec3f position);
	void setCamPosition(ofVec3f position);

	void setSpeed(float speed) { this->speed = speed; }

protected:

	

	// Key activate events
	void registerEvent();

	//ofCamera	camera;
	ofNode		cameraBody;
	
	uint64_t	lastTime;

	float		delta;

	float		speed;
	float		prevRotX;
	float		prevRotY;
};

