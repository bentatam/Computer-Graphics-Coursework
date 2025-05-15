#version 330 core

# define maxLights 10

//Inputs
layout(location = 0) in vec3 position; //telling OpenGL where to find the position data for the teapot's vertices so the shader can use that to draw the object.
layout(location = 1) in vec2 uv;//telling OpenGL where to find the texture coordinates so it knows how to map the texture correctly onto the 3D model
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

//Outputs
out vec3 fragmentPosition;
out vec2 UV;//output a 2D vector holding texture coordinates
//out vec3 Normal;
out vec3 tangentSpaceLightPosition[maxLights];
out vec3 tangentSpaceLightDirection[maxLights];

struct Light
{
    vec3 position;
    vec3 colour;
    float constant;
    float linear;
    float quadratic;
    int type;
    vec3 direction;
    float cosPhi;
};

//uniforms
uniform mat4 MVP; //a variable with a value set outside the shader, which is a 4x4 matrix (mat4).
uniform mat4 MV;
uniform Light lightSources[maxLights];

void main()
{
	//Output Vertex position
	gl_Position = MVP * vec4(position, 1.0);//multiplies the object's position by the MVP matrix to apply the transformations (like translation, rotation, and projection) before sending the vertex to the fragment shader.

	//Output Vertex Colour
	UV = uv;

    // Calculate the TBN matrix that transforms view space to tangent space
    mat3 invMV = transpose(inverse(mat3(MV)));
    vec3 t     = normalize(invMV * tangent);
    vec3 n     = normalize(invMV * normal);
    t          = normalize(t - dot(t, n) * n);
    vec3 b     = cross(n, t);
    mat3 TBN   = transpose(mat3(t, b, n));

	// Output tangent space fragment position, light positions and directions
    fragmentPosition = TBN * vec3(MV * vec4(position, 1.0));
    for (int i = 0; i < maxLights; i++)
    {
        tangentSpaceLightPosition[i]  = TBN * lightSources[i].position;
        tangentSpaceLightDirection[i] = TBN * lightSources[i].direction;
    }
}