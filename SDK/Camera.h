#pragma once

#include "../Utils/HMath.h"
#include <glm/ext/matrix_float4x4.hpp>

class Camera {
public:
	BUILD_ACCESS(this, Vec4, lookingAt, 0x30);
	BUILD_ACCESS(this, Vec3, cameraPos, 0x40);
	BUILD_ACCESS(this, float, uiScale, 0x58);
	BUILD_ACCESS(this, float, fieldOfView, 0x5C);
	BUILD_ACCESS(this, float, nearClippingPlane, 0x60);
	BUILD_ACCESS(this, float, farClippingPlane, 0x64);
	BUILD_ACCESS(this, glm::mat4, nausea, 0x68);
	BUILD_ACCESS(this, Vec4, lookingAt2, 0xD8);
	BUILD_ACCESS(this, Vec3, cameraPos2, 0xE8);
	BUILD_ACCESS(this, float, uiScale2, 0x100);
	BUILD_ACCESS(this, float, nearClippingPlane2, 0x108);
	BUILD_ACCESS(this, float, farClippingPlane2, 0x10C);
	
	void getForwardVector(Vec3*);
	void getEulerAngles(Vec3*);
	void setOrientation(float pitch, float yaw, float roll);
	void setOrientationDeg(float pitch, float yaw, float roll) {
		setOrientation((-yaw - 180) * RAD_DEG, -pitch * RAD_DEG, roll * RAD_DEG);
	}
};

class CameraManager {
public:
	Camera* getCameraOrDebugCamera();
};