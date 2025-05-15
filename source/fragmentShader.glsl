#version 330 core

// Inputs
in vec2 UV;//inputs the UV from the vertex shader
in vec3 fragmentPosition;//inputs the fragmentPostition as a vector
in vec3 Normal;//inputs normal

// Outputs
out vec3 fragmentColour;//ouptus the colour of the pixel

// Uniforms
uniform sampler2D diffuseMap;//This tells the shader that textureMap will hold a 2D texture.
uniform float ka; // Ambient reflection coefficient (controls how much ambient light the object reflects)
uniform float kd;//Diffuse reflection coefficient (controls the strength of diffused light)
uniform float ks;
uniform float Ns;
uniform vec3 lightColour;//Sets the colour of the light as a vector 3 (r,g,b)
uniform vec3 lightPosition;//sets the position of light in world space
//Attenuation Uniforms
uniform float constant;
uniform float linear;
uniform float quadratic;



void main()
{
    //Object Colour
    vec3 objectColour = vec3(texture(diffuseMap, UV));// Samples the texture using the UV coordinates to get the base color of the object

    //Anmbient Reflection
    vec3 ambient = ka * objectColour; // Applies the ambient reflection by multiplying the base color with the ambient coefficient

    //Diffuse Reflection
    vec3 light = normalize(lightPosition - fragmentPosition);// Calculate the direction vector from the fragment to the light source
    vec3 normal = normalize(Normal);// Normalize the interpolated surface normal
    float cosTheta = max(dot(normal, light), 0);// Compute the cosine of the angle between the normal and light direction, clamping to 0
    vec3 diffuse = kd * lightColour * objectColour * cosTheta;// Calculate the diffuse lighting contribution using Lambert's cosine law

    //Specular reflection
    vec3 camera = normalize(-fragmentPosition);
    vec3 reflection = - light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(camera, reflection), 0);
    vec3 specular = ks * lightColour * pow(cosAlpha, Ns);

    // Attenuation (change light intensity based on distance)
    float distance    = length(lightPosition - fragmentPosition);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);


    // Fragment colour
    fragmentColour = (ambient + diffuse + specular) * attenuation;
}