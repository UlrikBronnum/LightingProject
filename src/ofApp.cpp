#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	

	ofDisableArbTex();

	diffuse.load("res/textures/diffuse.jpg");
	diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	diffuse.getTexture().enableMipmap();
	diffuse.getTexture().generateMipmap();
	diffuse.resize(512, 512);


	text.setup("res/fonts/MicroExtendFLF-Bold.ttf", 12);
	style.setup(ofVec4f(0.3, 0.3, 0.3, 1.0), ofVec4f(0, 0, 1.0, -1.0), SLOPED);

	menu.setup(&style, text, ofVec4f(1.0, 1.0, 1.0, 1.0), "", 256, 1);
	menu.addGuiButton(save, "Save", 256, 24);
	menu.addGuiButton(textureEditor, "Editor", 256, 24);
	menu.addGuiButton(preview3d, "3D Preview", 256, 24);
	fill.setup(&style, text, ofVec4f(1.0, 1.0, 1.0, 1.0), "", 2048, 24);
	fill.setColorStates(ofVec3f(1.0, 1.0, 1.0));
	menu.addElement(fill);

	menu.setElementPosition(0, 0, 0);
	menu.setElementPosition(256, 0, 1);
	menu.setElementPosition(512, 0, 2);
	menu.setElementPosition(768, 0, 3);
	
	menu.setEventEnabled(true);
	textureEditor.setState(true);
	mainState = 1;

	ps.setup(&style, text, ofVec4f(1.0, 1.0, 1.0, 1.0), 256, 24);

	editor.setup(&style, text, ofVec4f(1.0, 1.0, 1.0, 1.0), 256, 36);
	editor.setBaseImage(diffuse);
}

//--------------------------------------------------------------
void ofApp::update()
{
	if ( save.getState() && mainState != 0 )
	{
		textureEditor.setState(false);
		preview3d.setState(false);
		mainState = 0;
	}
	else
	if (textureEditor.getState() && mainState != 1)
	{
		save.setState(false);
		preview3d.setState(false);
		mainState = 1;
	}
	else
	if (preview3d.getState() && mainState != 2)
	{
		save.setState(false);
		textureEditor.setState(false);
		mainState = 2;
	}

	if (mainState == 2)
	{
		ps.update();
	}
	


	editor.update();



/*	
	minMaxFilter.apply();

	invertFilter.apply();

	blurFilter.apply();
	
	hslFilter.setReference(diffuse);
	hslFilter.apply();

	

	GPUProcessing::heightmap(diffuse.getTexture(), blurFilter.getImage()->getTexture(), heightFbo, 1024, 1024, strength.getValue(), edgeStrength.getValue(), ofVec3f(0.33, 0.33, 0.33));
	edgeFilter.setReference(diffuse);
	edgeFilter.apply();

	gammaFilter.setReference(heightFbo.getTexture());
	gammaFilter.apply();

	normalmapFilter.setReference(*gammaFilter.getImage());
	normalmapFilter.apply();
	

	
	if (invertButton.getState())
	{
		GPUProcessing::invert(heightFbo.getTexture(), invertFbo, 1024, 1024);
		GPUProcessing::normalmap(invertFbo.getTexture(), normalFbo, 1024, 1024, sobel_horizontal, sobel_vertical, roughness.getValue(), ofVec3f(1.0, 0.0, 0.0));
	}
	else {
		GPUProcessing::normalmap(heightFbo.getTexture(), normalFbo, 1024, 1024, sobel_horizontal, sobel_vertical, roughness.getValue(), ofVec3f(1.0, 0.0, 0.0));
	}

	filterHeightmap();
	GPUProcessing::normalmap(heightFbo.getTexture(), normalFbo, 1024, 1024, sobel_horizontal, sobel_vertical, 1.0f, ofVec3f(1.0, 0.0, 0.0));

	normalFbo.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	normalFbo.getTexture().enableMipmap();
	normalFbo.getTexture().generateMipmap();
*/
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofSetBackgroundColor(255*0.1);

	editor.draw(textureEditor.getValue());
	if (mainState == 2)
	{
		ofTexture a = *editor.getAlbido();
		ofTexture n = *editor.getNormal();
		ofTexture s = *editor.getSpecular();
		ofTexture h = *editor.getHeight();

		if(a.isAllocated() && n.isAllocated() && s.isAllocated() && h.isAllocated())
			ps.draw(a,n,s,h);

		ps.drawControls(0, 256);
	}
	menu.draw(0, 0);
}
//--------------------------------------------------------------
void ofApp::filterHeightmap()
{
	/*
	ofFbo fbo1;
	ofFbo fbo2;
	ofFbo fbo3;

	Kernel kernel;
	kernel.setSize(5);//
	kernel.gaussianKernel(sigma.getValue());//

	GPUProcessing::invert(blurFilter.getImage()->getTexture(), fbo1, 1024, 1024, ofVec3f(1, 1, 1));
	//GPUProcessing::blur(fbo1.getTexture(), fbo2, 1024, 1024, kernel);
	//GPUProcessing::edge(diffuse.getTexture(), edgeFbo, 1024, 1024, scharr_horizontal, scharr_vertical, null_kernel, DIFFERENCE_VALUE);
	
	GPUProcessing::heightmap(diffuse.getTexture(), fbo1.getTexture(), heightFbo, 1024, 1024, strength.getValue(), edgeStrength.getValue(), ofVec3f(0.33, 0.33, 0.33));
	kernel.setSize(radius.getValue() * 2 + 1);//
	kernel.gaussianKernel(sigma.getValue());//
	//GPUProcessing::blur(fbo3.getTexture(), heightFbo, 1024, 1024, kernel);
	*/
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 13)
		ps.reloadLightShader();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
	ps.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) 
{
	ps.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	diffuse.load(dragInfo.files[0]);
	diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	diffuse.getTexture().enableMipmap();
	diffuse.getTexture().generateMipmap();
	diffuse.resize(1024, 1024);

	editor.setBaseImage(diffuse);

}
