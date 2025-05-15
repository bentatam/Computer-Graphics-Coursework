#include <common/maths.hpp>

glm::mat4 Maths::translate(const glm::vec3& v)
{
    glm::mat4 translate(1.0f);//creates a 4.4 identity matrix, diagonal elemtns are 1.0f (no scaling or rotation)
    translate[3][0] = v.x, translate[3][1] = v.y, translate[3][2] = v.z;//modifies the 4th collumn(index3) of the matrix, by setting the x,y,z positions to the respective parts of the input V
    return translate;
}

glm::mat4 Maths::scale(const glm::vec3& v)
{
    glm::mat4 scale(1.0f);//creates an identity matrix
    scale[0][0] = v.x; scale[1][1] = v.y; scale[2][2] = v.z;//modifies the diagonal elements of the matrix, which scales the object along the axis speciefied by xyz.
    return scale;
}

float Maths::radians(float angle)
{
    return angle * 3.1416f / 180.0f;//converts the angle from degrees to radians
}

glm::mat4 Maths::rotate(const float& angle, glm::vec3 v)
{
    v = glm::normalize(v); // normalizes v, as the length of the axis vector should not affect its rotation
    float c = cos(angle);//calculates the cosine of the angle
    float s = sin(angle);//calculates the sine of the angle
    float x2 = v.x * v.x, y2 = v.y * v.y, z2 = v.z * v.z;
    float xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
    float xs = v.x * s, ys = v.y * s, zs = v.z * s;

    glm::mat4 rotate;
    rotate[0][0] = (1 - c) * x2 + c;
    rotate[0][1] = (1 - c) * xy + zs;
    rotate[0][2] = (1 - c) * xz - ys;
    rotate[1][0] = (1 - c) * xy - zs;
    rotate[1][1] = (1 - c) * y2 + c;
    rotate[1][2] = (1 - c) * yz + xs;
    rotate[2][0] = (1 - c) * xz + ys;
    rotate[2][1] = (1 - c) * yz - xs;
    rotate[2][2] = (1 - c) * z2 + c;

    return rotate;
}
