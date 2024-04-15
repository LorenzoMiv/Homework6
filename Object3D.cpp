#include "Object3D.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics/Transform.hpp>


void Object3D::rebuildModelMatrix() {
	// Initialize the model matrix as the identity matrix.
	auto model = glm::mat4(1);

	// TODO: either initialize your own mat4 objects to represent scale/yaw/pitch/roll/translate
	// matrices and then multiply them together; 
	// OR learn to use glm::scale, glm::rotate, and glm::translate to do this.
	// Remember that the transformations must be applied in the correct order:
	//    scale, yaw, pitch, roll, translate

	auto scaleMatrix = glm::scale(model, m_scale);
	auto yawMatrix = glm::rotate(model, m_orientation[1], glm::vec3(0, 1, 0)); // Yaw around the y-axis
	auto pitchMatrix = glm::rotate(model, m_orientation[0], glm::vec3(1, 0, 0)); // Pitch around the x-axis
	auto rollMatrix = glm::rotate(model, m_orientation[2], glm::vec3(0, 0, 1)); // Roll around the z-axis
	auto translateMatrix = glm::translate(glm::mat4(1), m_position);

	//matrix multiplication: v_w = A_t * A_r * A_p * A_y * A_s * v_0

	// In the end, assign the computed model matrix to m_modelMatrix.
	m_modelMatrix = (translateMatrix * (rollMatrix * (pitchMatrix * (yawMatrix * (scaleMatrix * model)))));
}

Object3D::Object3D(std::shared_ptr<Mesh3D>&& mesh) : m_mesh(mesh), m_position(), m_orientation(), m_scale(1.0) {
	// After every change to m_position, m_orientation, or m_scale, we must rebuild the model matrix.


	rebuildModelMatrix();
}

const glm::vec3& Object3D::getPosition() const {
	return m_position;
}

const glm::vec3& Object3D::getOrientation() const {
	return m_orientation;
}

const glm::vec3& Object3D::getScale() const {
	return m_scale;
}

void Object3D::setPosition(const glm::vec3& position) {
	m_position = position;
	rebuildModelMatrix();
}

void Object3D::setOrientation(const glm::vec3& orientation) {
	m_position = orientation;
	rebuildModelMatrix();
}

void Object3D::setScale(const glm::vec3& scale) {
	m_position = scale;
	rebuildModelMatrix();
}

void Object3D::move(const glm::vec3& offset) {
	m_position = m_position + offset;
	rebuildModelMatrix();
}

void Object3D::rotate(const glm::vec3& rotation) {
	m_orientation = m_orientation + rotation;
	rebuildModelMatrix();
}

void Object3D::grow(const glm::vec3& growth) {
	m_scale = m_scale * growth;
	rebuildModelMatrix();
}
/*
void Object3D::translate(const glm::vec3& translate) {
	m_position.x = m_position.x + translate.x;
	m_position.y = m_position.y + translate.y;
	m_position.z = m_position.z + translate.z;
	rebuildModelMatrix();
}

void Object3D::bezierTranslation(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float_t t) {
	m_position.x = ;
	m_position.y = pow((1 - t), 3) * p0.y + 3 * pow((1 - t), 2) * t * p1.y + 3 * (1 - t) * pow(t, 2) * p2.y + pow(t, 3) * p3.y;
	m_position.z = pow((1 - t), 3) * p0.z + 3 * pow((1 - t), 2) * t * p1.z + 3 * (1 - t) * pow(t, 2) * p2.z + pow(t, 3) * p3.z;
	rebuildModelMatrix();
}
*/


void Object3D::render(sf::RenderWindow& window, const glm::mat4& view, const glm::mat4& proj) const {
	m_mesh->render(window, m_modelMatrix, view, proj);
}

