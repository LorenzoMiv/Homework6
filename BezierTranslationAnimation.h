#pragma once
#include "Object3D.h"
#include "Animation.h"
/**
 * @brief Translates an object at a continuous rate over an interval.
 */

class BezierTranslationAnimation : public Animation {
private:
	/**
	 * @brief How much to increment the move by each second.
	 */
	//glm::vec3 m_perSecond;
	glm::vec3 cp0;
	glm::vec3 cp1;
	glm::vec3 cp2;
	glm::vec3 cp3;

	/**
	 * @brief Advance the animation by the given time interval.
	 */
	void applyAnimation(float_t dt) override {
		auto t = currentTime() / duration();
		glm::vec3 interpolatedpoint;
		interpolatedpoint.x = pow((1 - t), 3) * cp0.x + 3 * pow((1 - t), 2) * t * cp1.x + 3 * (1 - t) * pow(t, 2) * cp2.x + pow(t, 3) * cp3.x;
		interpolatedpoint.y = pow((1 - t), 3) * cp0.y + 3 * pow((1 - t), 2) * t * cp1.y + 3 * (1 - t) * pow(t, 2) * cp2.y + pow(t, 3) * cp3.y;
		interpolatedpoint.z = pow((1 - t), 3) * cp0.z + 3 * pow((1 - t), 2) * t * cp1.z + 3 * (1 - t) * pow(t, 2) * cp2.z + pow(t, 3) * cp3.z;
		object().move(interpolatedpoint * dt);
	}

public:
	/**
	 * @brief Constructs a animation of a constant rotation by the given total rotation
	 * angle, linearly interpolated across the given duration.
	 */
	BezierTranslationAnimation(Object3D& object, float_t duration, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) :
		Animation(object, duration), cp0(p0/duration), cp1(p1/duration), cp2(p2/duration), cp3(p3/duration){}
};