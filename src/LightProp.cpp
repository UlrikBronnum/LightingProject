#include "LightProp.h"



LightProp::LightProp()
{
	lightShader.load("res/shaders/lightShader.vert", "res/shaders/lightShader.frag");
	setLightPosition(0, 0, 0);
	setLightType(POINT_LIGHT);
	setLightDiffuseColor(1, 1, 1);
	setLightSpecularColor(1, 1, 1);
	setLightIntencity(1.0f);
	setAttenuationConst(1.0f, 0.0f, 0.0f);

	lightMesh = ofMesh::sphere(10.0f * lightIntencity, 36, OF_PRIMITIVE_TRIANGLES);
}
LightProp::~LightProp()
{
}

void LightProp::draw(ofVec3f cameraPosition)
{
	ofMatrix4x4 traslation;
	traslation.makeTranslationMatrix(lightPosition.x, lightPosition.y, lightPosition.z);
	
	ofPushMatrix();

	lightShader.begin();
	lightShader.setUniformMatrix4f("translationMatrix", traslation);
	lightShader.setUniform3f("lightColor", lightDiffuseColor);
	lightShader.setUniform1f("lightIntencity", lightIntencity);
	lightShader.setUniform3f("eyePositionWorld", cameraPosition);

	lightMesh.draw();

	lightShader.end();

	ofPopMatrix();
}
void LightProp::setLightPosition(float x, float y, float z)
{
	lightPosition = ofVec4f(x, y, z, lightPosition.w);
}
void LightProp::setLightType(LightTypes type)
{
	if (DIRECTIONAL_LIGHT == type)
		lightPosition.w = 0.0f;
	else
		lightPosition.w = 1.0f;
}
void LightProp::setLightDiffuseColor(float r, float g, float b)
{
	lightDiffuseColor = ofVec3f(r, g, b);
}

void LightProp::setLightSpecularColor(float r, float g, float b)
{
	lightSpecularColor = ofVec3f(r, g, b);
}

void LightProp::setLightIntencity(float intencity)
{
	lightIntencity = intencity;
}

void LightProp::setAttenuationConst(float constant, float linear, float quadratic)
{
	lightAttenuationConst = ofVec3f(constant, linear, quadratic);
}

void LightProp::loadLight(ofShader& shader)
{

}