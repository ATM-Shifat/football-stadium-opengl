#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinder.h"
#include "sphere2.h"
#include <random>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);;

void gallery(Cube &redChair,Shader &lightingShaderWithTexture, unsigned int &cubeVAO, Shader &lightingShader, float length, glm::mat4 alTogether);
void seatTexture(Cube& textureCube, Shader& lightingShaderWithTexture, unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 als);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.004f;
float ball_x = 0.0f;
float ball_y = 0.0f;
float ball_z = 0.0f;
float slopeX = 0.0f;
float slopeY = 0.0f;

float ball_angleX = 0.0f;
float ball_angleY = 0.0f;

int level = 1;
int score = 3;
float sc_f = 0.0f;
bool start = true;
int ball_count = 0;
int serial = 0;
bool win = false;

//initial values

float gun_x=0.0f;
float gun_y=0.7f;
float balloon_x=1.5f;
float balloon_y = 1.2f;
float point_radius = 0.1f;

bool first_block = true;
bool balloon = true;

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


DirectionLight directlight(
    0.0f, 10.0f, 5.0f,
    .05f, 0.05f, 0.05f,
    0.2f, 0.2f, 0.2f,
    0.2f, 0.2f, 0.2f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

//positions of spot light
glm::vec3 spotLightPositions[] = {
    //glm::vec3(-3.0f, 4.5f, -0.5f),
    //glm::vec3(-3.0f, 4.5f, 10.0f),
    //glm::vec3(2.0f,  4.5f,  -0.5f),
    glm::vec3(0.0f, 3.0f, 7.9f)
};
SpotLight spotlight(
    spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z,
    0.0f, -1.0f, 1.0f,
    0.05f, 0.05f, 0.05f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(3.50f,  4.50f,  2.0f),
    glm::vec3(3.5f,  4.5f,  12.0f),
    glm::vec3(-3.5f,  4.5f,  2.0f),
    glm::vec3(-3.5f,  4.5f,  12.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    .05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,    // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
float bullet_z = -5.2f;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Football Stadium Shooting Game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath = "field.png";
    string specularMapPath = "field.png";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube field = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "redPlastic.png";
    specularMapPath = "redPlastic.png";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube redChair = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "bluePlastic.png";
    specularMapPath = "bluePlastic.png";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube blueChair = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "brownPlastic.png";
    specularMapPath = "brownPlastic.png";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube brownChair = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "greenPlastic.png";
    specularMapPath = "greenPlastic.png";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube greenChair = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    

    diffuseMapPath = "scoreboard.jpg";
    specularMapPath = "scoreboard.jpg";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube scoreBoard = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "gameOver.jpg";
    specularMapPath = "gameOver.jpg";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube gameOver = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Winner.jpg";
    specularMapPath = "Winner.jpg";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube winner = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "football.png";
    specularMapPath = "football.png";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Sphere2  football = Sphere2(1.0f, 144, 72, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f, diffMap, specMap, 0.0f, 0.0f, 1.0f, 1.0f);


    Cylinder bar = Cylinder();
    bar.setRadius(0.1);
    //ourShader.use();
    //lightingShader.use();

    Sphere point = Sphere();

    // render loop
    // -----------
    camera.Position = glm::vec3(2.0, 3.0, 0.0);

    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<float> dis(0.0f,1.4f);
    float randomX = dis(gen);
    std::uniform_real_distribution<float> disY(0.0f, 0.55f);
    float randomY = disY(gen);

    int count = 0;
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        // point light 5

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, altogather, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        altogather = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        lightingShader.setMat4("model", altogather);
        //func(cubeVAO, lightingShader, altogather);


        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        // 
        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights


        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

       model = glm::mat4(1.0f);
       model = glm::translate(model, spotLightPositions[0]);
       model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
       ourShader.setMat4("model", model);
       ourShader.setVec3("color", glm::vec3(cr, cg, cb));
       glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
       //glDrawArrays(GL_TRIANGLES, 0, 36);
        


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();

        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        directlight.setUpDirectionLight(lightingShaderWithTexture);
        spotlight.setUpSpotLight(lightingShaderWithTexture);



        //sky



        //Base
        glm::mat4 modelMatrixForBase = glm::mat4(1.0f);
        glm::mat4 rotateY;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.8, -0.1f, -2.3));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0, 0.3, 15.0));
        modelMatrixForBase = altogather * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelMatrixForBase, 0.59, 0.34, 0.28);


        //0.0f, 1.0f, -3.0f
       


        if (level == 1)
            sc_f = 0.0f;
        else if (level == 2)
            sc_f = 1.0f;
        else if (level == 3)
            sc_f = 2.0f;


        

        point_radius = (4.0 - sc_f) * 0.05;
        if (ball_z + 7.9f >= 9.0)
        {
            start = false;
            if (ball_x >= (-0.70 + randomX - point_radius) && ball_x <= (-0.7 + randomX + point_radius) && 0.35f + ball_y >= (0.35 + randomY - point_radius) && 0.35f + ball_y <= (0.35 + randomY + point_radius))
            {
                if (serial == ball_count)
                {
                    if (score < 5)
                        score++;
                    else
                    {
                        if (level < 3)
                            level++;
                        else
                            win = true;
                        score = 3;
                    }
                    serial--;

                }
                randomX = dis(gen);
                randomY = disY(gen);

            }
            else
            {
                if (serial == ball_count)
                {
                    if (score > 0)
                        score--;
                    serial--;

                }
            }

            
            shoot = false;
            ball_x = 0.0f;
            ball_z = 0.0f;
            ball_y = 0.0f;
            //printf("%d\n", score);
        }

        float p_x = 1.0f;
        for (int i = 0; i < score; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(p_x, 2.64, 9.8f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(.07f, .07f, 0.07f));
            glm::mat4 modelForPoint = altogather * translateMatrix * scaleMatrix;
            football.drawSphereWithTexture(lightingShaderWithTexture, modelForPoint);
            p_x -= 0.4f;
        }

        if (shoot)
        {
            ball_z += 0.01f;
            ball_x = slopeY * ball_z;
            ball_y = -1 * slopeX * ball_z;
        }

        //ball
        translateMatrix = glm::translate(identityMatrix, glm::vec3(ball_x, 0.35f + ball_y, 7.9f + ball_z));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.07f, .07f, 0.07f));
        glm::mat4 modelForBall = altogather * translateMatrix * scaleMatrix;
        //sphere.drawSphere(lightingShader, modelForBall);
        football.drawSphereWithTexture(lightingShaderWithTexture, modelForBall);

        //arrow
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.01f, 0.35f, 7.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02, 0.02f, 0.2f));
        glm::mat4 rotateXA = glm::rotate(identityMatrix, glm::radians(ball_angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotateYA = glm::rotate(identityMatrix, glm::radians(ball_angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 modelForArrow = altogather * translateMatrix * rotateXA * rotateYA * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForArrow, 1.0, 1.0, 1.0);

        //point
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.7+randomX, 0.35+randomY, 9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0-sc_f, 4.0f - sc_f, 0.03f));
        glm::mat4 modelForPoint = altogather * translateMatrix * scaleMatrix;
        point.drawSphere(lightingShader, modelForPoint);


        //scoreBoard
        glm::mat4 modelForScore = identityMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.25f, 2.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 1.5, 0.2));
        modelForScore = altogather * modelForScore * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForScore, 0.9, 0.8, 0.9);

        modelForScore = identityMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.25f, 0.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 3.5, 0.1));
        modelForScore = altogather * modelForScore * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForScore, 0.9, 0.8, 0.9);

        modelForScore = identityMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.25f, 0.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 3.5, 0.1));
        modelForScore = altogather * modelForScore * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForScore, 0.9, 0.8, 0.9);
       
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.19f, 2.0f, 9.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 1.5, 0.2));
        model = altogather * translateMatrix * scaleMatrix;
        scoreBoard.drawCubeWithTexture(lightingShaderWithTexture, model);

        
        if (score == 0)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.19f, 2.0f, 9.85f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 1.5, 0.2));
            model = altogather * translateMatrix * scaleMatrix;
            gameOver.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        if (win)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.19f, 2.0f, 9.8f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 1.5, 0.2));
            model = altogather * translateMatrix * scaleMatrix;
            winner.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        //field
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.1f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.2f, 10.0f));
        model = altogather * translateMatrix * scaleMatrix;
        field.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Goal Bar-------------------------------------------------------------------------
        //Goal bar 1
        //bar 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(.75f, 0.1f, .77f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
        model = altogather * translateMatrix * scaleMatrix;
        bar.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);
        //bar 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.72f, 0.1f, .77f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
        model = altogather * translateMatrix * scaleMatrix;
        bar.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);
        //top 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.01f, 0.9f, .77f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.75f, 0.05f));
        model = altogather * translateMatrix * rotateZMatrix * scaleMatrix;
        bar.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);

        //Goal bar 2

        //bar 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(.75f, 0.1f, 9.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
        model = altogather * translateMatrix * scaleMatrix;
        bar.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);
        //bar 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.73f, 0.1f, 9.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
        model = altogather * translateMatrix * scaleMatrix;
        bar.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);
        //top 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.01f, 0.9f, 9.3f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.75f, 0.05f));
        model = altogather * translateMatrix * rotateZMatrix * scaleMatrix;
        bar.drawCylinder(lightingShader, model, 1.0, 1.0, 1.0);

        //Goal Bar ---------------------------------------------------------------------------------

        //Gallery---------------------------------------------------------------------------------
        //right
        float length = 8.3f;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.5f, 0.1f, 0.9f));
        model = altogather * translateMatrix;// *scaleMatrix;
        gallery(blueChair, lightingShaderWithTexture, cubeVAO, lightingShader, length, model);
        //back
        length = 5.0;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.6f, 0.1f, 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = altogather * translateMatrix * rotateYMatrix;
        gallery(redChair, lightingShaderWithTexture, cubeVAO, lightingShader, length, model);
        //front
        length = 5.0;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.1f, -2.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = altogather * translateMatrix * rotateYMatrix;
        gallery(blueChair, lightingShaderWithTexture, cubeVAO, lightingShader, length, model);

        //left 1

        length = 3.5;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 0.1f, 4.4f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = altogather * translateMatrix * rotateYMatrix;
        gallery(redChair, lightingShaderWithTexture, cubeVAO, lightingShader, length, model);

        //left 2
        length = 3.5;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 0.1f, 9.4f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = altogather * translateMatrix * rotateYMatrix;
        gallery(blueChair, lightingShaderWithTexture, cubeVAO, lightingShader, length, model);

        //Gallery---------------------------------------------------------------------------------


        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


