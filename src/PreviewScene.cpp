#include "PreviewScene.h"



PreviewScene::PreviewScene()
{
	rotationY = 0.0f;

	lightDistanceFromObject = 0;
	pointLight.setLightPosition(0.0f, 0.0f, 500.0f);

	doDisplacement = false;
}


PreviewScene::~PreviewScene()
{
}
//--------------------------------------------------------------
void PreviewScene::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	lightMenu(style, font, fontColor, width, height);
	meshMenu(style, font, fontColor, width, height);

	reloadShader();
	reloadMesh();
}
//--------------------------------------------------------------
void PreviewScene::update()
{
	if (displacementSelection.getValue() != doDisplacement)
	{
		if (!doDisplacement) {
			meshControls.addGuiFloat(displacementScale, "Displacement", 256, 24, 0.0, 255.0, 0.01);
			meshControls.setElementPosition(0, 24 * 5 + 1 * 5, 5);
		}
		else {
			meshControls.eraseLastElement();
		}
		
		doDisplacement = displacementSelection.getValue();
		reloadShader();
		reloadMesh();
	}
	set3DModel();

	ofMatrix4x4 traslation;
	traslation.makeTranslationMatrix(camera.getPosition().x, camera.getPosition().y, meshPositionZ);
	ofMatrix4x4 rotation;
	rotation.makeRotationMatrix(0, ofVec3f(1, 0, 0), rotationY * 180, ofVec3f(0, 1, 0), 180, ofVec3f(0, 0, 1));

	transformation = rotation * traslation;

	pointLight.setLightDiffuseColor(lightColorRed.getValue(), 
									lightColorGreen.getValue(), 
									lightColorBlue.getValue());

	pointLight.setLightSpecularColor(lightColorRed.getValue(), 
									lightColorGreen.getValue(), 
									lightColorBlue.getValue());

	pointLight.setAttenuationConst(	lightAttenuationConst.getValue(), 
									lightAttenuationLinear.getValue(),
									lightAttenuationQuadratic.getValue());

	pointLight.setLightIntencity(	lightColorIntencity.getValue());

}
//--------------------------------------------------------------
void PreviewScene::draw(ofTexture& albido, ofTexture& normals, ofTexture& specular, ofTexture& heights)
{
	if (doDisplacement)
	{
		drawWithDisplacement(albido, normals, specular, heights);
	}
	else
	{
		drawWithNormalmap(albido, normals, specular);
	}
}
//--------------------------------------------------------------
void PreviewScene::drawControls(int x, int y)
{
	lightControls.draw(x, y);
	meshControls.draw(x, y + lightControls.getSize().y);
}
//--------------------------------------------------------------
void PreviewScene::drawLightControls(int x, int y)
{
	lightControls.draw(x, y);
}
//--------------------------------------------------------------
void PreviewScene::drawMeshControls(int x, int y)
{
	meshControls.draw(x, y);
}
//--------------------------------------------------------------
void PreviewScene::mouseMoved(int x, int y) {
	prevMousePos.x = x;
	prevMousePos.y = y;
}

//--------------------------------------------------------------
void PreviewScene::mouseDragged(int x, int y, int button) {
	if (x > 256 && 
		!lightControls.isPointInsideElement(x, y, lightControls.getSize().x, lightControls.getSize().y) &&
		!meshControls.isPointInsideElement(x, y, meshControls.getSize().x, meshControls.getSize().y) )

	{
		ofVec3f lightPosition;
		if (button == 0)
		{
			float lightX = (x - ofGetWindowWidth() / 2) * 0.66f;
			float lightY = (y - ofGetWindowHeight() / 2) * 0.66f;

			lightPosition = ofVec4f(camera.getPosition().x + lightX, camera.getPosition().y - lightY, lightDistanceFromObject, 1.0f);
			pointLight.setLightPosition(lightPosition.x, lightPosition.y, lightPosition.z);
		}
		if (button == 1)
		{
			lightDistanceFromObject = lightDistanceFromObject + (y-prevMousePos.y);
			lightPosition = pointLight.getLightPosition();
			lightPosition = ofVec4f(lightPosition.x, lightPosition.y, lightDistanceFromObject, 1.0f);
			pointLight.setLightPosition(lightPosition.x, lightPosition.y, lightPosition.z);
			
		}
		if (button == 2) 
		{
			rotationY = rotationY + float(prevMousePos.x-x) / float(ofGetWindowWidth() / 2.0f);
		}
	}
	prevMousePos.x = x;
	prevMousePos.y = y;
}
//--------------------------------------------------------------
void PreviewScene::set3DModel()
{
	if (meshState != 0 && boxMesh.getState()) 
	{
		planeMesh.setState(false);
		sphereMesh.setState(false);
		cylinderMesh.setState(false);

		meshState = 0;
		reloadMesh();
	}
	else if (meshState != 1 && planeMesh.getState())
	{
		boxMesh.setState(false);
		sphereMesh.setState(false);
		cylinderMesh.setState(false);

		meshState = 1;
		reloadMesh();
	}
	else if (meshState != 2 && sphereMesh.getState())
	{
		boxMesh.setState(false);
		planeMesh.setState(false);
		cylinderMesh.setState(false);

		meshState = 2;
		reloadMesh();
	}
	else if (meshState != 3 && cylinderMesh.getState())
	{
		boxMesh.setState(false);
		planeMesh.setState(false);
		sphereMesh.setState(false);
		
		meshState = 3;
		reloadMesh();
	}
}

