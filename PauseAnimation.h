#pragma once
#include "Object3D.h"
#include "Animation.h"
/**
 * @brief Translates an object at a continuous rate over an interval.
 */
class PauseAnimation : public Animation {
private:
	/**
	 * @brief How much to increment the orientation by each second.
	 */
	float_t duration;

	/**
	 * @brief Advance the animation by the given time interval.
	 */
	void applyAnimation(float_t dt) override {
		object().move(glm::vec3(0, 0, 0) * duration * dt);
	}

public:
	/**
	 * @brief Constructs a animation of a constant rotation by the given total rotation
	 * angle, linearly interpolated across the given duration.
	 */
	PauseAnimation(Object3D& object, float_t duration) :
		Animation(object, duration), duration(duration) {}
};