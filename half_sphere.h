#ifndef HALF_SPHERE_H
#define HALF_SPHERE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h" // Include the Shader header file

class HalfSphere {
public:
    HalfSphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18) {
        this->radius = radius;
        this->sectorCount = sectorCount;
        this->stackCount = stackCount;
        this->rotationAngle = 0.0f; // Initial rotation angle
        buildCoordinatesAndIndices();
    }

    // Update rotation (incremental rotation over time)
    void updateRotation(float deltaTime, float rotationSpeed) {
        rotationAngle += rotationSpeed * deltaTime; // Increment angle based on time and speed
        if (rotationAngle >= 360.0f)
            rotationAngle -= 360.0f; // Keep the angle within [0, 360]
    }

    // Function to render the rotating half-sphere
    void drawHalfSphere(Shader& shader, glm::mat4 moveMatrix, glm::vec4 color, float spec, float shininess) {
        shader.use();

        // Apply rotation around the Y-axis (local rotation)
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = moveMatrix * rotationMatrix; // Combine the moveMatrix and rotationMatrix

        // Set the model matrix in the shader
        shader.setMat4("model", model);

        // Set material properties in shader
        shader.setVec4("material.ambient", color);
        shader.setVec4("material.diffuse", color);
        shader.setVec4("material.specular", color * spec);
        shader.setFloat("material.shininess", shininess);

        // Render the half-sphere
        glBindVertexArray(halfSphereVAO);
        glDrawElements(GL_TRIANGLES, (GLuint)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    // Function to generate coordinates and indices for the half-sphere
    void buildCoordinatesAndIndices() {
        float x, y, z, xy; // Vertex positions
        float nx, ny, nz, lengthInv = 1.0f / radius; // Normals
        float sectorStep = 2 * glm::pi<float>() / sectorCount;
        float stackStep = glm::pi<float>() / (2 * stackCount); // Only half-sphere
        float stackAngle, sectorAngle;

        // Generate vertices, normals
        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = glm::pi<float>() / 2 - i * stackStep; // From 90 to 0 degrees
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);
                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);
            }
        }

        // Generate indices
        for (int i = 0; i < stackCount; ++i) {
            int k1 = i * (sectorCount + 1); // Current stack
            int k2 = k1 + sectorCount + 1; // Next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        // Create VAO, VBOs, and EBO
        glGenVertexArrays(1, &halfSphereVAO);
        glBindVertexArray(halfSphereVAO);

        unsigned int VBO, EBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(float), &coordinates[0], GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Private variables for half-sphere properties and buffers
    unsigned int halfSphereVAO;
    std::vector<float> coordinates;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    float radius;
    int sectorCount;
    int stackCount;
    float rotationAngle; // Rotation angle in degrees
};

#endif // HALF_SPHERE_H
