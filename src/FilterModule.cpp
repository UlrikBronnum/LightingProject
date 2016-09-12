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
	fbo.allocate(referenceTexture.getWidth(), referenceTexture.getHeight(), GL_RGB);
	//setTextureSettings();
}

void FilterModuleGPU::setReference(ofTexture& base)
{
	referenceTexture = base;
	fbo.allocate(referenceTexture.getWidth(), referenceTexture.getHeight(), GL_RGB);
	//setTextureSettings();
}
void FilterModuleGPU::drawPixels(int x, int y, int w, int h)
{
	ofSetColor(255);
	fbo.draw(x, y, w, h);
}
ofImage* FilterModuleGPU::getImage()
{
	image.getTexture() = fbo.getTexture();
	return &image;
}
void FilterModuleGPU::setTextureSettings()
{
	if (ofGetUsingArbTex())
	{
		referenceTexture.setTextureWrap(GL_CLAMP, GL_CLAMP);
		referenceTexture.enableMipmap();
		referenceTexture.generateMipmap();
		referenceTexture.setTextureMinMagFilter(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);

		fbo.getTexture().setTextureWrap(GL_CLAMP, GL_CLAMP);
		fbo.getTexture().enableMipmap();
		fbo.getTexture().generateMipmap();
		fbo.getTexture().setTextureMinMagFilter(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
	}
	else 
	{
		referenceTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
		referenceTexture.enableMipmap();
		referenceTexture.generateMipmap();
		referenceTexture.setTextureMinMagFilter(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);

		fbo.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
		fbo.getTexture().enableMipmap();
		fbo.getTexture().generateMipmap();
		fbo.getTexture().setTextureMinMagFilter(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
	}
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

	
	kernelState = -1;
	radiusValue = radius.getValue();

	inputGui.setEventEnabled(true);

	hasChanged = true;
}
void Blur::apply()
{
	setKernel();
	if (hasChanged) 
	{
		GPUProcessing::blur(referenceTexture, fbo, 1024, 1024, kernel);
		hasChanged = false;
	}
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
		hasChanged = true;
	}
	else if(kernelState != 1 && disc.getState())
	{
		box.setState(false);
		gaussian.setState(false);

		if (kernelState == 2)
			inputGui.eraseLastElement();

		kernelState = 1;
		hasChanged = true;
	}
	else if (kernelState != 2 && gaussian.getState())
	{
		box.setState(false);
		disc.setState(false);
		
		kernelState = 2;

		inputGui.addGuiFloat(sigma, "Sigma", inputGui.getWidth(), inputGui.getHeight(), 0.1, 5.0, 0.01);
		hasChanged = true;
	}
	reloadKernel();
}
void Blur::reloadKernel()
{
	int newRadius = radius.getValue();
	if (newRadius != radiusValue)
	{
		radiusValue = newRadius;
		kernel.setSize(radiusValue * 2 + 1);
		hasChanged = true;
	}
	
	if (kernelState == 0 && hasChanged)
	{
		kernel.boxKernel();
	}
	else if (kernelState == 1 && hasChanged)
	{
		kernel.discKernel();
	}
	else if (kernelState == 2)
	{
		float tmpSigma = sigma.getValue();
		if (tmpSigma != sigmaValue || hasChanged)
		{
			sigmaValue = tmpSigma;
			hasChanged = true;
			kernel.gaussianKernel(sigmaValue);
		}
	}
	
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MinMax::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Min / Max");
	inputGui.addGuiButton(getMin, "Min or Max", width, height);
	inputGui.addGuiButton(box, "Box", width, height);
	inputGui.addGuiButton(disc, "Disc", width, height);
	inputGui.addGuiButton(cross, "Cross", width, height);
	inputGui.addGuiInt(radius, "Radius", width, height, 1, 25, 1);
	box.setState(true);

	kernelState = -1;
	radiusValue = radius.getValue();

	inputGui.setEventEnabled(true);

	hasChanged = true;
	
	doMax = true;
}
void MinMax::apply()
{
	setKernel();

	bool tmp = getMin.getValue();
	if (doMax != tmp) 
	{
		doMax = tmp;
		hasChanged = true;
	}

	if (hasChanged) 
	{
		GPUProcessing::minMax(referenceTexture, fbo, 1024, 1024, kernel, doMax);
		hasChanged = false;
	}
}
void MinMax::setKernel()
{
	if (kernelState != 0 && box.getState())
	{
		disc.setState(false);
		cross.setState(false);

		kernelState = 0;
		hasChanged = true;
	}
	else if (kernelState != 1 && disc.getState())
	{
		box.setState(false);
		cross.setState(false);

		kernelState = 1;
		hasChanged = true;
	}
	else if (kernelState != 2 && cross.getState())
	{
		box.setState(false);
		disc.setState(false);

		kernelState = 2;
		hasChanged = true;
	}
	reloadKernel();
}
void MinMax::reloadKernel()
{
	int newRadius = radius.getValue();
	if (newRadius != radiusValue)
	{
		radiusValue = newRadius;
		kernel.setSize(radiusValue * 2 + 1);
		hasChanged = true;
	}
	if (kernelState == 0 && hasChanged)
	{
		kernel.boxKernelBinary();
	}
	else if (kernelState == 1 && hasChanged)
	{
		kernel.discKernelBinary();
	}
	else if (kernelState == 2 && hasChanged)
	{
		kernel.crossKernelBinary();
	}
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void Highpass::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	inputGui.addGuiInt(gRadius, "Radius", width, height, 1, 45, 1);

	inputGui.setEventEnabled(true);

	vRadius = -1.0;

	hasChanged = true;
}
void Highpass::apply()
{
	float tmpRadius = gRadius.getValue();
	if (vRadius != tmpRadius)
	{
		vRadius = tmpRadius;
		hasChanged = true;
	}
	if (hasChanged)
	{
		GPUProcessing::highpass(referenceTexture, fbo, 1024, 1024, vRadius);
		hasChanged = false;
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

	hasChanged = true;

}
void Morph::apply()
{
	setKernel();

	int tmpValue = 0;
	if (doDilation.getValue()) 
	{
		tmpValue = 1;
		if(!doADilation)
		{
			doADilation = true;
			hasChanged = true;
		}
	}
	else 
	{
		tmpValue = kernelSum;
		if (doADilation) 
		{
			doADilation = false;
			hasChanged = true;
		}
	}

	if (hasChanged) 
	{
		GPUProcessing::morph(referenceTexture, fbo, 1024, 1024, kernel, tmpValue);
		hasChanged = false;
		cout << "running" << endl;
	}
}
void Morph::setKernel()
{
	if (kernelState != 0 && box.getState())
	{
		disc.setState(false);
		cross.setState(false);

		kernelState = 0;
		hasChanged = true;
	}
	else if (kernelState != 1 && disc.getState())
	{
		box.setState(false);
		cross.setState(false);

		kernelState = 1;
		hasChanged = true;
	}
	else if (kernelState != 2 && cross.getState())
	{
		box.setState(false);
		disc.setState(false);

		kernelState = 2;
		hasChanged = true;
	}
	reloadKernel();
}
void Morph::reloadKernel()
{
	int newRadius = radius.getValue();
	if (newRadius != radiusValue)
	{
		radiusValue = newRadius;
		kernel.setSize(radiusValue * 2 + 1);
		hasChanged = true;
	}
	if (kernelState == 0 && hasChanged)
	{
		kernelSum = kernel.boxKernelBinary();
		hasChanged = true;
	}
	else if (kernelState == 1 && hasChanged)
	{
		kernelSum = kernel.discKernelBinary();
		hasChanged = true;
	}
	else if(kernelState == 2 && hasChanged)
	{
		kernelSum = kernel.crossKernelBinary();
		hasChanged = true;
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

	hasChanged = true;

	thresholdSum = -1.0;
}
void Edge::apply()
{
	setKernel();
	setOutput();

	float value = thresholdValue.getValue();
	if (value != thresholdSum)
	{
		thresholdSum = value;
		hasChanged = true;
	}
	if (hasChanged)
	{
		switch (kernelState)
		{
		case 0:
			GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, prewitt_horizontal, prewitt_vertical, prewitt_diagonal, output, thresholdSum);
			break;
		case 1:
			GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, sobel_horizontal, sobel_vertical, sobel_diagonal, output, thresholdSum);
			break;
		case 2:
			GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, kirsch_horizontal, kirsch_vertical, kirsch_diagonal, output, thresholdSum);
			break;
		case 3:
			GPUProcessing::edge(referenceTexture, fbo, 1024, 1024, scharr_horizontal, scharr_vertical, scharr_diagonal, output, thresholdSum);
			break;
		default:
			break;
		}
		hasChanged = false;
		cout << "running" << endl;
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
		hasChanged = true;
	}
	else if (kernelState != 1 && sobel.getState())
	{
		prewitt.setState(false);
		kirsch.setState(false);
		scharr.setState(false);

		kernelState = 1;
		hasChanged = true;
	}
	else if (kernelState != 2 && kirsch.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		scharr.setState(false);

		kernelState = 2;
		hasChanged = true;
	}
	else if (kernelState != 3 && scharr.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		kirsch.setState(false);

		kernelState = 3;
		hasChanged = true;
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
		hasChanged = true;
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
		hasChanged = true;
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
		hasChanged = true;
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
		hasChanged = true;
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
		hasChanged = true;
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
	kernelState = -1;
	inputGui.setEventEnabled(true);

	vRoughness = 0.0f;
	hasChanged = true;
}
void Normalmap::apply()
{
	setKernel();

	float tmpRoughness = roughness.getValue();
	if (vRoughness != tmpRoughness)
	{
		vRoughness = tmpRoughness;
		hasChanged = true;
	}

	if (hasChanged) 
	{
		switch (kernelState)
		{
		case 0:
			GPUProcessing::normalmap(referenceTexture, fbo, 1024, 1024,
				prewitt_horizontal, prewitt_vertical,
				vRoughness, ofVec3f(1.0, 0.0, 0.0));
			break;
		case 1:
			GPUProcessing::normalmap(referenceTexture, fbo, 1024, 1024,
				sobel_horizontal, sobel_vertical,
				vRoughness, ofVec3f(1.0, 0.0, 0.0));
			break;
		case 2:
			GPUProcessing::normalmap(referenceTexture, fbo, 1024, 1024,
				kirsch_horizontal, kirsch_vertical,
				vRoughness, ofVec3f(1.0, 0.0, 0.0));
			break;
		case 3:
			GPUProcessing::normalmap(referenceTexture, fbo, 1024, 1024,
				scharr_horizontal, scharr_vertical,
				vRoughness, ofVec3f(1.0, 0.0, 0.0));
			break;
		default:
			break;
		}
		hasChanged = false;
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
		hasChanged = true;
	}
	else if (kernelState != 1 && sobel.getState())
	{
		prewitt.setState(false);
		kirsch.setState(false);
		scharr.setState(false);

		kernelState = 1;
		hasChanged = true;
	}
	else if (kernelState != 2 && kirsch.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		scharr.setState(false);

		kernelState = 2;
		hasChanged = true;
	}
	else if (kernelState != 3 && scharr.getState())
	{
		prewitt.setState(false);
		sobel.setState(false);
		kirsch.setState(false);

		kernelState = 3;
		hasChanged = true;
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

	gammaValue = -1.0;

	hasChanged = true;
}
void Gamma::apply()
{
	float tmpGamma = gamma.getValue();
	if (gammaValue != tmpGamma)
	{
		gammaValue = tmpGamma;
		hasChanged = true;
	}
	if (hasChanged) 
	{
		GPUProcessing::gamma(referenceTexture, fbo, 1024, 1024, gammaValue);
		hasChanged = false;
	}
	
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

	brightnessValue = -1;
	contrastValue = -1;

	hasChanged = true;
}
void BrightnessContrast::apply()
{
	float tmpBrightness = brightness.getValue();
	if (brightnessValue != tmpBrightness)
	{
		brightnessValue = tmpBrightness;
		hasChanged = true;
	}
	float tmpContrast = contrast.getValue();
	if (contrastValue != tmpContrast)
	{
		contrastValue = tmpContrast;
		hasChanged = true;
	}
	if (hasChanged) 
	{
		GPUProcessing::brightCont(referenceTexture, fbo, 1024, 1024, brightnessValue, contrastValue);
		hasChanged = false;
	}
	
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void Invert::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	
	inputGui.setEventEnabled(true);
	
	hasChanged = true;
}
void Invert::apply()
{
	if (hasChanged)
	{
		GPUProcessing::invert(referenceTexture, fbo, 1024, 1024);
		hasChanged = false;
	}
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

	hasChanged = true;

	hueValue = -1.0;
	saturationValue = -1.0;
	luminaceValue = -1.0;
}
void HSLCorrection::apply()
{
	float tmpHue = hue.getValue();
	if (hueValue != tmpHue)
	{
		hueValue = tmpHue;
		hasChanged = true;
	}

	float tmpSaturation = saturation.getValue();
	if (saturationValue != tmpSaturation)
	{
		saturationValue = tmpSaturation;
		hasChanged = true;
	}

	float tmpLuminace = luminance.getValue();
	if (luminaceValue != tmpLuminace)
	{
		luminaceValue = tmpLuminace;
		hasChanged = true;
	}

	if(hasChanged)
	{
		GPUProcessing::hslCorrection(referenceTexture, fbo, 1024, 1024, hue.getValue(), saturation.getValue(), luminance.getValue());
		hasChanged = false;
	}
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void Blends::setup(GuiStyle* style, GuiFont& font, ofVec4f fontColor, int width, int height)
{
	FilterModule::setup(style, font, fontColor, width, height);
	inputGui.setLabel("Modifiers");
	inputGui.addGuiButton("Darken", width, height);
	inputGui.addGuiButton("Lighten", width, height);
	inputGui.addGuiButton("Multiply", width, height);
	inputGui.addGuiButton("Burn", width, height);
	inputGui.addGuiButton("Linear Burn", width, height);
	inputGui.addGuiButton("Screen", width, height);
	inputGui.addGuiButton("Color Dodge", width, height);
	inputGui.addGuiButton("Linear Dodge", width, height);
	inputGui.addGuiButton("Difference", width, height);
	inputGui.addGuiButton("Exclusion", width, height);
	inputGui.addGuiButton("Subtract", width, height);

	inputGui.setElementState(0, true);
	inputGui.setEventEnabled(true);
	activeInput = 0;


	selectionMenu.setup(style, font, fontColor, "", height, 1);
	selectionMenu.setEventEnabled(true);

	textureSelect.setup(style, font, fontColor, "", width, 1);
	textureSelect.addGuiButton(gTarget, "Target", width, height);
	textureSelect.addGuiButton(gBlend, "Blend", width, height);

	textureSelect.setEventEnabled(true);

	hasChanged = true;
}
void Blends::apply()
{
	updateSelectionMenu();
	setInput();

	if (hasChanged)
	{
		if(target.isAllocated() && blend.isAllocated())
			GPUProcessing::blends(target, blend, fbo, 1024, 1024, activeInput);
		hasChanged = false;
	}
}
void Blends::drawSelector(int x, int y)
{
	if (activeSelect != -1)
	{
		ofVec2f pos = selectionMenu.getElementPosition(activeSelect);
		if (pos.y + textureSelect.getSize().y > ofGetWindowHeight())
		{
			textureSelect.draw(pos.x - textureSelect.getSize().x,
								ofGetWindowHeight() - textureSelect.getSize().y);
		}
		else
		{
			textureSelect.draw(pos.x - textureSelect.getSize().x, pos.y - 1);
		}
	}
	
	selectionMenu.draw(x, y);
}
void Blends::setReferencesPossibilities(vector<FilterModule*> references)
{
	this->references = references;
	TextAlignment	alignment;
	alignment.h = hCENTER;
	alignment.v = vCENTER;

	for (int i = 0; i < this->references.size() + 1; i++)
	{
		selectionMenu.addGuiButton("", selectionMenu.getWidth(), selectionMenu.getWidth(), alignment);
	}
}
void Blends::setInput()
{
	int tmp = -1;

	for (int i = 0; i < inputGui.getNumElements(); i++)
	{
		if (inputGui.getElementState(i) && activeInput != i)
		{
			tmp = i;
		}
	}
	if (tmp != -1 )
	{
		inputGui.setElementState(activeInput, false);
		activeInput = tmp;
		hasChanged = true;
	}
	
}
void Blends::updateSelectionMenu()
{
	if (activeSelect != -1)
	{
		if (gTarget.getState())
		{
			for (int i = 0; i < selectionMenu.getNumElements(); i++)
			{
				if (selectionMenu.getElementLabel(i) == "t")
				{
					selectionMenu.setElementLabel(i, "");
				}
			}
			if (activeSelect == references.size())
			{
				selectionMenu.setElementLabel(activeSelect, "t");
				target = referenceTexture;
			}
			else
			{
				selectionMenu.setElementLabel(activeSelect, "t");
				target = references[activeSelect]->getImage()->getTexture();
			}
			gTarget.setState(false);
			selectionMenu.setElementState(activeSelect, false);
			activeSelect = -1;
			hasChanged = true;
		}
		else if (gBlend.getState())
		{
			for (int i = 0; i < selectionMenu.getNumElements(); i++)
			{
				if (selectionMenu.getElementLabel(i) == "b")
				{
					selectionMenu.setElementLabel(i, "");
				}
			}
			if (activeSelect == references.size())
			{
				selectionMenu.setElementLabel(activeSelect, "b");
				blend = referenceTexture;
			}
			else
			{
				selectionMenu.setElementLabel(activeSelect, "b");
				blend = references[activeSelect]->getImage()->getTexture();
			}
			gBlend.setState(false);
			selectionMenu.setElementState(activeSelect, false);
			activeSelect = -1;
			hasChanged = true;
		}
	}

	int	tmp = -1;
	for (int i = 0; i < selectionMenu.getNumElements(); i++)
	{
		if (selectionMenu.getElementState(i))
		{
			tmp = i;
		}
	}
	if (tmp != activeSelect && activeSelect != -1)
	{
		selectionMenu.setElementState(activeSelect, false);
	}
	activeSelect = tmp;
	
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

