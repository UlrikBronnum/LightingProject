#include "GraphicMesh.h"


GraphicMesh::GraphicMesh()
{
	mesh.enableColors();
	mesh.enableTextures();
	mesh.enableIndices();
	mesh.enableNormals();
	calculateNormals = true;
}


GraphicMesh::~GraphicMesh()
{
}

void GraphicMesh::draw() 
{
	mesh.draw();
}
void GraphicMesh::drawWireframe()
{
	mesh.drawWireframe();
}
void GraphicMesh::drawNormals() 
{
	for (int i = 0; i < mesh.getVertices().size(); i++) {
		ofSetColor(0,0,255);
		ofDrawLine(mesh.getVertices()[i], mesh.getVertices()[i] + mesh.getNormals()[i] * 100);
		ofSetColor(0,255,0);
		ofDrawLine(mesh.getVertices()[i], mesh.getVertices()[i] + tangents[i] * 100);
	}
}
void GraphicMesh::createMesh(ofMesh& m, bool calculateNormals) 
{
	clearMesh();
	mesh = m;
	this->calculateNormals = calculateNormals;

	if (mesh.getVertices().size() != mesh.getNormals().size() && calculateNormals) {
		mesh.clearNormals();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			mesh.addNormal(ofPoint(0, 0, 0));
		}
	}
	if (mesh.getVertices().size() != tangents.size()) {
		tangents.clear();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			tangents.push_back(ofVec3f(0, 0, 0));
		}
	}
}
void GraphicMesh::createQuad(int w, int h, ofVec2f texCoord1, ofVec2f texCoord2, ofVec2f texCoord3, ofVec2f texCoord4)
{		
	clearMesh();

	mesh = ofMesh::plane(w, h, 2, 2, OF_PRIMITIVE_TRIANGLES);

	mesh.getTexCoords()[0] = texCoord1;
	mesh.getTexCoords()[1] = texCoord2;
	mesh.getTexCoords()[2] = texCoord3;
	mesh.getTexCoords()[3] = texCoord4;

	if (mesh.getVertices().size() != mesh.getNormals().size()) {
		mesh.clearNormals();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			mesh.addNormal(ofPoint(0, 0, 0));
		}
	}
	if (mesh.getVertices().size() != tangents.size()) {
		tangents.clear();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			tangents.push_back(ofVec3f(0, 0, 0));
		}
	}
}
void GraphicMesh::createMeshFromImage(float* heightmap, int w, int h, float meshWidth, float meshHeight, float maxHeight)
{
	clearMesh();

	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.enableIndices();
	mesh.enableNormals();

	float xMultiplier = meshWidth / (float)w;
	float yMultiplier = meshHeight / (float)h;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			mesh.addVertex(
				ofPoint(x * xMultiplier,
						y * yMultiplier,
						maxHeight * heightmap[x + y * w])
			);

			mesh.addColor(ofFloatColor(1, 1, 1));  // placeholder for colour data, we'll get this from the camera
			mesh.addTexCoord(ofVec2f(float(x) / float(w), float(y) / float(h)));
		}
	}
	for (int y = 0; y < h - 1; y++) {
		for (int x = 0; x < w - 1; x++) {
			mesh.addIndex(x + y*w);					// 0
			mesh.addIndex((x + 1) + y*w);			// 1
			mesh.addIndex(x + (y + 1)*w);			// 10

			mesh.addIndex((x + 1) + y*w);			// 1
			mesh.addIndex((x + 1) + (y + 1)*w);		// 11
			mesh.addIndex(x + (y + 1)*w);			// 10
		}
	}
	if (mesh.getVertices().size() != mesh.getNormals().size()) {
		mesh.clearNormals();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			mesh.addNormal(ofPoint(0, 0, 0));
		}
	}
	if (mesh.getVertices().size() != tangents.size()) {
		tangents.clear();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			tangents.push_back(ofVec3f(0, 0, 0));
		}
	}
}
void GraphicMesh::createMeshFromImageSize(int w, int h, float meshWidth, float meshHeight)
{
	clearMesh();
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.enableIndices();
	mesh.enableNormals();

	float xMultiplier = meshWidth / (float)w;
	float yMultiplier = meshHeight / (float)h;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			mesh.addVertex(
				ofPoint(x * xMultiplier,
						y * yMultiplier,
						0.0f)
			);

			mesh.addColor(ofFloatColor(1, 1, 1));  // placeholder for colour data, we'll get this from the camera
			mesh.addTexCoord(ofVec2f(float(x) / float(w), float(y) / float(h)));
		}
	}
	for (int y = 0; y < h - 1; y++) {
		for (int x = 0; x < w - 1; x++) {
			mesh.addIndex(x + y*w);					// 0
			mesh.addIndex((x + 1) + y*w);			// 1
			mesh.addIndex(x + (y + 1)*w);			// 10

			mesh.addIndex((x + 1) + y*w);			// 1
			mesh.addIndex((x + 1) + (y + 1)*w);		// 11
			mesh.addIndex(x + (y + 1)*w);			// 10
		}
	}
	if (mesh.getVertices().size() != mesh.getNormals().size()) {
		mesh.clearNormals();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			mesh.addNormal(ofPoint(0, 0, 0));
		}
	}
	if (mesh.getVertices().size() != tangents.size()) {
		tangents.clear();
		for (int i = 0; i < mesh.getVertices().size(); i++) {
			tangents.push_back(ofVec3f(0, 0, 0));
		}
	}
	calcNormals();
	mesh.getVbo().setMesh(mesh, GL_DYNAMIC_DRAW);
}
void GraphicMesh::setMeshHeightsFromImage(float* heightmap, int w, int h, float maxHeight)
{
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			ofVec3f p = mesh.getVertices()[y * w + x];
			p.z = maxHeight * heightmap[y * w + x];
			mesh.getVertices()[y * w + x] = p;

			mesh.getColors()[y * w + x] = ofFloatColor(1,1,1); // placeholder for colour data, we'll get this from the camera

		}
	}
}

