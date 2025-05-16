#include <common/camera.hpp>

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target)//Constructor for the camera
{
	eye = Eye;//save camera position
	target = Target;//save camera target (where its looking)
}



glm::mat4 myLookAt(const glm::vec3& cameraPos, const glm::vec3& targetPos, const glm::vec3& upDir)
{
    // calculate forward vector from camera position to target and normalize it
    glm::vec3 forwardVec = glm::normalize(targetPos - cameraPos);

    // Calculate the right vector as perpendicular to forward and up vectors, normalized
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, upDir));

    // Calculate up vector
    glm::vec3 trueUpVec = glm::cross(rightVec, forwardVec);

    // Initialize 4x4 identity matrix
    glm::mat4 viewMatrix(1.0f);

    // Set rotation part of the matrix
    viewMatrix[0][0] = rightVec.x;
    viewMatrix[1][0] = rightVec.y;
    viewMatrix[2][0] = rightVec.z;

    viewMatrix[0][1] = trueUpVec.x;
    viewMatrix[1][1] = trueUpVec.y;
    viewMatrix[2][1] = trueUpVec.z;

    // Negative forward vector for correct camera orientation
    viewMatrix[0][2] = -forwardVec.x;
    viewMatrix[1][2] = -forwardVec.y;
    viewMatrix[2][2] = -forwardVec.z;

    // Translation: move world opposite of camera position by dot products
    viewMatrix[3][0] = -glm::dot(rightVec, cameraPos);
    viewMatrix[3][1] = -glm::dot(trueUpVec, cameraPos);
    viewMatrix[3][2] = glm::dot(forwardVec, cameraPos);

    return viewMatrix;
}

glm::mat4 myPerspective(float verticalFovRadians, float viewportAspect, float nearClip, float farClip)
{
    // Calculate half of the vertical field of view tangent
    float halfFovTan = std::tan(verticalFovRadians * 0.5f);

    glm::mat4 projMatrix(0.0f); // Initialize to zero matrix

    // X scaling based on aspect ratio and field of view
    projMatrix[0][0] = 1.0f / (viewportAspect * halfFovTan);

    // Y scaling based on field of view
    projMatrix[1][1] = 1.0f / halfFovTan;

    // Set depth range and perspective division
    projMatrix[2][2] = -(farClip + nearClip) / (farClip - nearClip);
    projMatrix[2][3] = -1.0f;

    projMatrix[3][2] = -(2.0f * farClip * nearClip) / (farClip - nearClip);

    return projMatrix;
}

// Method of Camera class that calculates the view and projection matrices
void Camera::calculateMatrices()
{
    // Updates camera directional vectors based on current rotation/orientation
    calculateCameraVectors();

    // Create the custom view matrix looking from eye towards eye + front with worldUp vector
    view = myLookAt(eye, eye + front, worldUp);

    // Create the custom perspective projection matrix based on camera FOV, aspect ratio, near and far planes
    projection = myPerspective(fov, aspect, near, far);
}

void Camera::calculateCameraVectors()
{
	front = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));//calculates a the direction the camera is facing from yaw and pitch. Creates a direction vector using triggenometry
	right = glm::normalize(glm::cross(front, worldUp));//Finds the right direction relative to the front and world up using the cross product.
	up = glm::cross(right, front);//Calculates the up direction of the camera based on the new front and right vectors.
}

//void Camera::calculateMatrices()
//{
//	calculateCameraVectors();//Updates the directions of front,right and up based on the yaw and pitch
//	view = glm::lookAt(eye, target, worldUp);//builds a view matrix that transforms everyhting from the world space into the cameras POV
//
//	// Calculate the projection matrix
//	projection = glm::perspective(fov, aspect, near, far);
//}

