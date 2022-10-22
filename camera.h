#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

const float EPSILON = 1e-20;
#define IN_RANGE(a,b)   (((a>(b-EPSILON))&&(a<(b+EPSILON)))?1:0)

#define DEFAULT_FOCUS_LENGTH 1.0f
#define NEAR_PLANE 0.01f
#define FAR_PLANE 20.0f
#define VIEW_ANGLE 60.0f
#define PI glm::pi<float>()


class Camera {
public:
	float rotU, rotV, rotW;  //values used by the callback in main.cpp
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 w;
	glm::vec3 eye;
	glm::mat4 scale;
	glm::mat4 rotate;
	glm::mat4 rotate_inverse;
	glm::mat4 translate;
	glm::mat4 translate_inverse;
	glm::mat4 m_pp;
	glm::mat4 model_view_matrix;
	glm::mat4 projection_matrix;
	glm::vec3 lookVec;
	glm::vec3 upVec;


	Camera();
	~Camera();


	glm::vec3 cross_product_n(glm::vec3 first, glm::vec3 second);
	glm::vec3 cross_product(glm::vec3 first, glm::vec3 second);

	void reset();
	void orientLookAt(glm::vec3 eyePoint, glm::vec3 focusPoint, glm::vec3 upVec);
	void orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec);
	void setViewAngle(float _viewAngle);
	void setNearPlane(float _nearPlane);
	void setFarPlane(float _farPlane);
	void setScreenSize(int _screenWidth, int _screenHeight);
	void setRotUVW(float u, float v, float w);  //called by main.cpp as a part of the slider callback

	glm::mat4 getUnhingeMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getScaleMatrix();
	glm::mat4 getInverseScaleMatrix();
	glm::mat4 getModelViewMatrix();
	glm::mat4 getInverseModelViewMatrix();

	void rotateV(float degree);
	void rotateU(float degree);
	void rotateW(float degree);
	//void rotate(glm::vec3 point, glm::vec3 axis, float degree);
	//void translate(glm::vec3 v);

	glm::vec3 getEyePoint();
	glm::vec3 getLookVector();
	glm::vec3 getUpVector();
	float getViewAngle();
	float getNearPlane();
	float getFarPlane();
	int getScreenWidth();
	int getScreenHeight();

	float getFilmPlanDepth();
	float getScreenWidthRatio();


private:
	float viewAngle, filmPlanDepth;
	float nearPlane, farPlane;
	int screenWidth, screenHeight;
	float screenWidthRatio;
};
#endif

