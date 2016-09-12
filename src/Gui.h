#pragma once

#include "ofMain.h"


enum StyleTypes 
{
	GRADIENT = 0,
	SLOPED,
	FLAT
};

class GuiStyle 
{
public:
	GuiStyle();
	~GuiStyle();

	virtual void setup(ofVec4f color, ofVec4f gradient = ofVec4f(1,0,1,0), StyleTypes style = GRADIENT);
	virtual void draw(int x, int y, int w, int h, float colorModifier = 1.0f);
	virtual void draw(int x, int y, int w, int h, StyleTypes customStyle, float colorModifier = 1.0f);

	virtual void setTexCoordX(float xTexcoordMax);
	virtual void setTexCoordY(float yTexcoordMax);

	ofVec4f getColor() { return color; }
	StyleTypes getStyleType(){ return style;}


protected:

	void setPosition(int x, int y, int w, int h);

	ofVec4f		color;
	ofVec4f		gradient;

	StyleTypes	style;

	ofMesh		mesh;
	ofShader	shader;

	bool		customTexcoords;
};

class GuiStyleTexture : public GuiStyle {
public:
	GuiStyleTexture();
	~GuiStyleTexture();

	virtual void setup(ofVec4f color, ofVec4f gradient, ofTexture& texture, bool destortTexture = false, StyleTypes style = GRADIENT) ;
	virtual void draw(int x, int y, int w, int h, float colorModifier = 1.0f) override;
	virtual void draw(int x, int y, int w, int h, StyleTypes customStyle, float colorModifier = 1.0f)override;

	void	setTexCoords(ofVec2f texCoord1, ofVec2f texCoord2, ofVec2f texCoord3, ofVec2f texCoord4) ;
	void	setTexCoordX(float xTexcoordMax) override;
	void	setTexCoordY(float yTexcoordMax) override;

protected:

	ofVec2f		setTexCoords(int w, int h);
	ofVec2f		getMaxTexCoords();

	bool		destortTexture;
	ofTexture	texture;
};


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

enum AlignmentTypeVertical {
	ABOVE = 3,
	TOP = 0,
	vCENTER = 1,
	BUTTOM = 2,
	BELOW = 4
};
enum AlignmentTypeHorizontal {
	LEFT = 0,
	hCENTER = 1,
	RIGHT = 2
};
struct TextAlignment {
	TextAlignment::TextAlignment() {
		v = vCENTER;
		h = hCENTER;
	}
	AlignmentTypeVertical v;
	AlignmentTypeHorizontal h;
};

class GuiFont
{
public:
	GuiFont();
	virtual ~GuiFont();

	// Setup font variables
	virtual void setup(string fontLocation, int fontSize);
	virtual void setup(ofTrueTypeFont& nFont);

	// Draw a string
	void draw(string text, int x, int y, ofColor color);
	void draw(string text, int x, int y, ofVec4f color);

	// Get width of a given string
	float getTextWidth(string text);
	// Get height of a given string
	float getTextHeight(string text);
	// Set line height
	void setLineHeight(float h);
	// Get line height
	float getLineHeight();
	// Set space width
	void setSpaceWidth(float w);
	// Get space width
	float getSpaceWidth();
	//
	string getFontLocation() { return fontLocation; }
	int getFontSize() { return fontSize; }
	//
	ofRectangle getBoundingBox(string text);

	// Fit text to text field size
	string cutTextWidth(string inputStr, float width);
	// Fit text to text field size
	string adjustTextWidth(string inputStr, float width);
	string adjustTextWidth(string inputStr, float width, int& stringHeight);
	// Fit text to text field size
	int findTextWidth(string inputStr, float width);

	// AlignTextToParent
	int setAlignedX(AlignmentTypeHorizontal hori, int width, string text);
	int setAlignedY(AlignmentTypeVertical vert, int height, string text);
	
	ofTrueTypeFont* getFont() { return &font; }

protected:

	string              fontLocation;
	int                 fontSize;

