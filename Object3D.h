
#pragma once
#include <memory>
#include <glm/ext.hpp>
#include "Mesh3D.h"

/**
 * @brief Represents an object placed in a 3D scene.
*/
class Object3D {
private:
	// The object's mesh.
	std::shared_ptr<Mesh3D> m_mesh;

	// The object's position, orientation, and scale in world space.
	glm::vec3 m_position;
	glm::vec3 m_orientation;
	glm::vec3 m_scale;
	glm::vec3 curr_velocity;
	glm::vec3 curr_acceleration;
	glm::vec3 rot_velocity;
	glm::vec3 rot_acceleration;

	// The object's cached local->world transformation matrix.
	glm::mat4 m_modelMatrix;

	// Recomputes the local->world transformation matrix.
	void rebuildModelMatrix();

public:
	// No default constructor; you must have a mesh to initialize an object.
	Object3D() = delete;

	/**
	 * @brief Constructs an Object3D at the origin of world space using the given mesh.
	*/
	Object3D(std::shared_ptr<Mesh3D>&& mesh);

	// Simple accessors.
	const glm::vec3& getPosition() const;
	const glm::vec3& getOrientation() const;
	const glm::vec3& getScale() const;
	const glm::vec3& getVelocity() const;
	const glm::vec3& getAcceleration() const;
	const glm::vec3& getRotVelocity() const;
	const glm::vec3& getRotAcceleration() const;

	// Simple mutators. These functions *override* the current position, rotation, or scale.
	void setPosition(const glm::vec3& position);

	/**
	 * @brief Sets the object's orientation angles.
	 * @param orientation pitch, yaw, and roll angles in radians.
	*/
	void setOrientation(const glm::vec3& orientation);
	void setScale(const glm::vec3& scale);
	void setVelocity(const glm::vec3& velocity);
	void setAcceleration(const glm::vec3& acceleration);
	void setRotAcceleration(const glm::vec3& rotAcceleration);
	void setRotVelocity(const glm::vec3& rotVelocity);
	// Transformations. These functions *incrementally update* the current position, rotation, or scale.
	void move(const glm::vec3& offset);
	
	/**
	 * @brief Rotates the object's current orientation angles.
	 * @param rotation pitch, yaw, and roll angles in radians, to apply to the object's current orientation.
	*/
	void rotate(const glm::vec3& rotation);

	void grow(const glm::vec3& growth);

	/**
	 * @brief Renders the object to a RenderWindow, using the given view and projection matrices.
	 * @brief Renders the object to a RenderWindow, using the given view and projection matrices.
	*/
	void render(sf::RenderWindow& window, const glm::mat4& view, const glm::mat4& proj) const;

	/*
	* @brief Moves the oobject by its velocity over the given time interval dt
	*/
	void tick(float_t dt);
};