#pragma once

#include "../Utils/HMath.h"

struct MobBodyRotationComponent {
public:
	float bodyRot;
	float prevBodyRot;
};

struct ActorRotationComponent {
public:
	Vec2 rot;
	Vec2 rotPrev;
};

struct ActorHeadRotationComponent {
public:
	Vec2 rot;
};

struct RuntimeIDComponent {
public:
	int64_t runtimeID;
};

struct MaxAutoStepComponent {
public:
	float stepHeight;
};

struct FallDistanceComponent {
public:
	float fallDistance;
};