	ofTrueTypeFont		font;

};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class GuiElement
{
public:
	GuiElement();
	~GuiElement();

	virtual void setup(GuiStyle* style, int width, int height);
	virtual void draw();
	virtual void draw(int x, int y);

	bool		isPointInsideElement(int x, int y);

	void		setWidth(int w) { width = w; }
	void		setHeight(int h) { height = h; }

	int			getWidth() { return width; }
	int			getHeight() { return height; }

	void		setPosition(int x, int y);
	ofVec2f		getPosition() { return position; }
	
	void		setDoPositionInDraw(bool updatePosition){ doPositionInDraw = updatePosition; }

	virtual bool getState() { return false; }

protected:

	GuiStyle*	style;

	int			width;
	int			height;

	bool		doPositionInDraw;
	ofVec2f		position;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class GuiInteractiveElement : public GuiElement 
{
public:
	GuiInteractiveElement();
	~GuiInteractiveElement();

	virtual void draw(int x, int y) override;

	virtual bool mouseMoved(int x, int y);
	virtual bool mousePressed(int x, int y, int button);
	virtual bool mouseDragged(int x, int y, int button);
	virtual bool mouseReleased(int x, int y, int button);
	virtual bool mouseExited(int x, int y);

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseExited(ofMouseEventArgs & args);
	
	// Key interactions
	virtual bool keyPressed(int key);
	virtual bool keyReleased(int key);

	virtual bool keyPressed(ofKeyEventArgs & args);
	virtual bool keyReleased(ofKeyEventArgs & args);

	void enableEvents();
	void disableEvents();

	ofVec2f getDragProcentage() { return dragProcentage; }
	void setDragProcentage(float x, float y) { 
		dragProcentage.x = x; 
		dragProcentage.y = y;
	}
	
	bool	getState() { return this->state; }
	void	setState(bool state){ this->state = state; }

	bool	getMouseInside() { return mouseInside; }

	void	setIsSwitch(bool isSwitch) { this->isSwitch = isSwitch; }

	static int getElementCount() { return elementCount; }

protected:

	static int elementCount;

	bool	isSwitch;
	bool	state;

	bool	mouseInside;
	ofVec2f	dragProcentage;

	bool    mouseEnable;
	bool    keyEnable;

	// Mouse activate events
	void registerMouseEvent();
	void unregisterMouseEvent();

	// Key activate events
	void registerKeyEvent();
	void unregisterKeyEvent();
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class GuiBaseElement : public GuiInteractiveElement
{
public:

	GuiBaseElement();
	~GuiBaseElement();

	virtual void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height);
	virtual void draw(int x, int y) override;

	// x = inactive, y = mouseOver, z = on
	void setColorStates(ofVec3f colorStates) { this->colorStates = colorStates; }

	void setTextAlignment(AlignmentTypeHorizontal h, AlignmentTypeVertical v);

	void setLabel(string label) { this->label = label; }
	string getLabel() { return label; }

	void setElementStyleType(StyleTypes elementStyle) { this->elementStyle = elementStyle; }

protected:

	GuiFont	font;
	ofVec4f	fontColor;

	StyleTypes elementStyle;

	string	label;

	ofVec3f	colorStates;
	
	TextAlignment textAlignment;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class GuiButton : public GuiBaseElement
{
public:

	GuiButton();
	~GuiButton();

	virtual void draw(int x, int y) override;

	bool getValue() { return state; }

protected:

	
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
template<typename Type>
class GuiSlider : public GuiBaseElement
{
public:

	GuiSlider();
	~GuiSlider();

	virtual void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height) override;

	void setSliderStyle(ofVec4f color, ofVec4f gradient = ofVec4f(1, 0, 1, 0), StyleTypes style = GRADIENT);
	void setSliderStyle(ofVec4f color, ofVec4f gradient, ofTexture& texture, StyleTypes style = GRADIENT);

	virtual void draw(int x, int y) override;

	void setRange(Type min, Type max, Type interval);

	void setValue(Type newValue);
	void setValue();
	Type getValue() { return value; }

protected:

	GuiStyle*	sliderStyle;

	bool isHorizontal;

	int significantDigits;
	bool isFloat;

	Type value;
	Type max;
	Type min;
	Type interval;
};

typedef GuiSlider<float> GuiFloat;
typedef GuiSlider<int> GuiInt;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class GuiInput : public GuiBaseElement
{
public:

	GuiInput();
	~GuiInput();

	virtual void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height) override;
	virtual void draw(int x, int y) override;

	virtual bool keyPressed(int key) override;
	virtual bool keyReleased(int key) override;

	virtual bool keyPressed(ofKeyEventArgs & args) override;
	virtual bool keyReleased(ofKeyEventArgs & args) override;

	string getValue();

protected:

	string value;
	string input;

	int	markerPosition;
};

class GuiArrayCounter : public GuiBaseElement
{
public:
	GuiArrayCounter();
	~GuiArrayCounter();

	virtual void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height);
	virtual void draw(int x, int y) override;

	virtual bool mouseMoved(int x, int y) override;
	virtual bool mousePressed(int x, int y, int button) override;

	void setValue(vector<int> values, int startIndex);
	void setValue(int value[], int size, int startIndex);
	void setValue(int maxValue);

	int getValue() { return value[valueIndex]; }

protected:

	GuiButton		addButton;
	GuiButton		subtractButton;

	int				valueIndex;

	vector<int>		value;
	
};



