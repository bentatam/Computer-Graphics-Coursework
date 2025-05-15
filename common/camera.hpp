#pragma once

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <common/maths.hpp>

class Camera 
{
public:
	//Projection Perameters
	float fov = Maths::radians(45.0f);//converts the camera fov from degrees to radians
	float aspect = 1024.0f / 768.0f;//calculates the aspect ratio of the window
	float near = 0.2f;//sets the near clipping plane(anything closer than this will not be rendered)
	float far = 100.0f;//sets the far clipping plane (anything further will not be rendered)

	//-------^^^^^^^^^^^^^^^^_________
	//Together used to create the projection matrix, which is what maps 3D coordinates into 2D screen coordinates.
	//------------------------------

	//Camera Vectors
	glm::vec3 eye;//this holds the position of the camera in world space
	glm::vec3 target;//this holds the position the camera is looking at in world space
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);//shows which direction is the global Up

	//These are the cameras orientation vectors, calculated based on 'eye' and 'target'
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);//which direction is the cameras RIGHT
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//which direction is the cameras UP
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);//Which direction is the cameras FRONT

	//Camera Euler Angles
	float yaw = Maths::radians(-90.0f);// Horizontal rotation: -90° so the camera initially faces forward (negative Z) - left/right rotation
	float pitch = 0.0f; // Vertical rotation: 0° means the camera is level - up/down rotation
	float roll = 0.0f;// Rotation around the front axis - rolling the camera (like a plane)

	//Transformation matrices
	glm::mat4 view; //view matrix - transforms world coordinates to the cameras POV
	glm::mat4 projection;//projection matrix - APplies perspective (the fov, aspect ration, near & far planes as declared above)

	//Constructor
	Camera(const glm::vec3 eye, const glm::vec3 target);// Initializes the camera with a position (eye) and a point to look at (target)

	//Methods
	void calculateMatrices();    // Recalculates the view and projection matrices based on camera vectors
	void calculateCameraVectors();  // Calculates the direction vectors (front, right, up) from yaw and pitch
};
