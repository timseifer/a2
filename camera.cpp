#include "Camera.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() {
	reset();
}

Camera::~Camera() {
}

glm::vec3 Camera::cross_product_n(glm::vec3 first, glm::vec3 second) {
	// find vector between x2 and x1
	float v1x = first[0];
	float v1y = first[1];
	float v1z = first[2];

	// find vector between x3 and x2
	float v2x = second[0];
	float v2y = second[1];
	float v2z = second[2];

	// cross product v1xv2

	float cx = v1y * v2z - v1z * v2y;
	float cy = v1z * v2x - v1x * v2z;
	float cz = v1x * v2y - v1y * v2x;

	// normalize

	float length = sqrt(cx * cx + cy * cy + cz * cz);
	if (length != 0) {
		cx = cx / length;
		cy = cy / length;
		cz = cz / length;
	}
	return glm::vec3(cx, cy, cz);
}

glm::vec3 Camera::cross_product(glm::vec3 first, glm::vec3 second) {
	// find vector between x2 and x1
	float v1x = first[0];
	float v1y = first[1];
	float v1z = first[2];

	// find vector between x3 and x2
	float v2x = second[0];
	float v2y = second[1];
	float v2z = second[2];

	// cross product v1xv2

	float cx = v1y * v2z - v1z * v2y;
	float cy = v1z * v2x - v1x * v2z;
	float cz = v1x * v2y - v1y * v2x;

	return glm::vec3(cx, cy, cz);
}


void Camera::reset() {
	setViewAngle(VIEW_ANGLE);
	setNearPlane(NEAR_PLANE);
	setFarPlane(FAR_PLANE);
	screenWidth = screenHeight = 200;
	screenWidthRatio = 1.0f;
	rotU = rotV = rotW = 0;
	orientLookAt(glm::vec3(0.0f, 0.0f, DEFAULT_FOCUS_LENGTH), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


}

//called by main.cpp as a part of the slider callback for controlling rotation
// the reason for computing the diff is to make sure that we are only incrementally rotating the camera
void Camera::setRotUVW(float u, float v, float w) {
	float diffU = u - rotU;
	float diffV = v - rotV;
	float diffW = w - rotW;
	rotateU(diffU);
	rotateV(diffV);
	rotateW(diffW);
	rotU = u;
	rotV = v;
	rotW = w;
}


void Camera::orientLookAt(glm::vec3 eyePoint, glm::vec3 lookatPoint, glm::vec3 upVec) {
	orientLookVec(eyePoint, lookatPoint, upVec);
}



void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) {
	this->eye = eyePoint;
	//wuv maps to fsu
	this->w = -normalize(eyePoint);
	this->u = normalize(cross(upVec, w));
	this->v = cross(this->w, this->u);
	this->lookVec = lookVec;
	this->upVec = upVec;
	float r_i[16] = {
		this->u[0], this->u[1], this->u[2], 0,
		this->v[0], this->v[1], this->v[2], 0,
		this->w[0], this->w[1], this->w[2], 0,
		0, 0, 0, 1
	};

	float r[16] = {
	this->u[0], this->v[0], this->w[0], 0,
	this->u[1], this->v[1], this->w[1], 0,
	this->u[2], this->v[2], this->w[2], 0,
	0, 0, 0, 1
	};
	this->rotate_inverse = glm::make_mat4(r_i);
	this->rotate = glm::make_mat4(r);

	float t_i[16] = {
		1, 0, 0, -1.0f * this->eye.x,
		0, 1, 0, -1.0f * this->eye.y,
		0, 0, 1, -1.0f * this->eye.z,
		0, 0, 0, 1
	};

	float t[16] = {
	1, 0, 0, this->eye.x,
	0, 1, 0, this->eye.y,
	0, 0, 1, this->eye.z,
	0, 0, 0, 1
	};
	this->translate_inverse = glm::make_mat4(t_i);
	this->translate = glm::make_mat4(t);

	float theta_w = glm::radians(this->viewAngle);
	float h2 = tan(theta_w / 2.0f) * farPlane * this->getScreenHeight() / this->getScreenWidth();
	float theta_h = atan(h2 / this->farPlane) * 2.0f;
	// theta_h needs checking
	float s[16] = {
		1.0f / (tan(theta_w / 2.0f) * this->farPlane), 0, 0, 0,
		0, 1.0f / (tan(theta_h / 2.0f) * this->farPlane), 0, 0,
		0, 0, 1.0f / this->farPlane, 0,
		0, 0, 0, 1,
	};

	this->scale = glm::make_mat4(s);


	float c = -(this->nearPlane / this->farPlane);
	float mpp[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1.0f / (c + 1.0f), c / (c + 1.0f),
		0, 0, -1, 0
	};
	this->m_pp = glm::make_mat4(mpp);
	this->model_view_matrix = this->translate_inverse * this->rotate_inverse;
	glm::mat4 correct_matrix_model = glm::lookAt(this->eye, this->lookVec, this->upVec);
	//this->model_view_matrix = correct_matrix_model;
	this->projection_matrix = this->scale * this->m_pp;
	glm::mat4 correct_projection_model = glm::perspective(glm::radians(this->viewAngle), this->getScreenWidthRatio(), this->nearPlane, this->farPlane);
	this->projection_matrix = correct_projection_model;
	//this->projection_matrix = correct_projection_model;
	float end = 0;


}

