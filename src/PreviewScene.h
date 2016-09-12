#pragma once

#include "ofMain.h"

#include "GraphicMesh.h"
#include "Gui.h"
#include "LightProp.h"

enum ModelType 
{
	BOX_MESH = 0,
	PLANE_MESH,
	SPHERE_MESH,
	CYLINDER_MESH
};

class PreviewScene
{
public:
	PreviewScene();
	~PreviewScene();

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height);
	void update();
	void draw(ofTexture& albido, ofTexture& normals, ofTexture& specular, ofTexture& heights);

	void drawControls(int x, int y);
	void drawLightControls(int x, int y);
	void drawMeshControls(int x, int y);

	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);

	void enable();
	void disable();

	void set3DModel();

	void reloadLightShader() { pointLight.reloadShader(); }

protected:

	void			drawWithNormalmap(ofTexture& albido, ofTexture& normals, ofTexture& specular);
	void			drawWithDisplacement(ofTexture& albido, ofTexture& normals, ofTexture& specular, ofTexture& displacement);


	void			lightMenu(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height);
	void			meshMenu(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height);

	void			reloadMesh();
	void			reloadShader();
		
	bool			doDisplacement;
	// Light
	LightProp		pointLight;
	ofVec2f			prevMousePos;
	float			lightDistanceFromObject;

	Gui				lightControls;

	GuiFloat		lightColorRed;
	GuiFloat		lightColorGreen;
	GuiFloat		lightColorBlue;

	GuiFloat		lightAttenuationConst;
	GuiFloat		lightAttenuationLinear;
	GuiFloat		lightAttenuationQuadratic;

	GuiFloat		lightColorIntencity;

	// Model
	GraphicMesh		mesh;
	ofMatrix4x4		transformation;
	float			rotationY;
	float			meshPositionZ;

	Gui				meshControls;

	GuiButton		boxMesh;
	GuiButton		planeMesh;
	GuiButton		sphereMesh;
	GuiButton		cylinderMesh;
	GuiButton		displacementSelection;
	GuiFloat		displacementScale;

	int				meshState;
	// Shader
	ofShader		shader;

	// Camera
	ofCamera		camera;

	int textureWidth;
	int textureHeight;
	
};

