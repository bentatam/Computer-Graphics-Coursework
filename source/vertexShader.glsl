#version 330 core

//Inputs
layout(location = 0) in vec3 position; //telling OpenGL where to find the position data for the teapot's vertices so the shader can use that to draw the object.
layout(location = 1) in vec2 uv;//telling OpenGL where to find the texture coordinates so it knows how to map the texture correctly onto the 3D model
layout(location = 2) in vec3 normal;

//Outputs
out vec3 fragmentPosition;
out vec2 UV;//output a 2D vector holding texture coordinates
out vec3 Normal;

//uniforms
uniform mat4 MVP; //a variable with a value set outside the shader, which is a 4x4 matrix (mat4).
uniform mat4 MV;

void main()
{
	//Output Vertex position
	gl_Position = MVP * vec4(position, 1.0);//multiplies the object's position by the MVP matrix to apply the transformations (like translation, rotation, and projection) before sending the vertex to the fragment shader.

	//Output Vertex Colour
	UV = uv;

	//Output view space fragment position and normal vector
	fragmentPosition = vec3(MV * vec4(position, 1.0));// transform the vertex position into view space
	Normal = mat3(transpose(inverse(MV))) * normal;// transform the normal into view space
}