glm::mat4 Camera::getScaleMatrix() {
	return this->scale;
}

glm::mat4 Camera::getInverseScaleMatrix() {
	return glm::inverse(this->scale);
}

glm::mat4 Camera::getUnhingeMatrix() {
	return this->m_pp;
}


glm::mat4 Camera::getProjectionMatrix() {
	return this->projection_matrix;
}

glm::mat4 Camera::getInverseModelViewMatrix() {
	return this->translate * this->rotate;
}


void Camera::setViewAngle (float _viewAngle) {
	this->viewAngle = _viewAngle;
}

void Camera::setNearPlane (float _nearPlane) {
	this->nearPlane = _nearPlane;
}

void Camera::setFarPlane (float _farPlane) {
	this->farPlane = _farPlane;
}

void Camera::setScreenSize (int _screenWidth, int _screenHeight) {
	this->screenWidth = _screenWidth;
	this->screenHeight = _screenHeight;
	this->screenWidthRatio = _screenWidth / _screenHeight;
}

glm::mat4 Camera::getModelViewMatrix() {
	return this->model_view_matrix;
}


void Camera::rotateV(float degrees) {
	glm::mat4 mymatrix = glm::rotate(glm::mat4(1.0), -degrees, glm::vec3(0, 0, 1));

	this->u = mymatrix * glm::vec4(this->u, 1);
	this->v = mymatrix * glm::vec4(this->v, 1);
	this->w = mymatrix * glm::vec4(this->w, 1);

}

void Camera::rotateU(float degrees) {
}

void Camera::rotateW(float degrees) {
}

//void Camera::translate(glm::vec3 v) {
//}
//
//void Camera::rotate(glm::vec3 point, glm::vec3 axis, float degrees) {
//}


glm::vec3 Camera::getEyePoint() {
	return this->eye;
}

glm::vec3 Camera::getLookVector() {
	return this->lookVec;
}

glm::vec3 Camera::getUpVector() {
	return this->upVec;
}

float Camera::getViewAngle() {
	return viewAngle;
}

float Camera::getNearPlane() {
	return nearPlane;
}

float Camera::getFarPlane() {
	return farPlane;
}

int Camera::getScreenWidth() {
	return screenWidth;
}

int Camera::getScreenHeight() {
	return screenHeight;
}

float Camera::getScreenWidthRatio() {
	return this->screenWidth/this->screenHeight;
}
