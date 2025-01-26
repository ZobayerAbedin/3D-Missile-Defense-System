#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

#include "Sphere2.h"
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "sphere.h"
#include "half_sphere.h"
#include "cylinder.h"
#include "cube.h"
#include "processInput.h"
#include "missile.h"
#include "binding.h"
#include "MissileLaunchingTruck.h"

#include "stb_image.h"

#include <iostream>
#include <stdlib.h>
#include<windows.h>  

using namespace std;

void read_file(string file_name, vector<float>& vec);
void drawTurret0(Shader ourShader, glm::vec4 color, glm::vec3 T);
void drawTurret(Shader ourShader, glm::vec4 color, glm::vec3 T);
void drawTruck(Shader lightingShader);
void Table(Shader ourshader, glm::mat4 moveMatrix, float rotateAngleTest_Y = 0);
void Chair(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y = 0);
void Floor(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void drawCube(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y, glm::vec4 color, float spec, float shininess);
void Road(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, glm::mat4 scaleMatrix);
void commandAndControl(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, unsigned int texture);
void load_texture(unsigned int& texture, string image_name, GLenum format);
void SetupPointLight(PointLight &pointLight, Shader ourShader, int lightNum);
void Door(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void ShowFunctions();

const int nt = 40;
const int ntheta = 30;
const double pi = 3.14159265389;

unsigned int bezierVAO, boatVAO, rotorVAO, cylinderVAO, carousalVAO, headVAO;

// Textures
unsigned int texture0, texture3, texture1, texture8,texture11;

unsigned int textureTurret;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    init();
   
    // Load OBJ file
    parseOBJ("truck.txt", tvertices, tfaces);
    vertexData = computeNormals(tvertices, tfaces);
    // Flatten faces into a single indices vector

    for (const auto& face : tfaces) {
        for (int index : face) {
            tindices.push_back(static_cast<unsigned int>(index)); // Corrected
        }
    }


    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightCubeShader("lightVertexShader.vs", "lightFragmentShader.fs");
    Shader cylinderShader("vertex.vs", "fragment.fs");


    unsigned int cubeVAO, cubeVBO, cubeEBO;
    unsigned int VBOt, VAOt, EBOt;

    bindCube(cubeVAO, cubeVBO, cubeEBO);
    bindTruck(VBOt, VAOt, EBOt);

   

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("transparent.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture 0" << endl;
    }
    stbi_image_free(data);


    load_texture(texture1, "desert.jpg", GL_RGB);
    load_texture(texture3, "brickwall_2.jpg", GL_RGB);
    load_texture(texture8, "road1.jpg", GL_RGB);
    load_texture(texture11, "wall0.jpg", GL_RGB);


    Sphere sphere = Sphere();
    Cylinder cylinder = Cylinder();

    Cylinder cylinder2 = Cylinder();
    Cylinder cylinder3 = Cylinder();
    HalfSphere half = HalfSphere();
    HalfSphere half2 = HalfSphere();


    ShowFunctions();

    // render loop
    // -----------
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        ourShader.use();
        glBindVertexArray(cubeVAO);

        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

       
        // camera/view transformation
        //glm::mat4 view = basic_camera.createViewMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        //**************lighting**************
        // 
        //Setting up Directional Light
        directionalLight.setUpLight(ourShader);
        if (!directionalLightOn)
            directionalLight.turnOff();
        if (!ambientOn)
            directionalLight.turnAmbientOff();
        if (!diffuseOn)
            directionalLight.turnDiffuseOff();
        if (!specularOn)
            directionalLight.turnSpecularOff();

        //Setting up Point Light
        SetupPointLight(pointLight1, ourShader, 1);
        SetupPointLight(pointLight2, ourShader, 2);
        SetupPointLight(pointLight3, ourShader, 3);
        SetupPointLight(pointLight4, ourShader, 4);

        //Setting up Spot Light
        spotLight.setUpLight(ourShader);
        if (!spotLightOn)
            spotLight.turnOff();
        if (!ambientOn)
            spotLight.turnAmbientOff();
        if (!diffuseOn)
            spotLight.turnDiffuseOff();
        if (!specularOn)
            spotLight.turnSpecularOff();

        //Setting up Camera and Others
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setBool("lightingOn", lightingOn);

        //cout << camera.Position[0] << " " << camera.Position[1] << " " << camera.Position[2] << endl;


        // Starting
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, scaleMatrix, rotateMatrix;
        glm::vec4 color1;

        float chairZ = 0.4f;
        float fixY = 0.45f;
        float chairX = 0.1f;

        //********** Object making ***********

        glActiveTexture(GL_TEXTURE0);
        //Ground
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -2.15f, 2.0f));
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture1);
        Floor(ourShader, translateMatrix, color1);

        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.2f, -2.145f, 12.0f));
        color1 = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture8);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 0.1f, 40.0f));
        Road(ourShader, translateMatrix, color1, scaleMatrix);
      
        //Door
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, -0.4f, -4.0f));
        color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
 
        Door(ourShader, translateMatrix, color1);
        
        color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::mat4 reflectionMatrix(1.0f);
        glm::vec3 normal(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                reflectionMatrix[i][j] = (i == j) ? 1.0f - 2.0f * normal[i] * normal[j] : -2.0f * normal[i] * normal[j];
            }
        }

        // Control center
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-12.6f, 0.0f, -4.0f));
        color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        commandAndControl(ourShader, reflectionMatrix * translateMatrix, color1, texture11);
        //inside control center
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.25f, 0.0f, 10.35f));
        Chair(ourShader, translateMatrix, -90.0f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.25f, 0.0f, 9.85f));
        Chair(ourShader, translateMatrix, -90.0f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 0.0f, 10.0f));
        Table(ourShader, translateMatrix, -90.0f);


          //base

        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.8f, -0.4f, -8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.6f, 1.8f, 6.6f));
        glm::mat4 moveMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        drawCube(ourShader, translateMatrix * scaleMatrix, 0.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 0.8f, 32.0f);


      


        // ========= RADAR =============

        half.updateRotation(0.016, 20.0f);

        glm::mat4 model = glm::mat4(1.0f);
        // Identity matrix
        moveMatrix = glm::translate(moveMatrix, glm::vec3(7.5f, 0.4f, -6.6f)); // Position at (2, 0, 3)

        // Draw the half-sphere
        half.drawHalfSphere(ourShader, moveMatrix, glm::vec4(0.5f, 0.5f, 0.7f, 1.0f), 0.5f, 32.0f);

        half2.updateRotation(0.016, 20.0f);
        moveMatrix = glm::translate(moveMatrix, glm::vec3(7.0f, -0.0f, 15.6f));
        half2.drawHalfSphere(ourShader, moveMatrix, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 0.5f, 32.0f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.0f, -0.4f, 8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.6f, 1.8f, 6.6f));
        moveMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        drawCube(ourShader, translateMatrix * scaleMatrix, 0.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 0.8f, 32.0f);

        glm::vec4 bodyColor2 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        model = glm::mat4(1.0f);


        glm::vec4 bodyColor3 = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
        lightCubeShader.use();
        lightCubeShader.setVec4("bodyColor", bodyColor3);

        // Set projection and view matrices in the shader
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        model = glm::mat4(1.0f);  // Identity matrix
        model = glm::translate(model, glm::vec3(8.5f, 1.5f, -6.0f));  // Translate cylinder
        model = glm::scale(model, glm::vec3(0.15f, 1.25f, 0.15f));

        lightCubeShader.setMat4("model", model);
        float rotationAngle = glfwGetTime() * 45.0f; // Dynamic rotation over time
        glm::vec3 rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f); // Rotate around Y-axis
        // Draw the cylinder using the shader
       // cylinder2.drawCylinder(lightCubeShader, model, bodyColor3, 0.5f, 32.0f, -1.0f * deltaTime);
        cylinder2.setRotation(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        // cylinder.setMotion(3.0f);

         //
         // =================== Missile =====================

        glm::vec4 bodyColor1 = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        lightCubeShader.use();
        lightCubeShader.setVec4("bodyColor", bodyColor1);

        // Set projection and view matrices in the shader
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        // Model matrix (translation and scaling)
        model = glm::mat4(1.0f);  // Identity matrix
        model = glm::translate(model, glm::vec3(9.3f, 1.10f, 3.2f));  // Translate cylinder
        model = glm::rotate(model, glm::radians(315.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));  // Optionally scale the cylinder



        // Set model matrix in the shader
        lightCubeShader.setMat4("model", model);
        rotationAngle = glfwGetTime() * 45.0f; // Dynamic rotation over time
        rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f); // Rotate around Y-axis
        // Draw the cylinder using the shader
        cylinder.drawCylinder(lightCubeShader, model, bodyColor1, 0.5f, 32.0f, -1.0f * deltaTime);
        cylinder.setRotation(45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        cylinder.setMotion(missile);


        // street lights

       

          //radar
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        float rotateAngleTest_Z = 45.0f;
        //PirateShip(ourShader, translateMatrix,rotateAngleTest_Z);
        glm::vec3 yAxis = glm::vec3(1.0f, 0.0f, 0.0f);
        rotationAngle = 45.0f; // Rotate 45 degrees


        // =============== TRUCK =================
        ourShader.use();
        color1 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, textureTurret);
        drawTurret0(lightCubeShader, color1, glm::vec3(9.7f, -2.5f, 2.5f));
        drawTruck(ourShader);
        // Define custom scaling factors for each axis
        float scaleX = 2.0f; // Scale in the X direction
        float scaleY = 1.5f; // Scale in the Y direction
        float scaleZ = 5.0f; // Scale in the Z direction

        // Create a scaling matrix
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));

        // Rotation angles (in degrees)
        float rotateAngleTest_Y = 45.0f; // Y-axis rotation
        float rotateAngleTest_X = 30.0f; // X-axis rotation
        rotateAngleTest_Z = 15.0f; // Z-axis rotation

        // Color, specular, and shininess properties for the cube
        glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f); // Red color
        float spec = 0.5f;
        float shininess = 32.0f;

        // Translation matrix (you provided this)
        translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(7.99f, 0.4f, -7.2f));




        //Lights
        lightCubeShader.use();
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.13f, 1.1f, 1.13f));

        // /for (int i = 0; i <= 0; i++) {

        // }

        for (int i = 1; i <= 1; i++)
        {
              //emissive
            glm::vec3 val = glm::vec3(0.5f);
            if (i == 1 and pointLightOn[0] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 2 and pointLightOn[1] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 3 and pointLightOn[2] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 4 and pointLightOn[3] == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 5 and spotLightOn == 0.0)
                bodyColor = glm::vec4(val, 1.0f);


            lightCubeShader.setVec4("bodyColor", bodyColor);
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
            lightCubeShader.setMat4("projection", projection);
            glm::mat4 view = camera.GetViewMatrix();
            lightCubeShader.setMat4("view", view);
            glm::mat4 tempModel = glm::mat4(1.0f);
            tempModel = glm::translate(tempModel, glm::vec3(10.2f, 12.35f, 7.98f));
            lightCubeShader.setMat4("model", tempModel * scaleMatrix);
            sphere.drawSphere(lightCubeShader);

        }


        //Texture drawing
        glDepthFunc(GL_LEQUAL);


        glm::mat4 view1 = glm::mat4(1.0f);
        glm::mat4 projection1 = glm::mat4(1.0f);
        view1 = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up)));
        projection1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glActiveTexture(GL_TEXTURE0);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}