class Gui : public GuiBaseElement
{
public:
	Gui();
	~Gui();

	virtual void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, string label, int width, int height);
	virtual void draw(int x, int y) override;
	
	bool mouseMoved(ofMouseEventArgs & args) override;
	bool mousePressed(ofMouseEventArgs & args) override;
	bool mouseDragged(ofMouseEventArgs & args) override;
	bool mouseReleased(ofMouseEventArgs & args) override;
	bool mouseExited(ofMouseEventArgs & args) override;

	// Key interactions
	bool keyPressed(ofKeyEventArgs & args) override;
	bool keyReleased(ofKeyEventArgs & args) override;

	void setElementPadding(int padding) { elementPadding = ofVec2f(padding, padding); }
	void setElementPadding(int paddingX, int paddingY) { elementPadding = ofVec2f(paddingX, paddingY); }
	void setElementPosition(int index);
	void setElementPosition(int elementPositionX, int elementPositionY, int index);
	void setElementPosition(ofVec2f elementPosition, int index);

	void updateGuiPositions();

	void addElement		(GuiBaseElement& element);

	void addGuiButton	(GuiButton& element, string label, int width, int height, bool localPtr = false);
	int addGuiButton	(string label, int width, int height, TextAlignment alignment = TextAlignment(), bool state = false, int index = -500);
	
	void addGuiFloat(	GuiFloat& element, string label, int width, int height,
						float min, float max, float interval, bool localPtr = false);
	
	void addGuiFloat(	GuiFloat& element, string label, int width, int height, 
						ofVec4f color, ofVec4f gradient, StyleTypes stylefloat,
						float min, float max, float interval, bool localPtr = false);
	
	void addGuiFloat(	GuiFloat& element, string label, int width, int height, 
						ofVec4f color, ofVec4f gradient, ofTexture& texture, StyleTypes style,
						float min, float max, float interval, bool localPtr = false);
	
	void addGuiInt( GuiInt& element, string label, int width, int height,
					int min, int max, int interval, bool localPtr = false);
	
	void addGuiInt(	GuiInt& element, string label, int width, int height, 
					ofVec4f color, ofVec4f gradient, StyleTypes style,
					int min, int max, int interval, bool localPtr = false);
	
	void addGuiInt(	GuiInt& element, string label, int width, int height, 
					ofVec4f color, ofVec4f gradient, ofTexture& texture, StyleTypes style,
					int min, int max, int interval, bool localPtr = false);
	
	void addGuiInput(GuiInput& element, string label, int width, int height, bool localPtr = false);
	
	void addGuiArrayCounter(GuiArrayCounter& element, string label, int width, int height, vector<int> value, int startIndex, bool localPtr = false);
	
	bool isPointInsideElement(int x, int y, int w, int h);
	
	void setEventEnabled(bool isEnabled);

	ofVec2f getSize() { return ofVec2f( fbo.getWidth(), fbo.getHeight() );}

	void eraseLastElement() 
	{
		if (elementIsPtr[elements.size() - 1]) {
			delete elements[elements.size() - 1];
		}
		elements.erase(elements.begin() + elements.size() - 1);
		elementPositions.erase(elementPositions.begin() + elementPositions.size() - 1);
		updateFbo = true;
		updateGuiPositions();
	}
	void eraseElement(int index)
	{
		if (index >= 0 && index < elements.size())
		{
			if (elementIsPtr[index]) {
				delete elements[index];
				elementIsPtr.erase(elementIsPtr.begin() + index);
			}
			elements.erase(elements.begin() + index);
			elementPositions.erase(elementPositions.begin() + index);
			updateFbo = true;

			for (int i = 0; i < elementPositions.size(); i++)
				setElementPosition(i);

			updateGuiPositions();
		}
	}
	int getNumElements() { return elements.size(); }

	void setElementState(int index, bool value);
	bool getElementState(int index);
	
	string getElementLabel(int index);
	void setElementLabel(int index, string label);

	ofVec2f getElementPosition(int index)
	{
		if (index > -1 && index < elementPositions.size()) {
			return ofVec2f(position.x, position.y) + elementPositions[index];
		}
		return ofVec2f(position.x, position.y);
	}
protected:

	void updateGui(int x, int y);

	ofFbo	fbo;

	bool	updateFbo;

	vector<GuiBaseElement*> elements;
	vector<ofVec2f>			elementPositions;
	vector<bool>			elementIsPtr;
	ofVec2f					elementPadding;
};