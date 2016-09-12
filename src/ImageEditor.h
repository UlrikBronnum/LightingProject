#pragma once

#include "ofMain.h"

#include "Gui.h"
#include "FilterModule.h"

class ImageEditor
{
public:
	ImageEditor();
	~ImageEditor();
	
	virtual void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height);
	virtual void update();
	virtual void draw(bool drawTexture);

	
	void setBaseImage(ofImage& image) { base = image;  }

	ofTexture* getAlbido()
	{
		if (!tAlbido.isAllocated()) {
			tAlbido = base.getTexture();
		}
		return &tAlbido;
	}
	ofTexture* getNormal() { return &tNormal; }
	ofTexture* getSpecular() { return &tSpecular; }
	ofTexture* getHeight() { return &tHeight; }


protected:

	void addLayer();
	void createLayer(int filterToCreate);
	void deleteLayer();
	void setActiveLayer();
	void addToVector(FilterModule& filter, string label);
	void setActiveModLayer();

	vector<FilterModule*> layer;

	ofTexture	tAlbido;
	ofTexture	tNormal;
	ofTexture	tSpecular;
	ofTexture	tHeight;

	ofImage		base;

	Gui			filterSelection;
	Gui			layerSelection;
	int			activeLayer;
	Gui			layerControl;
	int			activeModLayer;
	Gui			layerControlMenu;

	Gui			imageButton;

	GuiStyle*	style;
	GuiFont		font;
	ofVec4f		fontColor; 

	int			width; 
	int			height;
};

