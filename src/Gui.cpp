#include "Gui.h"



Gui::Gui()
{
	elementPadding = ofVec3f(0,0);
}


Gui::~Gui()
{
	
}
void Gui::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height)
{
	this->style = style;
	this->width = width;
	this->height = height;

	elementStyle = style->getStyleType();

	this->font = font;
	this->fontColor = fontColor;
	this->label = label;

	if(width != 0 && height != 0)
		fbo.allocate(width, height);

	updateFbo = true;
	
	setDoPositionInDraw(false);
}
void Gui::draw(int x, int y)
{
	setPosition(x, y);
	
	bool someElementIsActive = false;
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->getState()) {
			someElementIsActive = true;
		}
	}
	
	updateFbo = (updateFbo == false) ? isPointInsideElement(ofGetMouseX(), ofGetMouseY(), fbo.getWidth(), fbo.getHeight()) : updateFbo;

	if (updateFbo || someElementIsActive)
	{
		updateGui(x, y);
	}
	
	ofDisableAlphaBlending();
		ofSetColor(255);
		fbo.draw(x,y);
	ofEnableAlphaBlending();
	
}

bool Gui::mouseMoved(ofMouseEventArgs & args)
{
	bool result = GuiBaseElement::mouseMoved(args);

	if(isPointInsideElement(args.x, args.y, fbo.getWidth(), fbo.getHeight() ) )
		for (int i = 0; i < elements.size(); i++)
		{
			elements[i]->mouseMoved(args);
		}
	return result;
}
bool Gui::mousePressed(ofMouseEventArgs & args)
{
	bool result = GuiBaseElement::mousePressed(args);

	if (isPointInsideElement(args.x, args.y, fbo.getWidth(), fbo.getHeight()))
		for (int i = 0; i < elements.size(); i++)
		{
			if (elements[i]->mousePressed(args)) {
			}
		}
	return result;
}
bool Gui::mouseDragged(ofMouseEventArgs & args)
{
	bool result = GuiBaseElement::mouseDragged(args);
	if (isPointInsideElement(args.x, args.y, fbo.getWidth(), fbo.getHeight()))
		for (int i = 0; i < elements.size(); i++)
		{
			elements[i]->mouseDragged(args);
		}
	return result;
}
bool Gui::mouseReleased(ofMouseEventArgs & args)
{
	bool result = GuiBaseElement::mouseReleased(args);

	if (isPointInsideElement(args.x, args.y, fbo.getWidth(), fbo.getHeight()))
		for (int i = 0; i < elements.size(); i++)
		{
			elements[i]->mouseReleased(args);
		}

	return result;
}
bool Gui::mouseExited(ofMouseEventArgs & args)
{
	bool result = GuiBaseElement::mouseExited(args);

	if (isPointInsideElement(args.x, args.y, fbo.getWidth(), fbo.getHeight()))
		for (int i = 0; i < elements.size(); i++)
		{
			elements[i]->mouseExited(args);
		}
	return result;
}

// Key interactions
bool Gui::keyPressed(ofKeyEventArgs & args)
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->keyPressed(args);
	}
	return false;
}
bool Gui::keyReleased(ofKeyEventArgs & args)
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->keyReleased(args);
	}
	return false;
}


void Gui::updateGui(int x, int y)
{
	fbo.begin();
	ofClear(0, 0, 0, 0);

	ofSetColor(0);
	ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());

	GuiBaseElement::draw(0, 0);

	bool someElementIsActive = false;
	int elementY = height + elementPadding.y;

	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->draw(elementPositions[i].x, elementPositions[i].y);
		elements[i]->setPosition(position.x + elementPositions[i].x, position.y + elementPositions[i].y);
	}
	fbo.end();
}

