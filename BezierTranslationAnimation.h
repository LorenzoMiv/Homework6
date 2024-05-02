#pragma once
#include "glm/glm.hpp"
#include "Animation.h"

class BezierTranslationAnimation : public Animation {
private:
	glm::vec3 cp0;
	glm::vec3 cp1;
	glm::vec3 cp2;
	glm::vec3 cp3;

	void applyAnimation(float_t dt) override {
		auto t = currentTime() / duration();
		glm::vec3 interpolatedPoint;

		interpolatedPoint.x = pow((1 - t), 3) * cp0.x + 3 * pow((1 - t), 2) * t * cp1.x + 3 * (1 - t) * pow(t, 2) * cp2.x + pow(t, 3) * cp3.x;
		interpolatedPoint.y = pow((1 - t), 3) * cp0.y + 3 * pow((1 - t), 2) * t * cp1.y + 3 * (1 - t) * pow(t, 2) * cp2.y + pow(t, 3) * cp3.y;
		interpolatedPoint.z = pow((1 - t), 3) * cp0.z + 3 * pow((1 - t), 2) * t * cp1.z + 3 * (1 - t) * pow(t, 2) * cp2.z + pow(t, 3) * cp3.z;

		object().move(interpolatedPoint * dt);
	}

public:
	BezierTranslationAnimation(Object3D& object, float_t duration, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) :
		Animation(object, duration), cp0(p0/duration), cp1(p1/duration), cp2(p2/duration), cp3(p3/duration){}
};