#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Constants.h"
#include <iostream>

using namespace glm;


class Camera {
	GLFWwindow* window;
	glm::vec3 positionVec;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	bool firstRender;
public:
	Camera::Camera(
		GLFWwindow* windoww,
		glm::vec3 pos,
		float hAngle,
		float vAngle,
		float initFoV,
		float speedOfCamera,
		float speedOfMouse) {
		window = windoww;
		positionVec = pos;
		horizontalAngle = hAngle;
		verticalAngle = vAngle;
		initialFoV = initFoV;
		speed = speedOfCamera;
		mouseSpeed = speedOfMouse;
		firstRender = true;
		
	};

	glm::mat4 getViewMatrix() {
		return ViewMatrix;
	}
	glm::mat4 getProjectionMatrix() {
		return ProjectionMatrix;
	}
	glm::vec3 getPosition() {
		return positionVec;
	}
	float getFoV() {
		return initialFoV;
	}

	void computeMatricesFromInputs() {
		// glfwGetTime is called only onsce, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		if (!firstRender) {
			horizontalAngle += mouseSpeed * float(WINDOW_WIDTH / 2 - xpos);
			verticalAngle += mouseSpeed * float(WINDOW_HEIGHT / 2 - ypos);
		}
		else {
			firstRender = false;
		}

		// Make sure that when vertical angle is out of bounds, screen doesn't get flipped
		// 1.57 to 90 stopni w radianach
		if (verticalAngle > 1.57f)
			verticalAngle = 1.57f;
		if (verticalAngle < -1.57f)
			verticalAngle = -1.57f;

		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		glm::vec3 up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			positionVec += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			positionVec -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			positionVec += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			positionVec -= right * deltaTime * speed;
		}
		// Strafe up
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			positionVec += up * deltaTime * speed;
		}
		// Strafe down
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			positionVec -= up * deltaTime * speed;
		}

		float FoV = initialFoV;

		ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			positionVec,
			positionVec + direction,
			up
		);
		lastTime = currentTime;
	}
};

#endif