void gallery(Cube& textureCube, Shader& lightingShaderWithTexture, unsigned int& cubeVAO, Shader& lightingShader, float length, glm::mat4 alTogether)
{
    float rotateAngle_X = 0.0f;
    float rotateAngle_Y = 0.0f;
    float rotateAngle_Z = 0.0f;


    glm::mat4 rotateY = glm::mat4(1.0f);
    glm::mat4 rotateZ = glm::mat4(1.0f);
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, model1, model2;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    int cnt = 0;

    float baseHeight = 0.1;
    float width = 5;

    //plamtform and seat
    for (float i = 0.0f; i < 3.0f; i += 1.0f) {
        translateMatrix = glm::translate(identityMatrix, glm::vec3(x , y + i/2.0, z));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3((3.0 - i) * .5, 0.5, length)); 
        model = alTogether * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

        //seat
        for (float j = 0.0f; j < length/1.06; j += 0.7f) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(x + 1.05 - i/2.1, y + 0.6 + i/2.0 , z + 0.1 + j));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(.4, .5, .4));
            model = alTogether * translateMatrix * scaleMatrix;
            seatTexture(textureCube, lightingShaderWithTexture, cubeVAO, lightingShader, model);
        }
    }
    //back wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(x - 0.25, y , z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25, 2.5, length));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

}