//--------------------------------------------------------------
void PreviewScene::drawWithNormalmap(ofTexture& albido, ofTexture& normals, ofTexture& specular)
{
	textureWidth = albido.getWidth();
	textureHeight = albido.getHeight();

	ofEnableDepthTest();

	camera.begin();
	ofPushMatrix();
	shader.begin();
	
	shader.setUniformTexture("diffuseTexture", albido, 1);
	shader.setUniformTexture("normalTexture", normals, 2);
	shader.setUniformTexture("specularTexture", specular, 3);

	shader.setUniform4f("sceneLightPosition", pointLight.getLightPosition());
	shader.setUniform3f("sceneLightDiffuseColor", pointLight.getLightDiffuseColor());
	shader.setUniform3f("sceneLightSpecularColor", pointLight.getLightSpecularColor());
	shader.setUniform3f("sceneLightAttenuation", pointLight.getAttenuationConst());
	shader.setUniform1f("sceneLightIntencity", pointLight.getLightIntencity());

	shader.setUniform3f("eyePositionWorld", camera.getGlobalPosition());
	shader.setUniformMatrix4f("translationMatrix", transformation);

	mesh.draw();

	shader.end();

	ofPopMatrix();

	pointLight.draw(camera.getGlobalPosition());
	camera.end();

	ofDisableDepthTest();
}
//--------------------------------------------------------------
void PreviewScene::drawWithDisplacement(ofTexture& albido, ofTexture& normals, ofTexture& specular, ofTexture& displacement)
{
	ofEnableDepthTest();

	camera.begin();
	ofPushMatrix();
	shader.begin();

	shader.setUniformTexture("diffuseTexture", albido, 1);
	shader.setUniformTexture("normalTexture", normals, 2);
	shader.setUniformTexture("specularTexture", specular, 3);
	shader.setUniformTexture("displacementTexture", displacement, 4);
	shader.setUniform1f("displacementScale", displacementScale.getValue());
	
	shader.setUniform4f("sceneLightPosition", pointLight.getLightPosition());
	shader.setUniform3f("sceneLightDiffuseColor", pointLight.getLightDiffuseColor());
	shader.setUniform3f("sceneLightSpecularColor", pointLight.getLightSpecularColor());
	shader.setUniform3f("sceneLightAttenuation", pointLight.getAttenuationConst());
	shader.setUniform1f("sceneLightIntencity", pointLight.getLightIntencity());


	shader.setUniform3f("eyePositionWorld", camera.getGlobalPosition());
	shader.setUniformMatrix4f("translationMatrix", transformation);

	mesh.draw();

	shader.end();

	ofPopMatrix();


	pointLight.draw(camera.getGlobalPosition());

	camera.end();

	ofDisableDepthTest();
}
//--------------------------------------------------------------
void PreviewScene::lightMenu(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	lightControls.setup(style, font, fontColor, "Light Controls", width, height);
	
	lightControls.addGuiFloat(lightColorRed, "Red", 256, 24, 0.0, 1.0, 0.01);
	lightColorRed.setSliderStyle(ofVec4f(1.0, 0.0, 0.0, 1.0), ofVec4f(0.0, 1.0, 0, 0), SLOPED);
	lightColorRed.setValue(1.0);
	lightControls.addGuiFloat(lightColorGreen, "Green", 256, 24, 0.0, 1.0, 0.01);
	lightColorGreen.setSliderStyle(ofVec4f(0.0, 1.0, 0.0, 1.0), ofVec4f(0.0, 1.0, 0, 0), SLOPED);
	lightColorGreen.setValue(1.0);
	lightControls.addGuiFloat(lightColorBlue, "Blue", 256, 24, 0.0, 1.0, 0.01);
	lightColorBlue.setSliderStyle(ofVec4f(0.0, 0.0, 1.0, 1.0), ofVec4f(0.0, 1.0, 0, 0), SLOPED);
	lightColorBlue.setValue(1.0);



	lightControls.addGuiFloat(lightAttenuationConst, "Constant Atten", 256, 24, 0.0, 2.0, 0.01);
	lightControls.addGuiFloat(lightAttenuationLinear, "Linear Atten", 256, 24, 0.0, 2.0, 0.01);
	lightAttenuationLinear.setValue(1.0);
	lightControls.addGuiFloat(lightAttenuationQuadratic, "Quadratic Atten", 256, 24, 0.0, 2.0, 0.01);

	lightControls.addGuiFloat(lightColorIntencity, "Intencity", 256, 24, 0.0, 10.0, 0.01);
	lightColorIntencity.setValue(1.0);

	lightControls.setElementPosition(0, 0, 0);
	lightControls.setElementPosition(0, height + 1, 1);
	lightControls.setElementPosition(0, height * 2 + 1 * 2, 2);
	lightControls.setElementPosition(0, height * 3 + 1 * 3, 3);
	lightControls.setElementPosition(0, height * 4 + 1 * 4, 4);
	lightControls.setElementPosition(0, height * 5 + 1 * 5, 5);
	lightControls.setElementPosition(0, height * 6 + 1 * 6, 6);

	lightControls.setEventEnabled(true);
}
//--------------------------------------------------------------
void PreviewScene::meshMenu(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	meshControls.setup(style, font, fontColor, "Model Controls", width, height);
	
	meshControls.addGuiButton(boxMesh, "Box", 256, 24);
	meshControls.addGuiButton(planeMesh, "Plane", 256, 24);
	meshControls.addGuiButton(sphereMesh, "Sphere", 256, 24);
	meshControls.addGuiButton(cylinderMesh, "Cylinder", 256, 24);
	meshControls.addGuiButton(displacementSelection, "Displace", 256, 24);
	boxMesh.setState(true);
	
	meshControls.setElementPosition(0, 0, 0);
	meshControls.setElementPosition(0, height + 1, 1);
	meshControls.setElementPosition(0, height * 2 + 1 * 2, 2);
	meshControls.setElementPosition(0, height * 3 + 1 * 3, 3);
	meshControls.setElementPosition(0, height * 4 + 1 * 4, 4);
	

	meshControls.setEventEnabled(true);

	meshState = 0;
}
//--------------------------------------------------------------
void PreviewScene::reloadMesh()
{
	int resolution = textureWidth / 2.0;
	switch (meshState)
	{
	case 0:
		if (doDisplacement) {
			mesh.createMesh(ofMesh::box(500, 500, 500, resolution, resolution, resolution));
		}
		else {
			mesh.createMesh(ofMesh::box(500, 500, 500));
		}


		//mesh.createMesh(ofMesh::box(500, 500, 500));
		mesh.loadAttributes(shader);
		meshPositionZ = -750;
		break;
	case 1:
		if (doDisplacement) {
			mesh.createMesh(ofMesh::plane(500, 500, resolution, resolution, OF_PRIMITIVE_TRIANGLES));
		}
		else {
			mesh.createMesh(ofMesh::plane(500, 500, 2, 2, OF_PRIMITIVE_TRIANGLES));
		}


		//mesh.createMesh(ofMesh::plane(500, 500, 2, 2, OF_PRIMITIVE_TRIANGLES));
		mesh.loadAttributes(shader);
		meshPositionZ = -500;
		break;
	case 2:
		if (doDisplacement) {
			mesh.createMesh(ofMesh::sphere(250, resolution, OF_PRIMITIVE_TRIANGLES), false);
		}
		else {
			mesh.createMesh(ofMesh::sphere(250, 36, OF_PRIMITIVE_TRIANGLES), false);
		}


		//mesh.createMesh(ofMesh::sphere(250, 36, OF_PRIMITIVE_TRIANGLES),false);
		mesh.loadAttributes(shader);
		meshPositionZ = -750;
		break;
	case 3:
		if (doDisplacement) {
			mesh.createMesh(ofMesh::cylinder(250,500, resolution, resolution,2,false,OF_PRIMITIVE_TRIANGLES) );
		}
		else {
			mesh.createMesh(ofMesh::cylinder(250,500,36,6,2,false,OF_PRIMITIVE_TRIANGLES) );
		}


		//mesh.createMesh(ofMesh::cylinder(250,500,36,6,2,true,OF_PRIMITIVE_TRIANGLES) );
		mesh.loadAttributes(shader);
		meshPositionZ = -750;
		break;
	default:
		break;
	}
}
//--------------------------------------------------------------
// Load shader
void PreviewScene::reloadShader()
{
	if (doDisplacement) {
		shader.load("res/shaders/displacementLighting.vert", "res/shaders/displacementLighting.frag");
	}
	else {
		shader.load("res/shaders/normalmapLighting.vert", "res/shaders/normalmapLighting.frag");
	}
}

void PreviewScene::enable()
{
	lightControls.setEventEnabled(true);
	meshControls.setEventEnabled(true);
}
void PreviewScene::disable()
{
	lightControls.setEventEnabled(false);
	meshControls.setEventEnabled(false);
}