void Gui::setElementPosition(int elementPositionX, int elementPositionY, int index) 
{
	setElementPosition(ofVec2f(elementPositionX, elementPositionY), index);
}
void Gui::setElementPosition(ofVec2f elementPosition, int index) 
{
	if (index > -1 && index < elements.size()) 
	{
		if (elementPositions.size() != elements.size()) 
		{
			int guiWidth = 1;
			int guiHeight = 1;

			elementPosition = ofVec2f(elementPosition.x, elementPosition.y + elementPadding.y);
			elementPositions[index] = elementPosition;

			if (index < elements.size())
			{
				elements[index]->setPosition(position.x + elementPosition.x, position.y + elementPosition.y);

				if (elementPositions[index].x + elements[index]->getWidth() > guiWidth)
				{
					width = elementPositions[index].x + elements[index]->getWidth();
					guiWidth = width;
				}
				if (elementPositions[index].y + elements[index]->getHeight() > guiHeight)
				{
					guiHeight = elementPositions[index].y + elements[index]->getHeight() + elementPadding.y;
				}
			}
			fbo.allocate(guiWidth, guiHeight);
		}
		else
		{
			int guiWidth = 1;
			int guiHeight = 1;
			
			elementPositions[index] = ofVec2f(elementPosition.x, elementPosition.y + height + elementPadding.y);
			elements[index]->setPosition(position.x + elementPositions[index].x, position.y + elementPositions[index].y);

			for (int i = 0; i < elements.size(); i++) 
			{
				if (elementPositions[i].x + elements[i]->getWidth() > guiWidth)
				{
					width = elementPositions[i].x + elements[i]->getWidth();
					guiWidth = width;
				}
				if (elementPositions[i].y + elements[i]->getHeight() > guiHeight)
				{
					guiHeight = elementPositions[i].y + elements[i]->getHeight() + elementPadding.y;
				}
			}

			fbo.allocate(guiWidth, guiHeight);
		}
	}
	updateFbo = true;
}
void Gui::setElementPosition(int index)
{
	if (index > 0 && index < elements.size())
	{
		for (int i = index; i < elements.size(); i++)
		{
			int posX = elementPositions[i - 1].x;
			int posY = elementPositions[i - 1].y;
			int w = elements[i - 1]->getWidth();
			int h = elements[i - 1]->getHeight();

			if (posX + w + elementPadding.x + elements[i]->getWidth() <= fbo.getWidth())
			{
				elementPositions[i].x = posX + w + elementPadding.x;
				elementPositions[i].y = posY;
			}
			else
			{
				elementPositions[i].x = 0;
				elementPositions[i].y = posY + elementPadding.y + h;
			}

			elements[i]->setPosition(elementPositions[i].x, elementPositions[i].y);
		}
	}else
	if (index == 0 )
	{
		elementPositions[0] = ofVec2f(0, height + elementPadding.y);
		for (int i = 1; i < elements.size(); i++)
		{
			int posX = elementPositions[i - 1].x;
			int posY = elementPositions[i - 1].y;
			int w = elements[i - 1]->getWidth();
			int h = elements[i - 1]->getHeight();

			if (posX + w + elementPadding.x + elements[i]->getWidth() <= fbo.getWidth())
			{
				elementPositions[i].x = posX + w + elementPadding.x;
				elementPositions[i].y = posY;
			}
			else
			{
				elementPositions[i].x = 0;
				elementPositions[i].y = posY + elementPadding.y + h;
			}
		}
	}

	int guiWidth = 1;
	int guiHeight = 1;

	for (int i = 0; i < elements.size(); i++)
	{
		if (elementPositions[i].x + elements[i]->getWidth() > guiWidth)
		{
			width = elementPositions[i].x + elements[i]->getWidth();
			guiWidth = width;
		}
		if (elementPositions[i].y + elements[i]->getHeight() > guiHeight)
		{
			guiHeight = elementPositions[i].y + elements[i]->getHeight() + elementPadding.y;
		}
	}

	fbo.allocate(guiWidth, guiHeight);
	 
	updateFbo = true;
}
void Gui::updateGuiPositions()
{
	int guiWidth = 1;
	int guiHeight = 1;
	for (int i = 0; i < elements.size(); i++)
	{
		if (elementPositions[i].x + elements[i]->getWidth() > guiWidth)
		{
			width = elementPositions[i].x + elements[i]->getWidth();
			guiWidth = width;
		}
		if (elementPositions[i].y + elements[i]->getHeight() > guiHeight)
		{
			guiHeight = elementPositions[i].y + elements[i]->getHeight() + elementPadding.y;
		}
	}

	fbo.allocate(guiWidth, guiHeight);
}
void Gui::addElement(GuiBaseElement& element)
{
	element.setDoPositionInDraw(false);
	elements.push_back(&element);
	elementPositions.push_back(ofVec2f(0, height + elementPadding.y));
	setElementPosition(elements.size()-1);
}
void Gui::addGuiButton(GuiButton& element, string label, int width, int height, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
int Gui::addGuiButton(string label, int width, int height, TextAlignment alignment, bool state, int index)
{
	
	GuiButton* element = new GuiButton();
	element->setup(style, font, fontColor, label, width, height);
	element->setColorStates(colorStates);
	element->setDoPositionInDraw(false);
	element->disableEvents();
	element->setState(state);
	element->setTextAlignment(alignment.h, alignment.v);
	
	if (index < elements.size() && index != -500 ) 
	{
		int theIndex = (index <= 0) ? 0 : index;
		elements.insert(elements.begin() + theIndex, element);
		elementIsPtr.insert(elementIsPtr.begin() + theIndex, true);
		elementPositions.insert(elementPositions.begin() + theIndex, ofVec2f(0, height + elementPadding.y));
	
		for(int i = 0; i < elementPositions.size(); i++)
			setElementPosition(i);

		return theIndex;
	}
	else {
		elements.push_back(element);
		elementIsPtr.push_back(true);
		elementPositions.push_back(ofVec2f(0, height + elementPadding.y));

		for (int i = 0; i < elementPositions.size(); i++)
			setElementPosition(i);

		return elements.size();
	}
}

void Gui::addGuiFloat(	GuiFloat& element, string label, int width, int height,
						float min, float max, float interval, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	element.setRange(min, max, interval);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
void Gui::addGuiFloat(	GuiFloat& element, string label, int width, int height, 
						ofVec4f color, ofVec4f gradient, StyleTypes sliderStyle,
						float min, float max, float interval, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	element.setSliderStyle(color, gradient, sliderStyle);
	element.setRange(min, max, interval);
	addElement(element);
	elementIsPtr.push_back(localPtr);

}
void Gui::addGuiFloat(	GuiFloat& element, string label, int width, int height, 
						ofVec4f color, ofVec4f gradient, ofTexture& texture, StyleTypes sliderStyle,
						float min, float max, float interval, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	element.setSliderStyle(color, gradient, texture, sliderStyle);
	element.setRange(min, max, interval);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
void Gui::addGuiInt(GuiInt& element, string label, int width, int height,
					int min, int max, int interval, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	element.setRange(min, max, interval);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
void Gui::addGuiInt(GuiInt& element, string label, int width, int height, 
					ofVec4f color, ofVec4f gradient, StyleTypes sliderStyle,
					int min, int max, int interval, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	element.setSliderStyle(color, gradient, sliderStyle);
	element.setRange(min, max, interval);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
void Gui::addGuiInt(GuiInt& element, string label, int width, int height,
					ofVec4f color, ofVec4f gradient, ofTexture& texture, StyleTypes sliderStyle,
					int min, int max, int interval, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	element.setSliderStyle(color, gradient, texture, sliderStyle);
	element.setRange(min, max, interval);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
void Gui::addGuiInput(GuiInput& element, string label, int width, int height, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
void Gui::addGuiArrayCounter(GuiArrayCounter& element, string label, int width, int height, 
							vector<int> value, int startIndex, bool localPtr)
{
	element.setup(style, font, fontColor, label, width, height);
	element.setColorStates(colorStates);
	element.setValue(value, startIndex);
	addElement(element);
	elementIsPtr.push_back(localPtr);
}
bool Gui::isPointInsideElement(int x, int y, int w, int h)
{
	float limitX = position.x + w;
	float limitY = position.y + h;

	if ((x > position.x && x < limitX) &&
		(y > position.y && y < limitY))
	{
		return true;
	}
	return false;
}
void Gui::setEventEnabled(bool isEnabled)
{
	if (isEnabled) 
	{
		enableEvents();
	}
	else
	{
		disableEvents();
	}
}
void Gui::setElementState(int index, bool value)
{
	if (index >= 0 && index < elements.size())
	{
		elements[index]->setState(value);
	}
}
bool Gui::getElementState(int index)
{
	if (index >= 0 && index < elements.size())
	{
		return elements[index]->getState();
	}
	return false;
}
string Gui::getElementLabel(int index)
{
	if (index >= 0 && index < elements.size())
	{
		return elements[index]->getLabel();
	}
	return "";
}
void Gui::setElementLabel(int index, string label)
{
	if (index >= 0 && index < elements.size())
	{
		elements[index]->setLabel(label);
	}
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiStyle::GuiStyle()
{
	shader.load("res/shaders/gui/solidFill.vert", "res/shaders/gui/solidFill.frag");
	color = ofVec4f(1.0);
	customTexcoords = false;
}


GuiStyle::~GuiStyle()
{
}

void GuiStyle::setup(ofVec4f color, ofVec4f gradient, StyleTypes style)
{
	this->color = color;
	this->gradient = gradient;
	this->style = style;

	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	mesh.addVertex(ofVec3f(0, 0, 0));
	mesh.addVertex(ofVec3f(0, 1, 0));
	mesh.addVertex(ofVec3f(1, 0, 0));
	mesh.addVertex(ofVec3f(1, 1, 0));

	mesh.addTexCoord(ofVec2f(0, 0));
	mesh.addTexCoord(ofVec2f(0, 1));
	mesh.addTexCoord(ofVec2f(1, 0));
	mesh.addTexCoord(ofVec2f(1, 1));
}
void GuiStyle::draw(int x, int y, int w, int h, float colorModifier)
{
	draw(x, y, w, h, style, colorModifier);
}
void GuiStyle::draw(int x, int y, int w, int h, StyleTypes customStyle, float colorModifier)
{
	setPosition(x, y, w, h);

	shader.begin();
	
	shader.setUniform1f("tintModifier", colorModifier);
	shader.setUniform4f("gradientLimits", gradient);
	shader.setUniform1i("gradientType", customStyle);
	shader.setUniform4f("tintColor", color);
	 
		mesh.draw();
	shader.end();
}

void GuiStyle::setPosition(int x, int y, int w, int h)
{
	mesh.getVertices()[0] = ofVec3f(x, y, 0);
	mesh.getVertices()[1] = ofVec3f(x, y+h, 0);
	mesh.getVertices()[2] = ofVec3f(x+w, y, 0);
	mesh.getVertices()[3] = ofVec3f(x+w, y+h, 0);
}
void GuiStyle::setTexCoordX(float xTexcoordMax)
{
	mesh.getTexCoords()[2] = ofVec2f(xTexcoordMax, 0);
	mesh.getTexCoords()[3] = ofVec2f(xTexcoordMax, mesh.getTexCoords()[3].y);

	customTexcoords = true;
}
void GuiStyle::setTexCoordY(float yTexcoordMax)
{
	mesh.getTexCoords()[1] = ofVec2f(0, yTexcoordMax);
	mesh.getTexCoords()[3] = ofVec2f(mesh.getTexCoords()[3].x, yTexcoordMax);

	customTexcoords = true;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiStyleTexture::GuiStyleTexture()
{
	shader.load("res/shaders/gui/textured.vert", "res/shaders/gui/textured.frag");
	color = ofVec4f(1.0);
}


GuiStyleTexture::~GuiStyleTexture()
{
}

void GuiStyleTexture::setup(ofVec4f color, ofVec4f gradient, ofTexture& texture, bool destortTexture, StyleTypes style)
{
	GuiStyle::setup(color, gradient,style);

	
	this->destortTexture = destortTexture;
	this->texture = texture;

	this->texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
	this->texture.generateMipmap();
	this->texture.setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
}
void GuiStyleTexture::draw(int x, int y, int w, int h, float colorModifier)
{
	draw(x, y, w, h, style, colorModifier);
}
void GuiStyleTexture::draw(int x, int y, int w, int h, StyleTypes customStyle, float colorModifier)
{
	setPosition(x, y, w, h);

	ofVec2f texCoordMax = getMaxTexCoords();
	if (!destortTexture)
		texCoordMax = setTexCoords(w, h);

	if (customTexcoords)
	{
		texCoordMax = ofVec2f(1.0, 1.0);
	}

	shader.begin();

	shader.setUniform1f("tintModifier", colorModifier);
	shader.setUniform2f("texCoordMax", texCoordMax);
	shader.setUniform4f("gradientLimits", gradient);
	shader.setUniform1i("gradientType", customStyle);

	shader.setUniform4f("tintColor", color);
	shader.setUniformTexture("texture", texture, 1);

	mesh.draw();

	shader.end();
}
ofVec2f GuiStyleTexture::setTexCoords(int w, int h)
{
	float texCoordW = (float)w / texture.getWidth();
	float texCoordH = (float)h / texture.getHeight();

	mesh.getTexCoords()[0] = ofVec2f(0, 0);
	mesh.getTexCoords()[1] = ofVec2f(0, texCoordH);
	mesh.getTexCoords()[2] = ofVec2f(texCoordW, 0);
	mesh.getTexCoords()[3] = ofVec2f(texCoordW, texCoordH);

	return ofVec2f(texCoordW, texCoordH);
}
void GuiStyleTexture::setTexCoords(ofVec2f texCoord1, ofVec2f texCoord2, ofVec2f texCoord3, ofVec2f texCoord4)
{
	mesh.getTexCoords()[0] = texCoord1;
	mesh.getTexCoords()[1] = texCoord2;
	mesh.getTexCoords()[2] = texCoord3;
	mesh.getTexCoords()[3] = texCoord4;
	destortTexture = false;
}
void GuiStyleTexture::setTexCoordX(float xTexcoordMax)
{
	mesh.getTexCoords()[2] = ofVec2f(xTexcoordMax, 0);
	mesh.getTexCoords()[3] = ofVec2f(xTexcoordMax, mesh.getTexCoords()[3].y);

	customTexcoords = true;
}
void GuiStyleTexture::setTexCoordY(float yTexcoordMax)
{
	mesh.getTexCoords()[1] = ofVec2f(0, yTexcoordMax);
	mesh.getTexCoords()[3] = ofVec2f(mesh.getTexCoords()[3].x, yTexcoordMax);

	customTexcoords = true;
}
ofVec2f GuiStyleTexture::getMaxTexCoords()
{
	return mesh.getTexCoords()[3];
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiFont::GuiFont()
{

}

GuiFont::~GuiFont()
{

}

// Setup font variables
void GuiFont::setup(string fontLocation, int fontSize)
{
	this->fontLocation = fontLocation;
	this->fontSize = fontSize;

	font.loadFont(fontLocation, fontSize, true, true);
}
void GuiFont::setup(ofTrueTypeFont&  nFont)
{
	font = nFont;
}

void GuiFont::draw(string text, int x, int y, ofColor color)
{
	if (font.isLoaded()) {
		ofSetColor(color);
		font.drawString(text, x, y);
	}
	else {
		ofSetColor(color);
		ofDrawBitmapString(text, x, y);
	}
}
void GuiFont::draw(string text, int x, int y, ofVec4f color)
{
	if (font.isLoaded()) {
		ofSetColor(255 * color.x, 255 * color.y, 255 * color.z, 255 * color.w);
		font.drawString(text, x, y);
	}
	else {
		ofSetColor(255 * color.x, 255 * color.y, 255 * color.z, 255 * color.w);
		ofDrawBitmapString(text, x, y);
	}
}
// Get width of a given string
float GuiFont::getTextWidth(string text)
{
	if (font.isLoaded()) {
		return font.stringWidth(text);
	}
	return getBoundingBox(text).width;
}

// Get height of a given string
float GuiFont::getTextHeight(string text)
{
	if (font.isLoaded()) {
		return font.stringHeight(text);
	}
	return getBoundingBox(text).height;
}

// Set line height
void GuiFont::setLineHeight(float h)
{
	if (font.isLoaded()) {
		font.setLineHeight(h);
	}
}
// Get line height
float GuiFont::getLineHeight()
{
	if (font.isLoaded()) {
		return font.getLineHeight();
	}
	return getBoundingBox("X").height;
}

// Set space width
void GuiFont::setSpaceWidth(float w)
{
	if (font.isLoaded()) {
		font.setSpaceSize(w);
	}
}
// Get letter spacing
float GuiFont::getSpaceWidth()
{
	if (font.isLoaded()) {
		return font.getLetterSpacing();
	}
	return getBoundingBox(" ").width;
}
// Adjust string length to fit field
string GuiFont::cutTextWidth(string inputStr, float width)
{
	string nStr = inputStr;
	int textWidth = getTextWidth(nStr);

	while (textWidth >= width - getTextWidth("X")) {
		nStr = nStr.substr(1);
		textWidth = getTextWidth(nStr);
	}
	return nStr;
}
// Adjust string length to fit field
int GuiFont::findTextWidth(string inputStr, float width)
{
	string nStr = inputStr;
	int textWidth = getTextWidth(nStr);

	while (textWidth > width) {
		nStr = nStr.substr(0, nStr.size() - 2);
		textWidth = getTextWidth(nStr);
	}
	return nStr.size();
}
// Adjust string width to fit field
string GuiFont::adjustTextWidth(string inputStr, float width)
{
	int length = findTextWidth(inputStr, width);
	length = (length == 0) ? 1 : length;
	int lines = inputStr.size() / (length);
	if(length > 1)
		if ((inputStr.size() - length) % length != 0) {
			length -= 1;
			lines = inputStr.size() / (length);
		}

	string output = inputStr;
	int index = 1;
	for (int i = 0; i < lines; i += length) {
		output.insert((i + index), "\n");
		index++;
	}
	return output;
}
// Adjust string width to fit field
string GuiFont::adjustTextWidth(string inputStr, float width, int& stringHeight)
{
	int length = findTextWidth(inputStr, width);
	int lines = inputStr.size() / (length);
	stringHeight = 0;
	if ((inputStr.size() - length) % length != 0) {
		length -= 1;
		lines = inputStr.size() / (length);
	}

	string output = inputStr;
	for (int i = 0; i < lines; i+=length) {
		output.insert((i + 1), "\n");
	}
	stringHeight = getLineHeight() * (lines + 1);
	return output;
}
ofRectangle GuiFont::getBoundingBox(string text)
{
	if ( font.isLoaded() ) {
		return font.getStringBoundingBox(text, 0, 0);
	}
	else {
		ofBitmapFont f;
		return f.getBoundingBox(text, 0, 0);
	}
}

int GuiFont::setAlignedX(AlignmentTypeHorizontal hori, int width, string text)
{
	int x = 0;

	switch (hori) {
	case LEFT:
		x = 0;
		break;
	case hCENTER:
		x = width / 2 - getTextWidth(text) / 2;
		break;
	case RIGHT:
		x = width - getTextWidth(text);
		break;
	default:
		break;
	}
	return x;
}
int GuiFont::setAlignedY(AlignmentTypeVertical vert, int height, string text)
{
	int y = 0;
	int h = getLineHeight() / 2;
	switch (vert) {
	case TOP:
		y = h + getLineHeight() / 3;
		break;
	case vCENTER:
		y = height / 2 + getBoundingBox("X").height / 10 * 4;
		break;
	case BUTTOM:
		y = height - h + getLineHeight() / 3;
		break;
	case ABOVE:
		y = -h;
		break;
	case BELOW:
		y = height + h + getLineHeight() / 3;
		break;
	default:
		break;
	}
	return y;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiElement::GuiElement()
{
	
}

GuiElement::~GuiElement()
{
}

void GuiElement::setup(GuiStyle* style, int width, int height)
{
	this->style = style;
	this->width = width;
	this->height = height;

	doPositionInDraw = true;
}
void GuiElement::draw(int x, int y)
{
	if(doPositionInDraw)
		setPosition(x, y);

	style->draw(x, y, width, height);
}

void GuiElement::draw()
{
	draw(position.x, position.y);
}
void GuiElement::setPosition(int x, int y) {
	position = ofVec2f(x, y);
}
bool GuiElement::isPointInsideElement(int x, int y) 
{
	float limitX = position.x + width;
	float limitY = position.y + height;
	bool inside = false;
	if ((x > position.x && x < limitX) &&
		(y > position.y && y < limitY) ) 
	{
		inside = true;
	}
	return inside;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

int GuiInteractiveElement::elementCount = 0;

GuiInteractiveElement::GuiInteractiveElement()
{
	isSwitch = true;
	mouseEnable = false;
	keyEnable = false;

	dragProcentage.x = 0.0f;
	dragProcentage.y = 0.0f;

	GuiInteractiveElement::elementCount++;
	state = false;
	//cout << "constructor " << GuiInteractiveElement::getElementCount() << endl;
}
GuiInteractiveElement::~GuiInteractiveElement()
{
	disableEvents();
	GuiInteractiveElement::elementCount--;
	//cout << "destructor " << GuiInteractiveElement::getElementCount() << endl;

	//if(elementCount == 0)
		//
}
void GuiInteractiveElement::draw(int x, int y)
{
	if (doPositionInDraw)
		setPosition(x, y);

	float modifier = 1.0f;

	if (state) {
		modifier = 0.6f;
	}
	else if (mouseInside) {
		modifier = 0.8f;
	}
	style->draw(x, y, width, height, modifier);
}

bool GuiInteractiveElement::mouseMoved(int x, int y)
{
	mouseInside = isPointInsideElement(x, y);
	return mouseInside;
}
bool GuiInteractiveElement::mousePressed(int x, int y, int button)
{
	if (isPointInsideElement(x, y)) 
	{
		if (isSwitch) {
			state = !state;
		}
		else {
			state = true;
		}
		return state;
	}
	return false;
}
bool GuiInteractiveElement::mouseDragged(int x, int y, int button)
{
	if (isPointInsideElement(x, y))
	{
		float xPosition = (x - ( (int)position.x + 2 )) / (float)(width - 4);
		float yPosition = 1.0f - (y - ( (int)position.y + 2 )) / (float)(height - 4);

		dragProcentage = ofVec2f(xPosition, yPosition);
		state = true;

		return true;
	}
	return false;
}
bool GuiInteractiveElement::mouseReleased(int x, int y, int button)
{
	if (state) {
		return true;
	}
	return false;
}
bool GuiInteractiveElement::mouseExited(int x, int y)
{
	mouseInside = false;
	return mouseInside;
}
bool GuiInteractiveElement::mouseMoved(ofMouseEventArgs & args)
{
	return mouseMoved(args.x, args.y);
}
bool GuiInteractiveElement::mousePressed(ofMouseEventArgs & args)
{
	return mousePressed(args.x, args.y, args.button);
}
bool GuiInteractiveElement::mouseDragged(ofMouseEventArgs & args)
{
	return mouseDragged(args.x, args.y, args.button);
}
bool GuiInteractiveElement::mouseReleased(ofMouseEventArgs & args)
{
	return mouseReleased(args.x, args.y, args.button);
}
bool GuiInteractiveElement::mouseExited(ofMouseEventArgs & args)
{
	return mouseExited(args.x, args.y);
}
bool GuiInteractiveElement::keyPressed(int key)
{ 
	return false; 
}
bool GuiInteractiveElement::keyReleased(int key)
{ 
	return false; 
}

bool GuiInteractiveElement::keyPressed(ofKeyEventArgs & args)
{ 
	keyPressed(args.key);
	return false; 
}
bool GuiInteractiveElement::keyReleased(ofKeyEventArgs & args)
{ 
	keyReleased(args.key);
	return false;
}

void GuiInteractiveElement::enableEvents()
{
	if (!mouseEnable)
		registerMouseEvent();

	if (!keyEnable)
		registerKeyEvent();
}
void GuiInteractiveElement::disableEvents()
{
	if (mouseEnable)
		unregisterMouseEvent();

	if (keyEnable)
		unregisterKeyEvent();
}
void GuiInteractiveElement::registerMouseEvent()
{
	ofAddListener(ofEvents().mouseMoved, this, &GuiInteractiveElement::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &GuiInteractiveElement::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &GuiInteractiveElement::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &GuiInteractiveElement::mouseReleased);
	ofAddListener(ofEvents().mouseExited, this, &GuiInteractiveElement::mouseExited);
	mouseEnable = true;
}
void GuiInteractiveElement::unregisterMouseEvent()
{
	ofRemoveListener(ofEvents().mouseMoved, this, &GuiInteractiveElement::mouseMoved);
	ofRemoveListener(ofEvents().mousePressed, this, &GuiInteractiveElement::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &GuiInteractiveElement::mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &GuiInteractiveElement::mouseReleased);
	ofRemoveListener(ofEvents().mouseExited, this, &GuiInteractiveElement::mouseExited);
	mouseEnable = false;
}

// Key activate events
void GuiInteractiveElement::registerKeyEvent()
{
	ofAddListener(ofEvents().keyPressed, this, &GuiInteractiveElement::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &GuiInteractiveElement::keyReleased);
}
void GuiInteractiveElement::unregisterKeyEvent()
{
	ofRemoveListener(ofEvents().keyPressed, this, &GuiInteractiveElement::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &GuiInteractiveElement::keyReleased);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiBaseElement::GuiBaseElement()
{
	colorStates = ofVec3f(1.0, 0.8, 0.6);
	setTextAlignment(LEFT, vCENTER);
}
GuiBaseElement::~GuiBaseElement(){}

void GuiBaseElement::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height)
{
	GuiElement::setup(style, width, height);

	elementStyle = style->getStyleType();

	this->font = font;
	this->fontColor = fontColor;
	this->label = label;
}

void GuiBaseElement::draw(int x, int y)
{
	ofEnableAlphaBlending();

	if (doPositionInDraw)
		setPosition(x, y);

	float modifier = colorStates.x;

	if (state) {
		modifier = colorStates.z;
	}
	else if (mouseInside) {
		modifier = colorStates.y;
	}
	style->draw(x, y, width, height, elementStyle, modifier);

	int textX = font.setAlignedX(textAlignment.h, width, label);
	int textY = font.setAlignedY(textAlignment.v, height, label);

	font.draw(label, x + textX, y + textY, fontColor);

	ofDisableAlphaBlending();
}
void GuiBaseElement::setTextAlignment(AlignmentTypeHorizontal h, AlignmentTypeVertical v)
{
	textAlignment.h = h;
	textAlignment.v = v;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiButton::GuiButton()
{
	setTextAlignment(hCENTER, vCENTER);
}
GuiButton::~GuiButton() {}


void GuiButton::draw(int x, int y)
{
	ofEnableAlphaBlending();

	if (doPositionInDraw)
		setPosition(x, y);

	float modifier = colorStates.x;

	if (state) {
		modifier = colorStates.z;
	}
	else if (mouseInside) {
		modifier = colorStates.y;
	}
	style->draw(x, y, width, height, elementStyle, modifier);

	int textX = font.setAlignedX(textAlignment.h, width, label);
	int textY = font.setAlignedY(textAlignment.v, height, label);

	font.draw(label, x + textX, y + textY, fontColor);

	ofDisableAlphaBlending();
}



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
template<typename Type>
GuiSlider<Type>::GuiSlider()
{
	isHorizontal = true;
	sliderStyle = nullptr;
	setTextAlignment(LEFT, vCENTER);

	if (typeid(value) == typeid(float))
	{
		isFloat = true;
	}
	else
	{
		isFloat = false;
	}
	
}
template<typename Type>
GuiSlider<Type>::~GuiSlider() 
{
	delete sliderStyle;
}

template<typename Type>
void GuiSlider<Type>::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height)
{
	GuiBaseElement::setup(style, font, fontColor, label, width, height);
	if (width > height) {
		isHorizontal = true;
		setSliderStyle(ofVec4f(1.0, 1.0, 1.0, 1.0), ofVec4f(0.0, 1.0, 0, 0), SLOPED);
	}else{
		isHorizontal = false;
		setSliderStyle(ofVec4f(1.0, 1.0, 1.0, 1.0), ofVec4f(0, 0, 0.0, 1.0), SLOPED);
	}
}
template<typename Type>
void GuiSlider<Type>::setSliderStyle(ofVec4f color, ofVec4f gradient, StyleTypes style)
{
	if (sliderStyle)
		delete sliderStyle;

	GuiStyle* sStyle = new GuiStyle();
	sStyle->setup(color, gradient, style);

	sliderStyle = sStyle;
}
template<typename Type>
void GuiSlider<Type>::setSliderStyle(ofVec4f color, ofVec4f gradient, ofTexture& texture, StyleTypes style)
{
	if (sliderStyle)
		delete sliderStyle;

	GuiStyleTexture* sStyle = new GuiStyleTexture();
	sStyle->setup(color, gradient, texture, false, style);

	sliderStyle = sStyle;
}

template<typename Type>
void GuiSlider<Type>::draw(int x, int y)
{
	setValue();
	
	ofEnableAlphaBlending();

	if (doPositionInDraw)
		setPosition(x, y);

	float modifier = colorStates.x;

	if (state) {
		modifier = colorStates.z;
	}
	else if (mouseInside) {
		modifier = colorStates.y;
	}
	style->draw(x, y, width, height, elementStyle, modifier);

	ofVec4f valueColor = fontColor;//sliderStyle->getColor();
	
	if (isHorizontal) 
	{
		int w = (width-4) * getDragProcentage().x;
		int h = height / 2;
		sliderStyle->setTexCoordX((float)w/(width-4));
		sliderStyle->draw(x + 2, y + h / 2, w, h, 1.0f);

		int textX = font.setAlignedX(textAlignment.h, width, label);
		int textY = font.setAlignedY(textAlignment.v, height, label);

		font.draw(label, x + textX + 1, y + textY, fontColor);

		string valueStr = (isFloat) ? ofToString(value, significantDigits) : ofToString(value);

		textX = font.setAlignedX(RIGHT, width, valueStr);
		textY = font.setAlignedY(vCENTER, height, valueStr);

		float   intencity = getDragProcentage().x;
		if (intencity > 0.5) {
			intencity = 0.5;
		}
		else if (intencity < 0.0) {
			intencity = 0.0;
		}
		valueColor = ofVec4f(1.0) - valueColor * intencity;
		valueColor.w = (fontColor.w + intencity > 1.0) ? 1.0 : fontColor.w + intencity;

		font.draw(valueStr, x + textX - 1, y + textY, valueColor);
	}
	else 
	{
		int w = width / 2;
		int h =(height - 4) * getDragProcentage().y;
		if (h < 0)
			h = 0;

		sliderStyle->setTexCoordY((float)h / (height - 4));
		sliderStyle->draw(x + w / 2, y + (height-4), w, -h, 1.0f);

		if(label.size() != 0)
		{
			int letterHeight = 0;
			
			for (int i = 0; i < label.size(); i++) 
			{
				string letter = label.substr(i, 1);
				int textX = font.setAlignedX(hCENTER, width, letter);
				int textY = font.setAlignedY(TOP, height, letter);

				font.draw(letter, x + textX, y + letterHeight + textY, fontColor);
				letterHeight += textY;
			}
		}
		
		string valueStr = (isFloat) ? ofToString(value, significantDigits) : ofToString(value);

		if(valueStr.size() != 0)
		{
			int letterHeight = 0;
			int textY = font.setAlignedY(BUTTOM, height, valueStr.substr(valueStr.size() - 1, 1));

			for (int i = valueStr.size() - 1; i >= 0; i--)
			{
				string letter = valueStr.substr(i, 1);
				int textX = font.setAlignedX(hCENTER, width, letter);
				
				float   intencity = getDragProcentage().y;
				if (intencity > 0.5) {
					intencity = 0.5;
				}else if (intencity < 0.0) {
					intencity = 0.0;
				}
				valueColor = ofVec4f(1.0) - valueColor * intencity;
				valueColor.w = (fontColor.w + intencity > 1.0) ? 1.0: fontColor.w + intencity;

				font.draw(letter, x + textX, y + textY - letterHeight, valueColor);
				letterHeight += font.getTextHeight(letter);
			}
		}
	}
	ofDisableAlphaBlending();
}
template<typename Type>
void GuiSlider<Type>::setRange(Type min, Type max, Type interval)
{
	this->min = min;
	this->max = max;
	this->interval = interval;
	
	if (isFloat)
	{
		string tmp = ofToString(this->interval);
		size_t found = tmp.find_last_of("/.");
		significantDigits = tmp.substr(found + 1).size();
	}
	else 
	{
		significantDigits = 0;
	}
}
template<typename Type>
void GuiSlider<Type>::setValue() 
{
	if (isHorizontal) {
		int intervals = ((float)max - (float)min) / interval;
		int positionInIntervals = intervals * getDragProcentage().x; 
		value = min + positionInIntervals * interval;
	}
	else {
		int intervals = ((float)max - (float)min) / interval;
		int positionInIntervals = intervals * getDragProcentage().y;
		value = min + positionInIntervals * interval;
	}
	if(value > max)
	{
		value = max;
	}
	else if (value < min)
	{
		value = min;
	}
}
template<typename Type>
void GuiSlider<Type>::setValue(Type newValue)
{
	float total = (float)max - (float)min;
	float pos = ((float)max - newValue) / total;
	setDragProcentage(pos, pos);
}
template class GuiSlider<int>;
template class GuiSlider<unsigned int>;
template class GuiSlider<float>;
template class GuiSlider<double>;
template class GuiSlider<signed char>;
template class GuiSlider<unsigned char>;
template class GuiSlider<unsigned short>;
template class GuiSlider<short>;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------


GuiInput::GuiInput()
{
	setTextAlignment(hCENTER, vCENTER);
}
GuiInput::~GuiInput() {}

void GuiInput::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height)
{
	GuiBaseElement::setup(style, font, fontColor, label, width, height);
}
void GuiInput::draw(int x, int y)
{
	ofEnableAlphaBlending();

	if (doPositionInDraw)
		setPosition(x, y);

	float modifier = colorStates.x;

	if (state) {
		modifier = colorStates.z;
	}
	else if (mouseInside) {
		modifier = colorStates.y;
	}
	style->draw(x, y, width, height, elementStyle, modifier);

	int textX = height / 5.0f;
	int textY = height / 5.0f;
	int fieldWidth = width - height / 5.0f * 2;
	int fieldHeight = height / 5.0f * 3;

	ofSetColor(255 * 0.5 * fontColor.x * modifier,
			   255 * 0.5 * fontColor.y * modifier,
		       255 * 0.5 * fontColor.z * modifier,
		       128);

	ofDrawRectRounded(x + textX, y + textY, fieldWidth, fieldHeight, height / 5.0f);

	int lineX = x + height / 5.0f * 2;
	int lineY = y + height / 10.0f * 3;
	int textWidth = 0;

	if (value.empty())
	{
		textX = font.setAlignedX(textAlignment.h, width, label);
		textY = font.setAlignedY(textAlignment.v, height, label);
		ofVec4f tmpColor = ofVec4f(fontColor.x, fontColor.y, fontColor.z, 0.5);
		font.draw(label, x + textX, y + textY, fontColor*0.5);
	}
	else 
	{
		fieldWidth = width - height / 10.0f * 2;
		string tmpString = font.cutTextWidth(value, fieldWidth);
		
		textWidth = font.getTextWidth(tmpString) + height / 15.0f;
		textX = font.setAlignedX(LEFT, width, tmpString);
		textY = font.setAlignedY(vCENTER, height, tmpString);

		font.draw(tmpString, lineX + textX, y + textY, fontColor);
	}

	if (state)
	{
		if (ofGetSeconds() % 2 == 0)
		{
			ofSetColor(255 * fontColor.x,
				255 * fontColor.y,
				255 * fontColor.z,
				255 * fontColor.w);

			ofDrawLine(lineX + textWidth, lineY, lineX + textWidth, lineY + height / 10.0f * 4);
		}
	}

	ofDisableAlphaBlending();
}
bool GuiInput::keyPressed(int key)
{
	if (this->state) 
	{
		if (key >= 0 && key < 256) {
			try
			{
				char inputKey = (char)key;
				if (inputKey >= 32 && inputKey < 125) {
					value += inputKey;
					markerPosition++;
					return false;
				}
				else if (inputKey == 8) {
					if (!value.empty()) {
						value.erase(value.end() - 1);
					}
					return false;
				}

				throw std::exception();
			}
			catch (exception& e)
			{
				cerr << "Error: " << e.what() << endl;
			}
		}
	}
	return false;
}

bool GuiInput::keyReleased(int key)
{
	return false;
}

bool GuiInput::keyPressed(ofKeyEventArgs & args)
{
	return keyPressed(args.key);
}
bool GuiInput::keyReleased(ofKeyEventArgs & args)
{
	return keyReleased(args.key);
}

string GuiInput::getValue()
{
	string output = value;
	value = "";
	return output;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

GuiArrayCounter::GuiArrayCounter() 
{
	setTextAlignment(hCENTER, vCENTER);
	value.push_back(0);
	valueIndex = 0;
}
GuiArrayCounter::~GuiArrayCounter(){}

void GuiArrayCounter::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height)
{
	GuiBaseElement::setup(style, font, fontColor, label, width, height);
	addButton.setup(style, font, fontColor, "+", width/5.f, height/2.f);
	addButton.setDoPositionInDraw(false);
	subtractButton.setup(style, font, fontColor, "-", width/5.f, height/2.f);
	subtractButton.setDoPositionInDraw(false);
}
void GuiArrayCounter::draw(int x, int y) {
	ofEnableAlphaBlending();

	if (doPositionInDraw)
		setPosition(x, y);

	float modifier = colorStates.x;
	
	style->draw(x, y, width, height / 2.f, FLAT, modifier);

	style->draw(x + width / 5.f, y  + height / 2.f, width / 5.f * 3 + 1 , height / 2.f, FLAT, modifier);
	
	ofVec3f lineColor = ofVec3f(style->getColor() * 255 * 0.4f);//colorStates.z);
	ofSetColor(lineColor.x, lineColor.y, lineColor.z);
	ofDrawLine(x, y + height / 2.f-1, x + width, y + height / 2.f-1);
	ofDrawLine(x + width / 5.f, y + height / 2.f - 1, x + width / 5.f , y + height);
	ofDrawLine(x + width / 5.f * 4, y + height / 2.f - 1, x + width / 5.f * 4, y + height);


	addButton.setPosition(position.x + width / 5.f * 4+1, position.y + height / 2.f);
	subtractButton.setPosition(position.x, position.y + height / 2.f);

	addButton.draw(x + width / 5.f * 4+1, y + height / 2.f);
	subtractButton.draw(x, y + height / 2.f);

	int textX = font.setAlignedX(textAlignment.h, width, label);
	int textY = font.setAlignedY(textAlignment.v, height / 2.f, label);

	font.draw(label, x + textX, y + textY, fontColor);

	string valueStr = ofToString(value[valueIndex]);
	textX = width / 5.f + font.setAlignedX(textAlignment.h, width/5.f*3, valueStr);
	textY = height / 2.f + font.setAlignedY(textAlignment.v, height / 2.f, valueStr);

	font.draw(valueStr , x + textX, y + textY, fontColor);

	ofDisableAlphaBlending();
}
bool GuiArrayCounter::mouseMoved(int x, int y)
{
	addButton.mouseMoved(x, y);
	subtractButton.mouseMoved(x, y);

	if (isPointInsideElement(x, y))
	{
		return true;
	}
	return false;
}
bool GuiArrayCounter::mousePressed(int x, int y, int button)
{
	if (isPointInsideElement(x, y))
	{
		if (addButton.mousePressed(x, y, button)) {
			if (valueIndex < value.size()-1) {
				valueIndex++;
			}
			addButton.setState(false);
		}
		if (subtractButton.mousePressed(x, y, button)) {
			if (valueIndex > 0) {
				valueIndex--;
			}
			subtractButton.setState(false);
		}

		return true;
	}
	return false;
}
void GuiArrayCounter::setValue(vector<int> values, int startIndex)
{
	value.clear();
	for (int i = 0; i < values.size(); i++) 
	{
		value.push_back(values[i]);
	}
	valueIndex = startIndex;
	if (startIndex < 0) {
		valueIndex = 0;
	}
	if (startIndex > value.size() - 1) {
		valueIndex = value.size() - 1;
	}
}
void GuiArrayCounter::setValue(int values[], int size, int startIndex)
{
	value.clear();
	for (int i = 0; i < size; i++) 
	{
		value.push_back(values[i]);
	}
	valueIndex = startIndex;
	if (startIndex < 0) {
		valueIndex = 0;
	}
	if (startIndex > value.size() - 1) {
		valueIndex = value.size() - 1;
	}
}
void GuiArrayCounter::setValue(int maxValue)
{
	for (int i = 0; i < value.size(); i++)
	{
		if (value[i] <= maxValue)
		{
			valueIndex = i;
		}
	}
}