void seatTexture(Cube& textureCube, Shader& lightingShaderWithTexture, unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 als)
{


    float rotateAngle_X = 0.0f;
    float rotateAngle_Y = 0.0f;
    float rotateAngle_Z = 0.0f;


    glm::mat4 rotateY = glm::mat4(1.0f);
    glm::mat4 rotateZ = glm::mat4(1.0f);
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, model1, model2;

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    int cnt = 0;

    float height = 1.0f;
    float width = 1.0f;

    //back curve
    //back flat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(x, y, z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05, width , width));
    model = als * translateMatrix * scaleMatrix;

    textureCube.drawCubeWithTexture(lightingShaderWithTexture, model);
    //bottom flat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(x, y, z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(width, 0.05, width));
    model = als * translateMatrix * scaleMatrix;

    textureCube.drawCubeWithTexture(lightingShaderWithTexture, model);

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }


    //game control
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        if(!shoot)
            ball_angleX -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) if (!shoot) ball_angleX += 0.1f;;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        if (!shoot) ball_angleY -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        if (!shoot) ball_angleY += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        ball_count++;
        serial = ball_count;
        shoot = true;
        slopeX = tan(ball_angleX * 3.1416 / 180.0);
        slopeY = tan(ball_angleY * 3.1416 / 180.0);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        score = 3;
        level = 1;
        win = false;
    }



    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) level = 1;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) level = 2;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) level = 3;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
       // eyeY -= 2.5 * deltaTime;
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
       //eyeY -= 2.5 * deltaTime;
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }
}

void key_callback(GLFWwindow* window, int key,int scancode, int action, int mods)
{

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();

            directlight.turnAmbientOff();

            spotlight.turnAmbientOff();;
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();

            directlight.turnAmbientOn();

            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();

            directlight.turnDiffuseOff();

            spotlight.turnDiffuseOff();

            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();

            directlight.turnDiffuseOn();

            spotlight.turnDiffuseOn();

            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();

            directlight.turnSpecularOff();

            spotlight.turnSpecularOff();

            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();

            directlight.turnSpecularOn();

            spotlight.turnSpecularOn();

            specularToggle = !specularToggle;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}