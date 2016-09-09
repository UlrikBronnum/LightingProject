#pragma once

#include "ofMain.h"

#include "Gui.h"
#include "ImageProcessing.h"


class FilterModule
{
public:

	virtual void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) ;

	virtual void apply() {};

	virtual void setReference(ofImage& base) ;
	virtual void setReference(ofTexture& base) ;

	virtual void drawPixels(int x, int y, int w, int h);
	virtual void drawMenu(int x, int y);

	virtual ofImage* getImage() { return &image; };

	virtual float getControlerHeight() { return inputGui.getSize().y; }

	virtual void enable() { inputGui.enableEvents(); }
	virtual void disable() { inputGui.disableEvents(); }

protected:

	ofImage		image;

	Gui			inputGui;

	bool		hasChanged;
};

class FilterModuleGPU : public FilterModule
{
public:

	virtual void apply() = 0;

	virtual void setReference(ofImage& base);
	virtual void setReference(ofTexture& base);

	virtual void drawPixels(int x, int y, int w, int h) override;

	ofImage* getImage() override;

protected:

	ofTexture	referenceTexture;
	ofFbo		fbo;
};

class Blur : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:
	 
	void setKernel();

	void reloadKernel();


	GuiButton	box;
	GuiButton	disc;
	GuiButton	gaussian;

	GuiInt		radius;
	GuiFloat	sigma;
	
	int			kernelState;

	int			radiusValue;
	float		sigmaValue;
	

	Kernel	  kernel;
};
class MinMax : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	void setKernel();

	void reloadKernel();


	GuiButton	getMin;
	GuiButton	box;
	GuiButton	disc;
	GuiButton	cross;

	GuiInt		radius;

	int			kernelState;

	int			radiusValue;

	Kernel	  kernel;
};
class Morph : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	void setKernel();

	void reloadKernel();


	GuiButton	doDilation;
	GuiButton	box;
	GuiButton	disc;
	GuiButton	cross;

	GuiInt		radius;

	int			kernelState;
	int			kernelSum;

	int			radiusValue;
	

	Kernel	  kernel;
};
class Edge : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

	void drawMenu(int x, int y) override;
	
	float getControlerHeight() override { return inputGui.getSize().y + outputControl.getSize().y; }

	virtual void enable() override {
		inputGui.enableEvents(); 
		outputControl.enableEvents();
	}
	virtual void disable() override {
		inputGui.disableEvents(); 
		outputControl.disableEvents();
	}

protected:

	void setKernel();
	void setOutput();

	GuiButton	prewitt;
	GuiButton	sobel;
	GuiButton	kirsch;
	GuiButton	scharr;

	int			kernelState;

	EdgeOutput	output;

	Gui			outputControl;
	GuiButton	threshold;
	GuiButton	difference;
	GuiButton	magnitude;
	GuiButton	normalized;
	GuiButton	absolute;
	GuiInt		thresholdValue;

	int			outputState;
};

class Normalmap : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	void setKernel();

	GuiButton	prewitt;
	GuiButton	sobel;
	GuiButton	kirsch;
	GuiButton	scharr;
	GuiFloat	roughness;

	int			kernelState;
};
class Gamma : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	GuiFloat	gamma;
	
};
class BrightnessContrast : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	GuiInt	brightness;
	GuiInt	contrast;

};

class Invert : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:


};

class Threshold : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	void setValue();

	GuiInt thresholdRedMax;
	GuiInt thresholdRedMin;
	GuiInt thresholdGreenMax;
	GuiInt thresholdGreenMin;
	GuiInt thresholdBlueMax;
	GuiInt thresholdBlueMin;

	ofVec3f	thresholdMax;
	ofVec3f thresholdMin;

};
class HSLCorrection : public FilterModuleGPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	GuiFloat	hue;
	GuiFloat	saturation;
	GuiFloat	luminance;

};
class FilterModuleCPU : public FilterModule
{
public:

	virtual void apply() = 0;

	virtual void setReference(ofImage& base);
	virtual void setReference(ofTexture& base) {};

protected:

	ofPixels	referencePixels;
};


class HistogramStrech : public FilterModuleCPU
{
public:

	void setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height) override;
	void apply() override;

protected:

	GuiInt	newMax;
	GuiInt	newMin;
};