//Texture Loading

void load_texture(unsigned int &texture, string image_name, GLenum format)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_name.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture "<<image_name << endl;
    }
    stbi_image_free(data);
}

void SetupPointLight(PointLight &pointLight, Shader ourShader, int lightNum)
{
    pointLight.setUpLight(ourShader);
    if (!pointLightOn[pointLight.lightNumber - 1])
        pointLight.turnOff();
    if (!ambientOn)
        pointLight.turnAmbientOff();
    if (!diffuseOn)
        pointLight.turnDiffuseOff();
    if (!specularOn)
        pointLight.turnSpecularOff();
}



void Floor (Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 1.25f + 0.45f, -16.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(40.3f, 0.1f, 60.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Road(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, glm::mat4 scaleMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 1.25f + 0.45f, -16.0f));
   // scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 0.1f, 60.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



void commandAndControl(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color, unsigned int texture = texture0)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix;


    //roof
    glBindTexture(GL_TEXTURE_2D, texture0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.7f, 0.95f, 12.86f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-4.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.8f, 0.29f, 6.0f));
    model = translateMatrix * rotateZMatrix *scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", color * 0.5f * 0.5f);
    ourShader.setFloat("material.shininess", 256.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //back side
    glBindTexture(GL_TEXTURE_2D, texture);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.4f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 2.75f, 5.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right side
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.99f, -0.419f, 12.99f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 2.75f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //left side
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * 0.5f);
    ourShader.setFloat("material.shininess", 32.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.99f, -0.419f, 15.651f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 2.75f, 0.2f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //front side
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.04f, 4.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //front left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 0.0f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.4f, 1.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //front right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 0.0f, 14.75f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.4f, 0.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //front up

    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 0.6f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.8f, 5.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
    //floor
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.01f, -0.42f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.9f, 0.1f, 5.5f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.1f);
    ourShader.setVec4("material.diffuse", color * 0.1f);
    ourShader.setVec4("material.specular", color * 0.5f * 0.1f);
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    
}