float GraphicMesh::getGridHeight(int x, int y) 
{
	return mesh.getVertices()[y * width + x].z;
}
void GraphicMesh::resizeQuad(int w, int h)
{
	createQuad(w, h);
}

void GraphicMesh::loadAttributes(ofShader& shader)
{
	if(calculateNormals)
		calcNormals();
	calcTangents();

	float* tmp = new float[tangents.size() * 3];

	for (int i = 0; i < tangents.size(); i++) {
		tmp[i * 3 + 0] = tangents[i].x;
		tmp[i * 3 + 1] = tangents[i].y;
		tmp[i * 3 + 2] = tangents[i].z;
	}

	mesh.getVbo().setAttributeData(shader.getAttributeLocation("tangent"), &tmp[0], 3, tangents.size(), GL_DYNAMIC_DRAW, sizeof(ofVec3f));

	delete[]tmp;
}
void GraphicMesh::calcNormals()
{
	for (int i = 0; i < mesh.getIndices().size(); i += 3) 
	{
		const int i0 = mesh.getIndices()[i];
		const int i1 = mesh.getIndices()[i + 1];
		const int i2 = mesh.getIndices()[i + 2];

		ofVec3f e1 = mesh.getVertices()[i1] - mesh.getVertices()[i0];
		ofVec3f e2 = mesh.getVertices()[i2] - mesh.getVertices()[i0];
		ofVec3f no = e1.cross(e2).normalized();

		// depending on your clockwise / winding order, you might want to reverse the e2 / e1 above if your normals are flipped. 

		mesh.getNormals()[i0] = no;
		mesh.getNormals()[i1] = no;
		mesh.getNormals()[i2] = no;
	}

	for (int i = 0; i < mesh.getNormals().size(); i ++)
	{
		mesh.getNormals()[i] = mesh.getNormals()[i].normalized();
	}
}
void GraphicMesh::calcTangents() {

	for (int i = 0; i < mesh.getIndices().size(); i += 3) {
		
		const int i0 = mesh.getIndices()[i];
		const int i1 = mesh.getIndices()[i + 1];
		const int i2 = mesh.getIndices()[i + 2];

		ofVec3f vertex0 = mesh.getVertices()[i0];
		ofVec3f vertex1 = mesh.getVertices()[i1];
		ofVec3f vertex2 = mesh.getVertices()[i2];

		ofVec3f edge1 = vertex1 - vertex0;
		ofVec3f edge2 = vertex2 - vertex0;

		float deltaU1 = mesh.getTexCoords()[i1].x - mesh.getTexCoords()[i0].x;
		float deltaV1 = mesh.getTexCoords()[i1].y - mesh.getTexCoords()[i0].y;
		float deltaU2 = mesh.getTexCoords()[i2].x - mesh.getTexCoords()[i0].x;
		float deltaV2 = mesh.getTexCoords()[i2].y - mesh.getTexCoords()[i0].y;

		float dividend = (deltaU1*deltaV2 - deltaU2*deltaV1);
		float f = (dividend == 0) ? 0.0f : 1.0f / dividend;

		ofVec3f tangent = ofVec3f(
			f * (deltaV2 * edge1.x - deltaV1 * edge2.x),
			f * (deltaV2 * edge1.y - deltaV1 * edge2.y),
			f * (deltaV2 * edge1.z - deltaV1 * edge2.z));


		tangents[i0] = tangent;
		tangents[i1] = tangent;
		tangents[i2] = tangent;
	}

	for (int i = 0; i < tangents.size(); i++) {
		tangents[i] = tangents[i].normalized();

	}
		
}
void GraphicMesh::save() 
{
	mesh.save("model.ply");
}
/*
//--------------------------------------------------------------
void GraphicMesh::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	mesh.addVertex(a);
	mesh.addVertex(b);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void GraphicMesh::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
void GraphicMesh::addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c) {
	mesh.addTexCoord(a);
	mesh.addTexCoord(b);
	mesh.addTexCoord(c);
}

//--------------------------------------------------------------
void GraphicMesh::addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d) {
	addTexCoords(mesh, a, b, c);
	addTexCoords(mesh, a, c, d);
}
*/