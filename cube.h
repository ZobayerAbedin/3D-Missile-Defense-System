#pragma once

// texture
float extra = 4.0f;
float TXmin = 0.0f;
float TXmax = 1.0f + extra;
float TYmin = 0.0f;
float TYmax = 1.0f + extra;


float cube_vertices[] = {
    // positions      // normals         // texture coords
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

    0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
    0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

    0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
    0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
    0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

    0.0f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
    0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
    0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
    0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
    0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
    0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
    0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
    0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
    0.0f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
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

void drawCube(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y, glm::vec4 color, float spec, float shininess)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);

    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", color * spec);
    ourShader.setFloat("material.shininess", shininess);
    //glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}





void Table(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y)
{

    float tableY = 0.05f;
    // Rotation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;

    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec3 pivot;

    pivot = glm::vec3(0.25, 0.25 + tableY, 0.25);

    //Top
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + tableY, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.1f, 1.3f));

    translateToPivotOrg = scaleMatrix * glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = scaleMatrix * glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;

    //model = translateMatrix * rotateYMatrix * scaleMatrix;
    //model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ourShader.setFloat("material.shininess", 128.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    pivot = translateFromPivotOrg;
    // Leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.02f, -0.46f + tableY, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.02f, -0.46f + tableY, 0.559f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.92f, -0.46f + tableY, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.92f, -0.46f + tableY, 0.559f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Chair(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Y)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateYMatrix;
    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec3 pivot;

    float fixLeg = 0.7f;
    float fixY = 0.1f;
    float fixYWhole = -0.08f;

    pivot = glm::vec3(0.25 + 0.11, 0.25 - fixY + fixYWhole, 0.25);

    // Top
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.11f, 0.0f - fixY + fixYWhole, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.75f, 0.07f, 0.7f));

    translateToPivotOrg = scaleMatrix * glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = scaleMatrix * glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.3f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.3f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    pivot = translateFromPivotOrg;
    // Leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    /*ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);*/
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //// Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.43f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    /*ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);*/
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.43f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    /*ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);*/
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Left up 
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.17f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.25f, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Right up
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.39f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.25f, 0.07f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Back support
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.15f, 0.09f - fixY + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 0.1f));

    model = translateFromPivot * rotateYMatrix * translateToPivot * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.3f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.3f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


