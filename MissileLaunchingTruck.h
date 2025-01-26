#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

void drawCube(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y, glm::vec4 color, float spec, float shininess);
void drawTurret(Shader ourShader, glm::vec4 color, glm::vec3 T);



void drawTurret0(Shader ourShader, glm::vec4 color, glm::vec3 T) {

    //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //glm::mat4 translateMatrix, scaleMatrix;
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(T[0] + 1.0f, T[1] + 4.25f, T[2] + 2.5f));
    //translateMatrix = glm::rotate(translateMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //translateMatrix = glm::scale(translateMatrix, glm::vec3(4.0f, 4.0f, 0.2f));
    //drawCube(ourShader, translateMatrix, 0, color, 0.5, 0.2);

    drawTurret(ourShader, color, glm::vec3(T[0] + 0.0f, T[1] + 0.0f, T[2] + 0.0f));
    drawTurret(ourShader, color, glm::vec3(T[0] + 0.75f, T[1] + 0.0f, T[2] + 0.0f));
    drawTurret(ourShader, color, glm::vec3(T[0] + 0.0f, T[1] + 0.75f, T[2] + 0.0f));
    drawTurret(ourShader, color, glm::vec3(T[0] + 0.75f, T[1] + 0.75f, T[2] + 0.0f));
}

void drawTurret(Shader ourShader, glm::vec4 color, glm::vec3 T) {

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(T[0] + 0.0f, T[1] + 5.0f, T[2] + 0.0f));
    translateMatrix = glm::rotate(translateMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::scale(translateMatrix, glm::vec3(1.0f, 0.2f, 5.0f));
    drawCube(ourShader, translateMatrix, 0, color, 0.5, 0.2);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(T[0] + 0.0f, T[1] + 4.5f, T[2] + 0.0f));
    translateMatrix = glm::rotate(translateMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::scale(translateMatrix, glm::vec3(1.0f, 0.2f, 5.0f));
    drawCube(ourShader, translateMatrix, 0, color, 0.5, 0.2);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(T[0] + 0.0f, T[1] + 4.5f, T[2] + 0.0f));
    translateMatrix = glm::rotate(translateMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 1.0f, 5.0f));
    drawCube(ourShader, translateMatrix, 0, color, 0.5, 0.2);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(T[0] - 0.5f, T[1] + 4.5f, T[2] + 0.0f));
    translateMatrix = glm::rotate(translateMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 1.2f, 5.0f));
    drawCube(ourShader, translateMatrix, 0, color, 0.5, 0.2);

    //translateMatrix = glm::translate(identityMatrix, glm::vec3(T[0] + 0.0f, T[1] + 4.5f, T[2] + 2.5f));
    //translateMatrix = glm::rotate(translateMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //translateMatrix = glm::scale(translateMatrix, glm::vec3(1.2f, 1.2f, 0.2f));
    //drawCube(ourShader, translateMatrix, 0, color, 0.5, 0.2);

}

void drawTruck(Shader lightingShader) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, rotateXMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.0f, -9.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.03f, 0.03f));
    rotateXMatrix = glm::rotate(identityMatrix, -1 * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //translateMatrix = glm::translate(identityMatrix, sofaTranslation);
    model = rotateXMatrix * translateMatrix * scaleMatrix;
    // Define the wood color
    glm::vec4 woodColor = glm::vec4(0.52f, 0.37f, 0.26f, 1.0f); // Medium brown (wood-like)

    // Setting up material properties
    lightingShader.setVec3("material.ambient", glm::vec3(woodColor) * 0.5f); // Darker for ambient
    lightingShader.setVec3("material.diffuse", glm::vec3(woodColor));         // Main color for diffuse
    lightingShader.setVec3("material.specular", glm::vec3(0.2f, 0.2f, 0.2f)); // Subtle highlights for specular
    lightingShader.setFloat("material.shininess", 16.0f);

    lightingShader.setMat4("model", model);
    // Bind VAO
    //glBindVertexArray(VAOt);

    // Draw the object
    glDrawElements(GL_TRIANGLES, tindices.size(), GL_UNSIGNED_INT, 0);

}

