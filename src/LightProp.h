#pragma once

#include "ofMain.h"

enum LightTypes
{
	DIRECTIONAL_LIGHT = 0,
	POINT_LIGHT
};
class LightProp
{
public:
	LightProp();
	~LightProp();

	void draw(ofVec3f cameraPosition);

	void setLightPosition(float x, float y, float z);
	ofVec4f getLightPosition() { return lightPosition; }
	void setLightType(LightTypes type);

	void setLightDiffuseColor(float r, float g, float b);
	ofVec3f getLightDiffuseColor() { return lightDiffuseColor; }
	
	void setLightSpecularColor(float r, float g, float b);
	ofVec3f getLightSpecularColor() { return lightSpecularColor; }

	void setLightIntencity(float intencity);
	float getLightIntencity() { return lightIntencity; }

	void setAttenuationConst(float constant, float linear, float quadratic);
	ofVec3f getAttenuationConst() { return lightAttenuationConst; }

	void loadLight(ofShader& shader);


	void reloadShader(){ lightShader.load("res/shaders/lightShader.vert", "res/shaders/lightShader.frag"); }


protected:

	//	Light
	ofVec4f			lightPosition;
	ofVec3f			lightDiffuseColor;
	ofVec3f			lightSpecularColor;
	float			lightIntencity;
	ofVec3f			lightAttenuationConst;
	

	//  Light shader
	ofShader		lightShader;

	//  Light Mesh
	ofMesh			lightMesh;

};

