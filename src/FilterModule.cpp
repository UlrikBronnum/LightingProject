#include "FilterModule.h"

void FilterModule::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	inputGui.setup(style, font, fontColor, "", width, height);
}
void FilterModule::drawPixels(int x, int y, int w, int h)
{
	image.draw(x, y, w, h);
}
void FilterModule::drawMenu(int x, int y) 
{
	ofDisableDepthTest();
	inputGui.draw(x, y);
}
void FilterModule::setReference(ofImage& base)
{
	image = base;
}

void FilterModule::setReference(ofTexture& base)
{
	image.getTexture() = base;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void FilterModuleGPU::setReference(ofImage& base)
{
	referenceTexture = base.getTexture();
}

void FilterModuleGPU::setReference(ofTexture& base)
{
	referenceTexture = base;
}
void FilterModuleGPU::drawPixels(int x, int y, int w, int h)
{
	ofSetColor(255);
	fbo.draw(x, y, w, h);
}
ofImage* FilterModuleGPU::getImage()
{
	//fbo.readToPixels( image.getPixelsRef() );
	image.getTexture() = fbo.getTexture();
	return &image;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void Blur::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Blur");
	inputGui.addGuiButton(box, "Box", width, height);
	inputGui.addGuiButton(disc, "Disc", width, height);
	inputGui.addGuiButton(gaussian, "Gaus", width, height);
	inputGui.addGuiInt(radius, "Radius", width, height,1,25,1);
	box.setState(true);

	
	kernelState = 0;
	radiusValue = radius.getValue();

	inputGui.setEventEnabled(true);
}
void Blur::apply()
{
	setKernel();
	GPUProcessing::blur(referenceTexture, fbo, 1024, 1024, kernel);
}
void Blur::setKernel()
{
	if (kernelState != 0 && box.getState())
	{
		disc.setState(false);
		gaussian.setState(false);

		if (kernelState == 2)
			inputGui.eraseLastElement();

		kernelState = 0;
	}
	else if(kernelState != 1 && disc.getState())
	{
		box.setState(false);
		gaussian.setState(false);

		if (kernelState == 2)
			inputGui.eraseLastElement();

		kernelState = 1;

	}
	else if (kernelState != 2 && gaussian.getState())
	{
		box.setState(false);
		disc.setState(false);
		
		kernelState = 2;

		inputGui.addGuiFloat(sigma, "Sigma", inputGui.getWidth(), inputGui.getHeight(), 0.1, 2.0, 0.01);
	}
	reloadKernel();
}
void Blur::reloadKernel()
{
	bool radiusHasChanged = false;
	int newRadius = radius.getValue();
	if (newRadius != radiusValue)
	{
		radiusValue = newRadius;
		radiusHasChanged = true;
		kernel.setSize(radiusValue * 2 + 1);
	}
	if (kernelState == 2) 
	{
		sigmaValue = sigma.getValue();
		kernel.gaussianKernel(sigmaValue);
	}
	else {
		if (kernelState == 0)
		{
			kernel.boxKernel();
		}
		else 
		{
			kernel.discKernel();
		}
	}
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MinMax::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Blur");
	inputGui.addGuiButton(getMin, "Min or Max", width, height);
	inputGui.addGuiButton(box, "Box", width, height);
	inputGui.addGuiButton(disc, "Disc", width, height);
	inputGui.addGuiButton(cross, "Cross", width, height);
	inputGui.addGuiInt(radius, "Radius", width, height, 1, 25, 1);
	box.setState(true);

	kernelState = 0;
	radiusValue = radius.getValue();

	inputGui.setEventEnabled(true);
}
void MinMax::apply()
{
	setKernel();
	GPUProcessing::minMax(referenceTexture, fbo, 1024, 1024, kernel, getMin.getValue());
}
void MinMax::setKernel()
{
	if (kernelState != 0 && box.getState())
	{
		disc.setState(false);
		cross.setState(false);

		kernelState = 0;
	}
	else if (kernelState != 1 && disc.getState())
	{
		box.setState(false);
		cross.setState(false);

		kernelState = 1;
	}
	else if (kernelState != 2 && cross.getState())
	{
		box.setState(false);
		disc.setState(false);

		kernelState = 2;
	}
	reloadKernel();
}
void MinMax::reloadKernel()
{
	bool radiusHasChanged = false;
	int newRadius = radius.getValue();
	if (newRadius != radiusValue)
	{
		radiusValue = newRadius;
		radiusHasChanged = true;
		kernel.setSize(radiusValue * 2 + 1);
	}
	if (kernelState == 0)
	{
		kernel.boxKernelBinary();
	}
	else if (kernelState == 1)
	{
		kernel.discKernelBinary();
	}
	else
	{
		kernel.crossKernelBinary();
	}
	
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void Morph::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Blur");
	inputGui.addGuiButton(doDilation, "Dilation / Erosion", width, height);
	inputGui.addGuiButton(box, "Box", width, height);
	inputGui.addGuiButton(disc, "Disc", width, height);
	inputGui.addGuiButton(cross, "Cross", width, height);
	inputGui.addGuiInt(radius, "Radius", width, height, 1, 25, 1);
	box.setState(true);

	kernelState = 0;
	radiusValue = radius.getValue();

	inputGui.setEventEnabled(true);
}
void Morph::apply()
{
	setKernel();
	int tmpValue = 0;
	if (doDilation.getValue()) {
		tmpValue = 1;
	}
	else {
		tmpValue = kernelSum;
	}
	GPUProcessing::morph(referenceTexture, fbo, 1024, 1024, kernel,tmpValue);
}
void Morph::setKernel()
{
	if (kernelState != 0 && box.getState())
	{
		disc.setState(false);
		cross.setState(false);

		kernelState = 0;
	}
	else if (kernelState != 1 && disc.getState())
	{
		box.setState(false);
		cross.setState(false);

		kernelState = 1;
	}
	else if (kernelState != 2 && cross.getState())
	{
		box.setState(false);
		disc.setState(false);

		kernelState = 2;
	}
	reloadKernel();
}
void Morph::reloadKernel()
{
	bool radiusHasChanged = false;
	int newRadius = radius.getValue();
	if (newRadius != radiusValue)
	{
		radiusValue = newRadius;
		radiusHasChanged = true;
		kernel.setSize(radiusValue * 2 + 1);
	}
	if (kernelState == 0)
	{
		kernelSum = kernel.boxKernelBinary();
	}
	else if (kernelState == 1)
	{
		kernelSum = kernel.discKernelBinary();
	}
	else
	{
		kernelSum = kernel.crossKernelBinary();
	}

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void Edge::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Edge Detection");
	inputGui.addGuiButton(prewitt, "Prewitt", width, height);
	inputGui.addGuiButton(sobel, "Sobel", width, height);
	inputGui.addGuiButton(kirsch, "Kirsch", width, height);
	inputGui.addGuiButton(scharr, "Scharr", width, height);
	prewitt.setState(true);

	inputGui.setEventEnabled(true);


	outputControl.setup(style, font, fontColor, "Output", width, height);
	outputControl.addGuiButton(difference, "Difference", width, height);
	outputControl.addGuiButton(magnitude, "Magnitude", width, height);
	outputControl.addGuiButton(normalized, "Normalized", width, height);
	outputControl.addGuiButton(absolute, "Absolute", width, height);
	outputControl.addGuiButton(threshold, "Threshold", width, height);
	difference.setState(true);

	output = (EdgeOutput)1;
	
	outputControl.setEventEnabled(true);
}
void Edge::apply()
{
	setKernel();
	setOutput();

	float value = thresholdValue.getValue();
	switch (kernelState)
	{
	case 0:
		GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, prewitt_horizontal, prewitt_vertical, prewitt_diagonal, output, value);
		break;
	case 1:
		GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, sobel_horizontal, sobel_vertical, sobel_diagonal, output, value);
		break;
	case 2:
		GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, kirsch_horizontal, kirsch_vertical, kirsch_diagonal, output, value);
		break;
	case 3:
		GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, scharr_horizontal, scharr_vertical, scharr_diagonal, output, value);
		break;
	default:
		break;
	}
}
void Edge::setKernel()
{
	if (kernelState != 0 && prewitt.getState())
	{
		sobel.setState(false);
		kirsch.setState(false);
		scharr.setState(false);

		kernelState = 0;
	}
	else if (kernelState != 1 && sobel.getState())
	{
		prewitt.setState(false);
		kirsch.setState(false);
		scharr.setState(false);

		kernelState = 1;

	}
	else if (kernelState != 2 && kirsch.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		scharr.setState(false);

		kernelState = 2;

	}
	else if (kernelState != 3 && scharr.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		kirsch.setState(false);

		kernelState = 3;
	}
}
void Edge::setOutput()
{
	if (output != 0 && threshold.getState())
	{
		difference.setState(false);
		magnitude.setState(false);
		normalized.setState(false);
		absolute.setState(false);

		if (output != (EdgeOutput)0){
			outputControl.addGuiInt(thresholdValue, "Threshold", inputGui.getWidth(), inputGui.getHeight(), 0, 255, 1);
		}

		output = (EdgeOutput)0;
	}
	else if (output != 1 && difference.getState())
	{
		threshold.setState(false);
		magnitude.setState(false);
		normalized.setState(false);
		absolute.setState(false);

		if (output == (EdgeOutput)0)
			outputControl.eraseLastElement();

		output = (EdgeOutput)1;

	}
	else if (output != 2 && magnitude.getState())
	{
		threshold.setState(false);
		difference.setState(false);
		normalized.setState(false);
		absolute.setState(false);

		if (output == (EdgeOutput)0)
			outputControl.eraseLastElement();

		output = (EdgeOutput)2;

	}
	else if (output != 3 && normalized.getState())
	{
		threshold.setState(false);
		difference.setState(false);
		magnitude.setState(false);
		absolute.setState(false);

		if (output == (EdgeOutput)0)
			outputControl.eraseLastElement();

		output = (EdgeOutput)3;
	}
	else if (output != 4 && absolute.getState())
	{
		threshold.setState(false);
		difference.setState(false);
		magnitude.setState(false);
		normalized.setState(false);

		if (output == (EdgeOutput)0)
			outputControl.eraseLastElement();

		output = (EdgeOutput)4;
	}
}
void Edge::drawMenu(int x, int y)
{
	ofDisableDepthTest();
	inputGui.draw(x, y);
	outputControl.draw(x,y+inputGui.getSize().y);
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void Normalmap::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Edge Detection");
	inputGui.addGuiButton(prewitt, "Prewitt", width, height);
	inputGui.addGuiButton(sobel, "Sobel", width, height);
	inputGui.addGuiButton(kirsch, "Kirsch", width, height);
	inputGui.addGuiButton(scharr, "Scharr", width, height);
	inputGui.addGuiFloat(roughness, "Roughness", width, height,0.0,2.0,0.01);
	roughness.setValue(1.0);
	prewitt.setState(true);

	inputGui.setEventEnabled(true);
}
void Normalmap::apply()
{
	setKernel();
	switch (kernelState)
	{
	case 0:
		GPUProcessing::normalmap(	referenceTexture, fbo, 1024, 1024, 
									prewitt_horizontal, prewitt_vertical, 
									roughness.getValue(), ofVec3f(1.0, 0.0, 0.0));
		break;
	case 1:
		GPUProcessing::normalmap(	referenceTexture, fbo, 1024, 1024,		
									sobel_horizontal, sobel_vertical, 
									roughness.getValue(), ofVec3f(1.0, 0.0, 0.0));
		break;
	case 2:
		GPUProcessing::normalmap(	referenceTexture, fbo, 1024, 1024, 
									kirsch_horizontal, kirsch_vertical, 
									roughness.getValue(), ofVec3f(1.0, 0.0, 0.0));
		break;
	case 3:
		GPUProcessing::normalmap(	referenceTexture, fbo, 1024, 1024, 
									scharr_horizontal, scharr_vertical, 
									roughness.getValue(), ofVec3f(1.0, 0.0, 0.0));
		break;
	default:
		break;
	}
}
void Normalmap::setKernel()
{
	if (kernelState != 0 && prewitt.getState())
	{
		sobel.setState(false);
		kirsch.setState(false);
		scharr.setState(false);

		kernelState = 0;
	}
	else if (kernelState != 1 && sobel.getState())
	{
		prewitt.setState(false);
		kirsch.setState(false);
		scharr.setState(false);

		kernelState = 1;

	}
	else if (kernelState != 2 && kirsch.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		scharr.setState(false);

		kernelState = 2;

	}
	else if (kernelState != 3 && scharr.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		kirsch.setState(false);

		kernelState = 3;
	}
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void Gamma::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	inputGui.addGuiFloat(gamma, "Gamma", width, height, 0.0, 5.0, 0.01);
	gamma.setValue(1.0);

	inputGui.setEventEnabled(true);
}
void Gamma::apply()
{
	GPUProcessing::gamma(referenceTexture, fbo, 1024, 1024, gamma.getValue());
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void BrightnessContrast::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	inputGui.addGuiInt(brightness, "Brightness", width, height, -255, 255, 1);
	inputGui.addGuiInt(contrast, "Contrast", width, height, -255, 255, 1);

	brightness.setValue(0);
	contrast.setValue(0);

	inputGui.setEventEnabled(true);
}
void BrightnessContrast::apply()
{
	GPUProcessing::brightCont(referenceTexture, fbo, 1024, 1024, brightness.getValue(),contrast.getValue());
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void Invert::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	
	inputGui.setEventEnabled(true);
}
void Invert::apply()
{
	GPUProcessing::invert(referenceTexture, fbo, 1024, 1024);
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void Threshold::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	inputGui.addGuiInt(thresholdRedMax, "Red greater", width, height, 0, 255, 1);
	inputGui.addGuiInt(thresholdRedMin, "Red lesser", width, height, 0, 255, 1);
	inputGui.addGuiInt(thresholdGreenMax, "Green greater", width, height, 0, 255, 1);
	inputGui.addGuiInt(thresholdGreenMin, "Green lesser", width, height, 0, 255, 1);
	inputGui.addGuiInt(thresholdBlueMax, "Blue greater", width, height, 0, 255, 1);
	inputGui.addGuiInt(thresholdBlueMin, "Blue lesser", width, height, 0, 255, 1);

	inputGui.setEventEnabled(true);

	thresholdMax = ofVec3f(	thresholdRedMax.getValue(),
							thresholdGreenMax.getValue(),
							thresholdBlueMax.getValue());

	thresholdMin = ofVec3f(	thresholdRedMin.getValue(),
							thresholdGreenMin.getValue(),
							thresholdBlueMin.getValue());

	hasChanged = true;
}
void Threshold::apply()
{
	setValue();

	if (hasChanged) {
		GPUProcessing::threshold(referenceTexture, fbo, 1024, 1024, thresholdMax, thresholdMin);
		hasChanged = false;
		cout << "threshold" << endl;
	}
}
void Threshold::setValue()
{
	ofVec3f tmpMax = ofVec3f(	thresholdRedMax.getValue(),
								thresholdGreenMax.getValue(),
								thresholdBlueMax.getValue());

	ofVec3f diffMax = tmpMax - thresholdMax;

	ofVec3f tmpMin = ofVec3f(	thresholdRedMin.getValue(),
								thresholdGreenMin.getValue(),
								thresholdBlueMin.getValue());

	ofVec3f diffMin = tmpMin - thresholdMin;

	if ( ( diffMax.x != 0 || diffMax.y != 0 || diffMax.z != 0 ) ||
		 ( diffMin.x != 0 || diffMin.y != 0 || diffMin.z != 0 ) ) 
	{
		hasChanged = true;
	}
		
	thresholdMax = tmpMax;
	thresholdMin = tmpMin;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void HSLCorrection::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	inputGui.addGuiFloat(hue, "Hue", width, height, -360.0, 360.0, 0.01);
	inputGui.addGuiFloat(saturation, "Saturation", width, height, -1.0, 1.0, 0.01);
	inputGui.addGuiFloat(luminance, "Luminance", width, height, -1.0, 1.0, 0.01);

	hue.setValue(0);
	saturation.setValue(0);
	luminance.setValue(0);

	inputGui.setEventEnabled(true);
}
void HSLCorrection::apply()
{
	GPUProcessing::hslCorrection(referenceTexture, fbo, 1024, 1024, hue.getValue(), saturation.getValue() , luminance.getValue());
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void FilterModuleCPU::setReference(ofImage& base)
{
	referencePixels = base.getPixelsRef();

	if (referencePixels.getNumChannels() == 3)
		image.allocate(referencePixels.getWidth(), referencePixels.getHeight(), OF_IMAGE_COLOR);
	else if (referencePixels.getNumChannels() == 4)
		image.allocate(referencePixels.getWidth(), referencePixels.getHeight(), OF_IMAGE_COLOR_ALPHA);

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void HistogramStrech::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	inputGui.addGuiInt(newMax, "Max", width, height, 0, 255, 1);
	inputGui.addGuiInt(newMin, "Min", width, height, 0, 255, 1);

	inputGui.setElementPosition(0, 0, 0);
	inputGui.setElementPosition(0, height + 1, 1);

	inputGui.setEventEnabled(true);
}
void HistogramStrech::apply()
{
	ColorProcessing::colorStretch(	referencePixels, image.getPixels(),
									newMin.getValue(), newMax.getValue());
	image.update();

}

