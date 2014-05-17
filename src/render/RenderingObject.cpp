#include "RenderingObject.h"

void RenderingObject::init(ShaderProgram *shaderProgramm) {
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->VBO_data.size(), &this->VBO_data[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Helper::vec3s) * this->IBO_data.size(), &this->IBO_data[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->CBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->CBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->CBO_data.size(), &this->CBO_data[0], GL_STATIC_DRAW);

	// init uniform variables
	this->modelUniform = shaderProgramm->getUniformLocation("ModelMatrix");
	
	/* initialize temp matrices */
	this->translationMatrix = glm::mat4(1.0f);
	this->scaleMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::mat4(1.0f);
	this->renderingModelMatrix = glm::mat4(1.0f);
	
	// set rendering type
	this->renderingMode = GL_TRIANGLES;
	//set no parent
	this->parent = NULL;
}

void RenderingObject::setParent(RenderingObject *parent){
	this->parent = parent;
}

RenderingObject *RenderingObject::getParent(void){
	return this->parent;
}

void RenderingObject::calculateModelMatrix(void) {
	glm::mat4 tmpScaleMatrix;
	
	tmpScaleMatrix = this->scaleMatrix;
	
	// if we have a parent, recalculate our scale matrix
	if(this->parent != NULL) {
		glm::mat4 parentScalingMatrix = this->parent->getScaleMatrix();
		tmpScaleMatrix = this->scaleMatrix * glm::inverse(parentScalingMatrix);
	}
	
	this->modelMatrix = this->translationMatrix * this->rotationMatrix * tmpScaleMatrix;
	this->renderingModelMatrix = this->modelMatrix;
}

glm::mat4 RenderingObject::getRenderingMatrix(void) {
	return this->renderingModelMatrix;
}

glm::mat4 RenderingObject::getRotationMatrix(void) {
	return this->rotationMatrix;
}

glm::mat4 RenderingObject::getScaleMatrix(void) {
	return this->scaleMatrix;
}

glm::mat4 RenderingObject::getTranslationMatrix(void) {
	return this->translationMatrix;
}

void RenderingObject::scale(float scale) {
	this->scaleMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(scale));
}

void RenderingObject::rotateX(float angle) {
	angle = angle * M_PI / 180.0;
	this->rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f)) * this->rotationMatrix;
}

void RenderingObject::rotateY(float angle) {
	angle = angle * M_PI / 180.0;
	this->rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * this->rotationMatrix;
}

void RenderingObject::rotateZ(float angle) {
	angle = angle * M_PI / 180.0;
	this->rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * this->rotationMatrix;
}

void RenderingObject::rotate(float angle, glm::vec3 axes) {
	angle = angle * M_PI / 180.0;
	this->rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axes) * this->rotationMatrix;
}

void RenderingObject::translateX(float x) {
	this->translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, 0.0f)) * this->translationMatrix;
}

void RenderingObject::translateY(float y) {
	this->translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, y, 0.0f)) * this->translationMatrix;
}

void RenderingObject::translateZ(float z) {
	this->translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, z)) * this->translationMatrix;
}

void RenderingObject::translate(float x, float y, float z) {
	this->translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * this->translationMatrix;
}

void RenderingObject::render(void) {
	// set buffers
	glEnableVertexAttribArray(S_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glVertexAttribPointer(S_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(S_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, this->CBO);
	glVertexAttribPointer(S_COLOR, 3, GL_FLOAT,GL_FALSE, 0, 0);   

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
	GLint size; 
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	
	// calculate the model matrix
	this->calculateModelMatrix();
	
	// if we have a parent, recalculate our model matrix
	if(this->parent != NULL) {
		this->renderingModelMatrix = this->modelMatrix * this->parent->getRenderingMatrix();
	}

	// set uniform variables
	glUniformMatrix4fv(this->modelUniform, 1, GL_TRUE, glm::value_ptr(this->renderingModelMatrix));

	// draw
	glDrawElements(this->renderingMode, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	// disable attributes
	glDisableVertexAttribArray(S_POSITION);
	glDisableVertexAttribArray(S_COLOR);
}
