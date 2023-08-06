#pragma once

#include "../Utils/HMath.h"

#include "PlayerMovementProxy.h"

struct MobBodyRotationComponent {
	float bodyRot;
	float prevBodyRot;
};

struct ActorRotationComponent {
	Vec2 rot;
	Vec2 rotPrev;
};

struct ActorHeadRotationComponent {
	Vec2 rot;
};

struct RuntimeIDComponent {
	int64_t runtimeID;
};

struct MaxAutoStepComponent {
	float stepHeight;
};

struct FallDistanceComponent {
	float fallDistance;
};

class PlayerMovementProxy;

struct ActorMovementProxyComponent {
	PlayerMovementProxy* movementProxy;
};