//door
void Door(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glBindTexture(GL_TEXTURE_2D, texture3);

    // Create identity matrix
    glm::mat4 identityMatrix = glm::mat4(1.0f);

    // Rotation matrices
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    // Define pivot point for door rotation (near one side of the door)
    glm::vec3 pivot = glm::vec3(7.5f, 0.0f, 15.0f);

    // Handle door angle change (open/close)
    gateAngle += isGateOpening * gateSpeed;
    if (isGateOpening == -1.0f)
        gateAngle = max(0.0f, gateAngle);
    if (isGateOpening == 1.0f)
        gateAngle = min(90.0f, gateAngle);

    // Translate and scale the door (cube)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.5f, 0.0f, 15.0f)); // Position of door
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 2.0f, 1.2999f)); // Cube size (door)
    model = translateMatrix * scaleMatrix;

    // Prepare for rotation
    glm::mat4 translateToPivot = glm::translate(identityMatrix, -pivot); // Translate to pivot
    glm::mat4 translateFromPivot = glm::translate(identityMatrix, pivot); // Translate from pivot
    rotateYMatrix = glm::rotate(identityMatrix,-1.0f*glm::radians(gateAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis

    // Combine transformations (translate to pivot, rotate, then translate back)
    model = translateFromPivot * rotateYMatrix * translateToPivot * model;

    // Apply shader and set material properties
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", color * 0.5f);
    ourShader.setVec4("material.diffuse", color * 0.5f);
    ourShader.setVec4("material.specular", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    ourShader.setFloat("material.shininess", 32.0f);

    // Draw the door (cube)
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}









void read_file(string file_name, vector<float>& vec)
{
    ifstream file(file_name);
    float number;

    while (file >> number)
        vec.push_back(number);

    file.close();
}



void ShowFunctions()
{
    cout << "Functions:\n~~~~~~~~~~~~~~~~~~~~\n";

    cout << "\nMovements:\n-----------------------------------\n";
    cout << "Move (Forward, Backward, Left, Right): (W, S, A, D)\n";
    cout << "Move (Up, Down): (E, R)\n";
    cout << "Pitch (Up, Down): (X, C)\n";
    cout << "Yaw (Right, Left): (Y, U)\n";
    cout << "Roll (Right, Left): (Z, Q)\n";
    cout << "-----------------------------------\n\n";

    cout << "\nLighting:\n-----------------------------------\n";
    cout << "Day and Night Toggle: 0\n";
    cout << "Point Lights ON/OFF: 1\n";
    cout << "Directional Light ON/OFF: 2\n";
    cout << "Spot Light ON/OFF: 3\n";
    cout << "Ambient ON/OFF: 4\n";
    cout << "Diffuse ON/OFF: 5\n";
    cout << "Specular ON/OFF: 6\n";
    cout << "-----------------------------------\n\n";
}
