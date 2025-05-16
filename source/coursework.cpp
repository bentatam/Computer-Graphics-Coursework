#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>

// Function prototypes
void keyboardInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window);

// Frame timers
float previousTime = 0.0f;  // time of previous iteration of the loop
float deltaTime = 0.0f;  // time elapsed since the previous frame

//Jumping Variables
bool isJumping = false;
float jumpTime = 0.0f;
const float jumpHeight = 2.0f;
const float jumpDuration = 1.0f;


bool rotateTeapots = false;

// Create camera object
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// Object struct
struct Object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};

// Light struct
struct Light
{
    glm::vec3 position;
    glm::vec3 colour;
    float constant;
    float linear;
    float quadratic;
    unsigned int type;
    glm::vec3 direction;
    float cosPhi;
};

// Create vector of light sources
std::vector<Light> lightSources;

int main(void)
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        //getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Lab08 Lighting", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Use back face culling
    glEnable(GL_CULL_FACE);

    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Capture mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Compile shader program
    unsigned int shaderID, lightShaderID;
    //shaderID      = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    shaderID = LoadShaders("vertexShader.glsl", "multipleLightsFragmentShader.glsl");
    lightShaderID = LoadShaders("lightVertexShader.glsl", "lightFragmentShader.glsl");

    // Activate shader
    glUseProgram(shaderID);

    // Load models
    Model teapot("../assets/teapot.obj");
    Model sphere("../assets/sphere.obj");
    Model crate("../assets/cube.obj");
    Model floor("../assets/plane.obj");
    Model wall("../assets/plane.obj");

    // Load the textures
    teapot.addTexture("../assets/blue.bmp", "diffuse");
    teapot.addTexture("../assets/diamond_normal.png", "normal");
    teapot.addTexture("../assets/neutral_specular.png", "specular");


    crate.addTexture("../assets/crate.jpg", "diffuse");
    crate.addTexture("../assets/neutral_normal.png", "specular");
    crate.addTexture("../assets/neutral_specular.png", "specular");


    floor.addTexture("../assets/stones_diffuse.png", "diffuse");
    floor.addTexture("../assets/stones_normal.png", "normal");
    floor.addTexture("../assets/stones_specular.png", "specular");

    wall.addTexture("../assets/bricks_diffuse.png", "diffuse");
    wall.addTexture("../assets/bricks_normal.png", "normal");
    wall.addTexture("../assets/bricks_specular.png", "specular");

    // Use wireframe rendering (comment out to turn off)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //TeapotPositions array
    glm::vec3 teapotPositions[] =
    {
        glm::vec3(-6.0f, 0.0f, 0.0f),
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(-5.0f, 0.0f, -5.0f),
        glm::vec3(5.0f, 0.0f, -5.0f)
    };

    //Crate Positions
    glm::vec3 cratePositions[] =
    {
        glm::vec3(-6.0f, -1.25f, 0.0f),
        glm::vec3(5.0f, -1.25f, 0.0f),
        glm::vec3(-5.0f, -1.25f, -5.0f),
        glm::vec3(5.0f, -1.25f, -5.0f)
    };

    glm::vec3 wallPositions[] =
    {
        glm::vec3(0.0f, 8.0f,  -10.0f),
        glm::vec3(0.0f, 8.0f,  10.0f),
        glm::vec3(10.0f, 8.0f,  10.0f),
        glm::vec3(-10.0f, 8.0f,  10.0f)
    };

    glm::vec3 wallRotations[] =
    {
        glm::vec3(1.0f, 0.0f, 0.0f),  // right wall
        glm::vec3(1.0f, 0.0f, 0.0f), // left wall
        glm::vec3(0.0f, 0.0f, 1.0f), //front wall
        glm::vec3(0.0f, 0.0f, 1.0f), //back wall
    };

    float wallAngles[] = {
        Maths::radians(90.0f), // right
        Maths::radians(-90.0f), //left
        Maths::radians(90.0f), //front 
        Maths::radians(-90.0f)//back
    };

    //Create Teapots
    std::vector<Object> objects;
    Object obj;

    obj.name = "teapot";//give the teapot object the name "teapot"
    for (unsigned int i = 0; i < 4; i++)
    {
        obj.position = teapotPositions[i];//set the position to the indexed vector 3 in the teapot position array
        obj.rotation = glm::vec3(1.0f, 1.0f, 1.0f);//set the rotation axis
        obj.scale = glm::vec3(0.75f, 0.75f, 0.75f);//set the scale
        obj.angle = Maths::radians(0.0f * i);//set the rotation
        objects.push_back(obj);// Add the configured object to the vector
    }

    //Add Crates
    obj.name = "crate";
    for (unsigned int i = 0; i < 4; i++)
    {
        obj.position = cratePositions[i];//set the position to the indexed vector 3 in the crate position array
        obj.rotation = glm::vec3(1.0f, 1.0f, 1.0f);//set the rotation axis
        obj.scale = glm::vec3(0.75f, 0.75f, 0.75f);//set the scale
        obj.angle = Maths::radians(0.0f * i);//set the rotation
        objects.push_back(obj);// Add the configured object to the vector
    }

    //Add Walls
    obj.name = "wall";
    for (int i = 0; i < 4; i++) {
        obj.position = wallPositions[i];
        obj.rotation = glm::vec3(1.0f, 0.0f, 0.0f);
        obj.angle = 90.0f;
        obj.scale = glm::vec3(10.0f, 10.0f, 10.0f);
        obj.name = "wall";
        obj.rotation = wallRotations[i];
        obj.angle = wallAngles[i];
        objects.push_back(obj);
    }


    // Add floor model to objects vector
    obj.name = "floor";
    obj.position = glm::vec3(0.0f, -2.0f, 0.0f);
    obj.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    obj.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    obj.angle = 0.0f;
    objects.push_back(obj);



    //-----------LIGHTING---------------
    teapot.ka = 0.2f;//sets ambient reflection coefficeint for teapots
    teapot.kd = 0.7f;//Diffuse reflection Coefficeint
    teapot.ks = 1.0f;//specular constant
    teapot.Ns = 20.0f;



    crate.ka = 0.2f;//sets ambient reflection coefficeint for crates
    crate.kd = 0.7f;//Diffuse reflection Coefficeint
    crate.ks = 1.0f;//specular constant
    crate.Ns = 20.0f;


    // Define floor light properties
    floor.ka = 0.2f;
    floor.kd = 1.0f;
    floor.ks = 1.0f;
    floor.Ns = 20.0f;

    // Define floor light properties
    wall.ka = 0.2f;
    wall.kd = 1.0f;
    wall.ks = 1.0f;
    wall.Ns = 3.0f;



    //Attenuation
    float constant = 1.0f;
    float linear = 0.05f;
    float quadratic = 0.02f;


    Light light;

    
    // Add first point light source
    light.position = glm::vec3(-5.0f, 2.0f, 5.0f);
    light.colour = glm::vec3(0.0f, 0.0f, 1.0f);
    light.constant = 1.0f;
    light.linear = 0.1f;
    light.quadratic = 0.02f;
    light.type = 1;
    lightSources.push_back(light);

    //// Add second point light source
    //light.position = glm::vec3(1.0f, 2.0f, -8.0f);
    //light.colour = glm::vec3(1.0f, 1.0f, 1.0f);
    //lightSources.push_back(light);


    // Add spotlight
    light.position = glm::vec3(5.0f, 3.0f, 0.0f);
    light.colour = glm::vec3(1.0f, 0.0f, 0.0f);
    light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    light.cosPhi = std::cos(Maths::radians(45.0f));
    light.type = 2;
    lightSources.push_back(light);

    //// Add directional light
    light.direction = glm::vec3(1.0f, -1.0f, 0.0f);
    light.colour = glm::vec3(1.0f, 1.0f, 1.0f);
    light.type = 3;
    lightSources.push_back(light);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;

        // Get inputs
        keyboardInput(window);
        mouseInput(window);

        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        glUseProgram(shaderID);

        // Send multiple light source properties to the shader
        for (unsigned int i = 0; i < static_cast<unsigned int>(lightSources.size()); i++)
        {
            glm::vec3 viewSpaceLightPosition = glm::vec3(camera.view * glm::vec4(lightSources[i].position, 1.0f));
            std::string idx = std::to_string(i);
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].colour").c_str()), 1, &lightSources[i].colour[0]);
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].position").c_str()), 1, &viewSpaceLightPosition[0]);
            glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].constant").c_str()), lightSources[i].constant);
            glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].linear").c_str()), lightSources[i].linear);
            glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].quadratic").c_str()), lightSources[i].quadratic);
            glUniform1i(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].type").c_str()), lightSources[i].type);

            //Spotlight
            glm::vec3 viewSpaceLightDirection = glm::vec3(camera.view * glm::vec4(lightSources[i].direction, 0.0f));
            glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].direction").c_str()), 1, &viewSpaceLightDirection[0]);
            glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].cosPhi").c_str()), lightSources[i].cosPhi);
        }


        // Send object lighting properties to the fragment shader
        glUniform1f(glGetUniformLocation(shaderID, "ka"), teapot.ka);
        glUniform1f(glGetUniformLocation(shaderID, "kd"), teapot.kd);
        glUniform1f(glGetUniformLocation(shaderID, "ks"), teapot.ks);
        glUniform1f(glGetUniformLocation(shaderID, "Ns"), teapot.Ns);

        // Calculate view and projection matrices
        camera.target = camera.eye + camera.front;
        camera.calculateMatrices();

        glUniformMatrix4fv(glGetUniformLocation(shaderID, "V"), 1, GL_FALSE, &camera.view[0][0]);

        //TeapotLoop
        for (int i = 0; i < static_cast<unsigned int>(objects.size()); i++)//for each object in objects
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);// Create translation matrix to move the object to its world position
            glm::mat4 scale = Maths::scale(objects[i].scale);// Create scaling matrix to resize the object
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);// Create rotation matrix based on angle and axis
            glm::mat4 model = translate * rotate * scale;// Combine transformations into a single model matrix

            // Calculate Model-View and Model-View-Projection matrices

            glm::mat4 MV = camera.view * model;// Combine model matrix with view matrix (camera)
            glm::mat4 MVP = camera.projection * MV;// Combine MV with projection matrix to get final MVP
  

            // Send matrices to the vertex shader as uniforms
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);// Upload MVP matrix to shader
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);// Upload MV matrix to shader


            // Draw the model
            if (objects[i].name == "teapot")
                teapot.draw(shaderID);

            if (objects[i].name == "floor")
                floor.draw(shaderID);

            if (objects[i].name == "crate")
                crate.draw(shaderID);

            if (objects[i].name == "wall")
                wall.draw(shaderID);
        }

        // ---------------------------------------------------------------------
        // Activate light source shader
        glUseProgram(lightShaderID);
        // Draw light sources

        for (unsigned int i = 0; i < static_cast<unsigned int>(lightSources.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(lightSources[i].position);
            glm::mat4 scale = Maths::scale(glm::vec3(0.1f));
            glm::mat4 model = translate * scale;

            // Send the MVP and MV matrices to the vertex shader
            glm::mat4 MVP = camera.projection * camera.view * model;
            glUniformMatrix4fv(glGetUniformLocation(lightShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

            // Send model, view, projection matrices and light colour to light shader
            glUniform3fv(glGetUniformLocation(lightShaderID, "lightColour"), 1, &lightSources[i].colour[0]);

            // Draw light source
            sphere.draw(lightShaderID);
        }


        if (isJumping)//if player is jumping
        {
            jumpTime += deltaTime;//count how long they have been jumping for
            float t = jumpTime / jumpDuration; //Normalized jump time

            if (t >= 1.0f)//if they have been jumping for over 1 second
            {
                isJumping = false;//stop the jump
                camera.eye.y = 0.0f; //end of jump, go back down
            }
            else
            {
                camera.eye.y = jumpHeight * sin(glm::pi<float>() * t); //creates a smooth arc up to the max jump height and back down
            }
        }
        else
        {
            camera.eye.y = 0.0f;
        }

        if (rotateTeapots)
        {
            for (int i = 0; i < static_cast<unsigned int>(objects.size()); i++)//for each object in teapotVector
            {
                Object& obj = objects[i]; // Get a reference to the current object
                if (obj.name == "teapot")
                {
                    obj.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
                    obj.angle += 5.0f * deltaTime;
                }
            }
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    teapot.deleteBuffers();
    glDeleteProgram(shaderID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera using WSAD keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.eye += 5.0f * deltaTime * camera.front;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.eye -= 5.0f * deltaTime * camera.front;
    }


    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.eye -= 5.0f * deltaTime * camera.right;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.eye += 5.0f * deltaTime * camera.right;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping)
    {
        isJumping = true;
        jumpTime = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        rotateTeapots = true;
    }

    else
    {
        rotateTeapots = false;
    }

}

void mouseInput(GLFWwindow* window)
{
    // Get mouse cursor position and reset to centre
    double xPos, yPos;//create an x and y variable for the mouse position
    glfwGetCursorPos(window, &xPos, &yPos);//set the x and y vairable to the mouse position in the window
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);//recentre the mouse

    // Update yaw and pitch angles
    camera.yaw += 0.0005f * float(xPos - 1024 / 2);//move the camera left/right depending on mousepos
    camera.pitch += 0.0005f * float(768 / 2 - yPos);//move the camera up/down depending on mousepos
}


