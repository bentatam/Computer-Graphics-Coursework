#pragma once

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

//maths class
class Maths
{
public:
	//Transformation Matricies - Static functiones called iwthout creating an instance of the class
	static glm::mat4 translate(const glm::vec3& v);//translation matrix - used to move an object in space, by vector v. return a mat4 that represents the translation

	static glm::mat4 scale(const glm::vec3& v);//creates a scaling matrix - changes the size of an object on each axis from the input v (vec3)

	static float radians(float angle);//Converts angles from degrees to radians - which is what openGL uses.

	static glm::mat4 rotate(const float& angle, glm::vec3 v);//creates a rotation matrix - rotates by the angle in radians defined by 'angle' on the axis set by v (vec3)
};

