#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"  // Include the Shader header file


struct SmoothingGroup {
    std::vector<int> faces; // Indices of faces in this group
};



class Cylinder {
public:
    // Constructor to initialize cylinder with optional radius, height, and number of slices and stacks
    Cylinder(float radius = 1.0f, float height = 2.0f, int sectorCount = 36, int stackCount = 18) {
        this->radius = radius;
        this->height = height;
        this->sectorCount = sectorCount;
        this->stackCount = stackCount;
        this->rotationAngle = 0.0f;  // No rotation by default
        this->rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);  // Default axis
        this->currentPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Start at origin
        this->motionSpeed = 0.0f; // No motion by default
        buildCoordinatesAndIndices();
    }
    void parseOBJ(const std::string& filePath,
        std::vector<glm::vec3>& vertices,
        std::vector<std::vector<int>>& faces) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        int currentSmoothingGroup = 0;
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v") {
                glm::vec3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (prefix == "f") {
                std::vector<int> face;
                std::string vertexStr;

                while (ss >> vertexStr) {
                    size_t doubleSlashPos = vertexStr.find("//");
                    if (doubleSlashPos != std::string::npos) {
                        // Handle "f 1//1 2//2 3//3"
                        std::string vertexIndex = vertexStr.substr(0, doubleSlashPos);
                        if (!vertexIndex.empty()) {
                            int index = std::stoi(vertexIndex) - 1; // Convert to 0-based index
                            face.push_back(index);
                        }
                    }
                    else if (vertexStr.find("/") == std::string::npos) {
                        // Handle "f 1 2 3"
                        int index = std::stoi(vertexStr) - 1; // Convert to 0-based index
                        face.push_back(index);
                    }
                    else {
                        std::cerr << "Unexpected face format: " << vertexStr << std::endl;
                    }
                }

                if (face.size() >= 3) {
                    faces.push_back(face);

                    // Add face to current smoothing group
                    if (currentSmoothingGroup > 0) {
                        smoothingGroups[currentSmoothingGroup].faces.push_back(faces.size() - 1);
                    }
                }
            }
            else if (prefix == "s") {
                std::string group;
                ss >> group;
                if (group == "off" || group == "0") {
                    currentSmoothingGroup = 0;
                }
                else {
                    currentSmoothingGroup = std::stoi(group);
                }
            }
        }

        std::cout << "Number of vertices: " << vertices.size() << std::endl;
        std::cout << "Number of indices: " << faces.size() << std::endl;

        file.close();

    }

    // Set rotation properties
    void setRotation(float angle, glm::vec3 axis) {
        this->rotationAngle = angle;
        this->rotationAxis = glm::normalize(axis);
    }
    // Compute normals and create combined array
    std::vector<float> computeNormals(const std::vector<glm::vec3>& vertices, const std::vector<std::vector<int>>& faces) {
        std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f));
        std::vector<float> vertexData;

        // Calculate face normals
        for (const auto& face : faces) {
            glm::vec3 v0 = vertices[face[0]];
            glm::vec3 v1 = vertices[face[1]];
            glm::vec3 v2 = vertices[face[2]];

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

            // Add face normal to vertex normals (for smooth shading)
            for (int index : face) {
                normals[index] += faceNormal;
            }
        }

        // Normalize vertex normals
        for (auto& normal : normals) {
            normal = glm::normalize(normal);
        }

        // Combine positions and normals
        for (size_t i = 0; i < vertices.size(); ++i) {
            const glm::vec3& position = vertices[i];
            const glm::vec3& normal = normals[i];

            vertexData.push_back(position.x);
            vertexData.push_back(position.y);
            vertexData.push_back(position.z);
            vertexData.push_back(normal.x);
            vertexData.push_back(normal.y);
            vertexData.push_back(normal.z);
        }

        return vertexData;
    }


    // Set motion properties
    void setMotion(float speed) {
        this->motionSpeed = speed;
    }

   

    // Function to render the cylinder with external transformations and fixed rotation
    void drawCylinder(Shader& shader, glm::mat4 model, glm::vec4 color, float spec, float shininess,float deltaTime) {
        shader.use();

        if (motionSpeed != 0.0f) {
            // Compute the direction vector based on the rotation axis and angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);

            // Assuming the "forward" direction is initially along the positive Z axis (aligned with the axis of the cylinder)
            glm::vec3 forwardDirection = glm::vec3(0.0f, 0.0f, 1.0f);

            // Rotate the forward direction vector by the rotation matrix to get the direction of movement
            glm::vec3 moveDirection = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forwardDirection, 0.0f)));

            // Update position along the moveDirection vector
            currentPosition += moveDirection * motionSpeed * deltaTime;
        }
        // Apply translation
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), currentPosition);
        model = translationMatrix * model;

        // Apply rotation
        if (rotationAngle != 0.0f) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
            model = model * rotationMatrix;
        }
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

        // Set model matrix in shader
        shader.setMat4("model", model);

        // Set material properties in shader
        shader.setVec4("material.ambient", color);
        shader.setVec4("material.diffuse", color);
        shader.setVec4("material.specular", color * spec);
        shader.setFloat("material.shininess", shininess);

        // Render the cylinder
        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, tindices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    // Function to generate coordinates, normals, and indices for the cylinder
    void buildCoordinatesAndIndices() {
        // Load OBJ file
        parseOBJ("missile.txt", tvertices, tfaces);
        vertexData = computeNormals(tvertices, tfaces);
        // Flatten faces into a single indices vector

        for (const auto& face : tfaces) {
            for (int index : face) {
                tindices.push_back(static_cast<unsigned int>(index)); // Corrected
            }
        }

        // Create VAO, VBOs, and EBO
        glGenVertexArrays(1, &cylinderVAO);
        glBindVertexArray(cylinderVAO);


        // OpenGL Buffer Setup
        unsigned int VBOt, VAOt, EBOt;
        glGenVertexArrays(1, &cylinderVAO);
        glGenBuffers(1, &VBOt);
        glGenBuffers(1, &EBOt);

        glBindVertexArray(cylinderVAO);

        // VBO for vertex data (positions and normals are precomputed in vertexData)
        glBindBuffer(GL_ARRAY_BUFFER, VBOt);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        // EBO for face indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOt);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tindices.size() * sizeof(unsigned int), tindices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind VAO (optional for safety)
        glBindVertexArray(0);
    }

    // Private variables for cylinder dimensions and buffers
    unsigned int cylinderVAO;
    std::vector<float> coordinates;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    float radius;
    float height;
    int sectorCount;
    int stackCount;
    float rotationAngle; // Rotation angle in degrees
    glm::vec3 rotationAxis; // Rotation axis
    glm::vec3 currentPosition; // Current position of the cylinder
    float motionSpeed; // Speed of motion
    std::unordered_map<int, SmoothingGroup> smoothingGroups;
    vector<float> vertexData;
    std::vector<glm::vec3> tvertices;
    std::vector<std::vector<int>> tfaces;
    std::vector<unsigned int> tindices;
};

#endif // CYLINDER_H
