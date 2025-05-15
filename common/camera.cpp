#include <common/camera.hpp>

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target)//Constructor for the camera
{
	eye = Eye;//save camera position
	target = Target;//save camera target (where its looking)
}

void Camera::calculateMatrices()
{
	calculateCameraVectors();//Updates the directions of front,right and up based on the yaw and pitch
	view = glm::lookAt(eye, target, worldUp);//builds a view matrix that transforms everyhting from the world space into the cameras POV

	// Calculate the projection matrix
	projection = glm::perspective(fov, aspect, near, far);
}

void Camera::calculateCameraVectors()
{
	front = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));//calculates a the direction the camera is facing from yaw and pitch. Creates a direction vector using triggenometry
	right = glm::normalize(glm::cross(front, worldUp));//Finds the right direction relative to the front and world up using the cross product.
	up = glm::cross(right, front);//Calculates the up direction of the camera based on the new front and right vectors.
}
