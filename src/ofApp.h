#pragma once

#include "ofMain.h"

#include "GraphicMesh.h"
#include "Gui.h"
#include "ImageProcessing.h"
#include "MoveCam.h"

#include "LightProp.h"

#include "PreviewScene.h"

#include "FilterModule.h"
#include "ImageEditor.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void filterHeightmap();


		ofImage			diffuse;

		//	Controls
		GuiFont			text;
		GuiStyle		style;

		Gui				menu;
		GuiButton		save;
		GuiButton		textureEditor;
		GuiButton		preview3d;
		GuiBaseElement	fill;

		int				mainState;
/*		

		// Filters
		ofFbo			heightFbo;
		ofFbo			normalFbo;
		ofFbo			invertFbo;
		ofFbo			edgeFbo;
*/
		PreviewScene	ps;

	
		ImageEditor		editor;
};
