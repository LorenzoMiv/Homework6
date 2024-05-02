#pragma once
#include "Object3D.h"
#include "Animation.h"

class PauseAnimation : public Animation {
private:
	float_t duration;

	void applyAnimation(float_t dt) override {
		object().move(glm::vec3(0) * duration * dt);
	}

public:
	PauseAnimation(Object3D& object, float_t duration) :
		Animation(object, duration), duration(duration) {}
};
