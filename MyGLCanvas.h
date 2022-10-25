#pragma once

#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

#include <FL/gl.h>
#include <FL/glut.h>
#include <FL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <iostream>
#include <list>

#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Pyramid.h"

#include "Camera.h"
#include "scene/SceneParser.h"


class MyGLCanvas : public Fl_Gl_Window {
public:
	glm::vec3 rotVec;
	glm::vec3 eyePosition;

	int wireframe;
	int smooth;
	int fill;
	int normal;
	int segmentsX, segmentsY;

	OBJ_TYPE objType;
	Cube* cube;
	Cylinder* cylinder;
	Cone* cone;
	Sphere* sphere;
	Pyramid* pyramid;
	Shape* shape;

	Camera* camera;
	SceneParser* parser;

	MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
	~MyGLCanvas();
	void renderShape(OBJ_TYPE type);
	void setSegments();
	void loadSceneFile(const char* filenamePath);
	void setShape(OBJ_TYPE type);
	void resetScene();

private:
	void draw();
	void drawScene();
	void drawObject(OBJ_TYPE type);
	void drawAxis();
	void traverse1(SceneNode* root, list<ScenePrimitive*>& primitives, vector<SceneTransformation*>& scenetransformations, vector<pair<ScenePrimitive*, vector<SceneTransformation*>>>& my_scene_vals);
	int handle(int);
	void resize(int x, int y, int w, int h);
	void updateCamera(int width, int height);
	void setLight(const SceneLightData &light);
	void applyMaterial(const SceneMaterial &material);
};

#endif // !MYGLCANVAS_H