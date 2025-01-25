#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <unordered_map>
#include "cylinder.h"

using namespace std;

std::unordered_map<int, SmoothingGroup> smoothingGroups;
vector<float> vertexData;
std::vector<glm::vec3> tvertices;
std::vector<std::vector<int>> tfaces;
std::vector<unsigned int> tindices;



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