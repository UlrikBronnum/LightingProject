#include "ImageEditor.h"



ImageEditor::ImageEditor(){}


ImageEditor::~ImageEditor()
{
	for (int i = 0; i < layer.size(); i++)
	{
		delete layer[i];
	}
	layer.clear();
}

void ImageEditor::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	this->style = style;
	this->font = font;
	this->fontColor = fontColor;
	this->width = width;
	this->height = height;

	layerSelection.setup(this->style, this->font, this->fontColor, "", this->width - this->height - 1, 1);
	layerSelection.setEventEnabled(true);

	layerControl.setup(this->style, this->font, this->fontColor, "", this->height, 1);
	layerControl.setEventEnabled(true);

	imageButton.setup(this->style, this->font, this->fontColor, "Source Image", this->width, this->height);
	imageButton.setEventEnabled(true);

	

	layerControlMenu.setup(this->style, this->font, this->fontColor, "", this->height, 1);
	layerControlMenu.addGuiButton("Delete", this->width, this->height);
	layerControlMenu.addGuiButton("Albido", this->width, this->height);
	layerControlMenu.addGuiButton("Normalmap", this->width, this->height);
	layerControlMenu.addGuiButton("Specularmap", this->width, this->height);
	layerControlMenu.addGuiButton("Heightmap", this->width, this->height);
	layerControlMenu.setEventEnabled(false);

	filterSelection.setup(this->style, this->font, this->fontColor, "", this->width, 1);
	filterSelection.addGuiButton("Invert", this->width, this->height);
	filterSelection.addGuiButton("Brightness/Contrast", this->width, this->height);
	filterSelection.addGuiButton("Gamma", this->width, this->height);
	filterSelection.addGuiButton("HSL Correction", this->width, this->height);
	filterSelection.addGuiButton("Blur", this->width, this->height);
	filterSelection.addGuiButton("Max/Min", this->width, this->height);
	filterSelection.addGuiButton("Edge Detection", this->width, this->height);
	filterSelection.addGuiButton("Normalmap", this->width, this->height);
	filterSelection.addGuiButton("Threshold", this->width, this->height);
	filterSelection.addGuiButton("Morphology", this->width, this->height);
	filterSelection.addGuiButton("Highpass", this->width, this->height);
	filterSelection.addGuiButton("Blend", this->width, this->height);
	filterSelection.setEventEnabled(true);

	activeLayer = -1;
	activeModLayer = -1;
}

void ImageEditor::update()
{
	if (activeModLayer != -1)
	{
		if (layerControlMenu.getElementState(0))
		{
			deleteLayer();
			layerControlMenu.setElementState(0, false);
			activeModLayer = -1;
		}
		else
		if (layerControlMenu.getElementState(1))
		{
			tAlbido = layer[activeModLayer]->getImage()->getTexture();
			layerControlMenu.setElementState(1, false);
			layerControl.setElementState(activeModLayer, false);

			for (int i = 0; i < layerControl.getNumElements(); i++)
			{
				string label = layerControl.getElementLabel(i);
				if (label == "a")
				{
					layerControl.setElementLabel(i, "");
				}
			}
			layerControl.setElementLabel(activeModLayer, "a");

			activeModLayer = -1;
		}
		else
		if (layerControlMenu.getElementState(2))
		{
			tNormal = layer[activeModLayer]->getImage()->getTexture();
			layerControlMenu.setElementState(2, false);
			layerControl.setElementState(activeModLayer, false);

			for (int i = 0; i < layerControl.getNumElements(); i++)
			{
				string label = layerControl.getElementLabel(i);
				if (label == "n")
				{
					layerControl.setElementLabel(i, "");
				}
			}
			layerControl.setElementLabel(activeModLayer, "n");

			activeModLayer = -1;
		}
		else
		if (layerControlMenu.getElementState(3))
		{
			tSpecular = layer[activeModLayer]->getImage()->getTexture();
			layerControlMenu.setElementState(3, false);
			layerControl.setElementState(activeModLayer, false);

			for (int i = 0; i < layerControl.getNumElements(); i++)
			{
				string label = layerControl.getElementLabel(i);
				if (label == "s")
				{
					layerControl.setElementLabel(i, "");
				}
			}
			layerControl.setElementLabel(activeModLayer, "s");

			activeModLayer = -1;
		}
		else
		if (layerControlMenu.getElementState(4))
		{
			tHeight = layer[activeModLayer]->getImage()->getTexture();
			layerControlMenu.setElementState(4, false);
			layerControl.setElementState(activeModLayer, false);

			for (int i = 0; i < layerControl.getNumElements(); i++) 
			{
				string label = layerControl.getElementLabel(i);
				if (label == "h")
				{
					layerControl.setElementLabel(i,"");
				}
			}
			layerControl.setElementLabel(activeModLayer, "h");

			activeModLayer = -1;
		}
	}
	addLayer();

	if(activeLayer != -1)
		layer[activeLayer]->apply();

	for (int i = activeLayer-1; i >= 0; i--) 
	{
		layer[i]->doUpdate();
	}

	setActiveLayer();
	setActiveModLayer();
}

void ImageEditor::draw(bool drawTexture)
{
	if(drawTexture){
		if (activeLayer == -1)
		{
			base.draw(ofGetWindowWidth() / 2 - 512, ofGetWindowHeight() / 2 - 512,1024,1024);
		}
		else
		{
			layer[activeLayer]->drawPixels(ofGetWindowWidth()/2-512, ofGetWindowHeight()/2-512, 1024, 1024);
			layer[activeLayer]->drawMenu(0, 24);
			ofVec2f pos = layerControl.getElementPosition(activeLayer);
			layer[activeLayer]->drawSelector(pos.x - layerControl.getSize().x, pos.y + height-1);
		}
	}
	if (activeLayer != -1)
	{
		layer[activeLayer]->drawMenu(0, 24);
	}

	filterSelection.draw(ofGetWindowWidth() - this->width, 24);

	layerControl.draw(	ofGetWindowWidth() - this->width,
						ofGetWindowHeight() - imageButton.getHeight() - layerControl.getSize().y);

	layerSelection.draw(ofGetWindowWidth() - this->width + layerControl.getSize().x, 
						ofGetWindowHeight() - imageButton.getHeight() - layerSelection.getSize().y);

	imageButton.draw(	ofGetWindowWidth() - this->width, ofGetWindowHeight() - imageButton.getHeight());

	if (activeModLayer > -1) {
		ofVec2f pos = layerControl.getElementPosition(activeModLayer);
		if (pos.y + layerControlMenu.getSize().y > ofGetWindowHeight())
		{
			layerControlMenu.draw(pos.x - layerControlMenu.getSize().x, ofGetWindowHeight()-layerControlMenu.getSize().y);
		}
		else
		{
			layerControlMenu.draw(pos.x - layerControlMenu.getSize().x, pos.y - 1);
		}
	}
}

void ImageEditor::addLayer()
{
	int newFilter = -1;
	for (int i = 0; i < filterSelection.getNumElements(); i++)
	{
		if (filterSelection.getElementState(i))
		{
			newFilter = i;
			filterSelection.setElementState(i, false);
		}
	}
	if (newFilter != -1)
		createLayer(newFilter);
}
void ImageEditor::deleteLayer()
{
	layerSelection.eraseElement(activeModLayer);
	layerControl.eraseElement(activeModLayer);
	layer[activeModLayer]->disable();
	delete layer[activeModLayer];
	layer.erase(layer.begin() + activeModLayer);

	activeModLayer = -1;


	if (activeLayer > layer.size() - 1) {
		activeLayer = layer.size() - 1;
	}
	else if (layer.empty()) {
		activeLayer = -1;
	}
	
	setActiveLayer();

}

void ImageEditor::createLayer(int filterToCreate)
{
	FilterModule *newFilter = nullptr;

	if (filterToCreate == 0)
	{
		newFilter = new Invert();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 1)
	{
		newFilter = new BrightnessContrast();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 2)
	{
		newFilter = new Gamma();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 3)
	{
		newFilter = new HSLCorrection();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 4)
	{
		newFilter = new Blur();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 5)
	{
		newFilter = new MinMax();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 6)
	{
		newFilter = new Edge();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 7)
	{
		newFilter = new Normalmap();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 8)
	{
		newFilter = new Threshold();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 9)
	{
		newFilter = new Morph();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 10)
	{
		newFilter = new Highpass();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
	}
	else if (filterToCreate == 11)
	{
		newFilter = new Blends();
		newFilter->setup(this->style, this->font, this->fontColor, this->width, this->height);
		newFilter->setReferencesPossibilities(layer);
		activeLayer = 0;
	}
	if(newFilter != nullptr)
		addToVector(*newFilter, filterSelection.getElementLabel(filterToCreate));

	
	setActiveLayer();
}
void ImageEditor::addToVector(FilterModule& filter, string label) 
{
	// set current layer to false to change to new layer
	layerSelection.setElementState(activeLayer, false);
	// add buttons to menu inverted as 
	TextAlignment tA;
	tA.h = LEFT;
	tA.v = vCENTER;

	TextAlignment	alignment;
	alignment.h = hCENTER;
	alignment.v = vCENTER;

	int posS = layerSelection.addGuiButton(label, this->width - this->height - 1, this->height, tA, true, activeLayer);
	int posC = layerControl.addGuiButton("", this->height, this->height, alignment, false, activeLayer);

	if (activeLayer > -1 && activeLayer < layer.size())
		layer[activeLayer]->disable();

	if (posC < 0 || layer.empty() || posC > layer.size())
	{
		filter.setReference(base);
		layer.push_back(&filter);
	}
	else
	{
		filter.setReference(*layer[activeLayer]->getImage());
		layer.insert(layer.begin() + posC, &filter);
	}
}
void ImageEditor::setActiveLayer()
{
	if (imageButton.getState() && activeLayer != -1)
	{
		layer[activeLayer]->disable();

		activeLayer = -1;

		for (int i = 0; i < layerSelection.getNumElements(); i++)
		{
			layerSelection.setElementState(i, false);
		}
	}
	else
	{
		int state = -1;
		for (int i = 0; i < layerSelection.getNumElements(); i++)
		{
			if ( layerSelection.getElementState(i) && i != activeLayer)
			{
				state = i;
			}
		}	
		if (state != -1) 
		{
			layerSelection.setElementState(activeLayer, false);

			if (activeLayer > -1)
				layer[activeLayer]->disable();

			activeLayer = state;
			layerSelection.setElementState(activeLayer, true);

			if (activeLayer > -1)
				layer[activeLayer]->enable();

			imageButton.setState(false);
		}
		else {
			layerSelection.setElementState(activeLayer, true);

		}
	}
}
void ImageEditor::setActiveModLayer()
{
	int state = -1;
	for (int i = 0; i < layerControl.getNumElements(); i++)
	{
		if (layerControl.getElementState(i))
		{
			state = i;
		}
	}
	if (state != -1)
	{
		activeModLayer = state;
		layerControl.setElementState(activeModLayer,true);
		layerControlMenu.setEventEnabled(true);
	}
	else
	{
		layerControl.setElementState(activeModLayer,false);
		activeModLayer = -1;
		layerControlMenu.setEventEnabled(false);
	}
}