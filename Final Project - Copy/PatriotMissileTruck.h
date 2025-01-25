#ifndef PATRIOT_MISSILE_TRUCK_H
#define PATRIOT_MISSILE_TRUCK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"

class PatriotMissileTruck {
public:
    PatriotMissileTruck(unsigned int cubeVAO, unsigned int cylinderVAO, unsigned int coneVAO)
        : cubeVAO(cubeVAO), cylinderVAO(cylinderVAO), coneVAO(coneVAO) {
    }

    void draw(
        Shader& shader,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 position,
        float baseRotation,
        float turretElevation)
    {
        // Use the provided shader
        shader.use();

        // Set the projection and view matrices in the shader
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Model matrix for the truck
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(baseRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        // Draw parts of the truck
        drawChassis(shader, model);
        drawCabin(shader, model);
        drawWheels(shader, model);
        drawLaunchers(shader, model, turretElevation);
        drawAntenna(shader, model);
    }

private:
    unsigned int cubeVAO, cylinderVAO, coneVAO;

    void drawChassis(Shader& shader, glm::mat4& model)
    {
        glm::mat4 chassisModel = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
        chassisModel = glm::scale(chassisModel, glm::vec3(3.0f, 1.0f, 1.5f));
        shader.setMat4("model", chassisModel);
        shader.setVec3("objectColor", glm::vec3(0.4f, 0.2f, 0.1f));
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void drawCabin(Shader& shader, glm::mat4& model)
    {
        glm::mat4 cabinModel = glm::translate(model, glm::vec3(-1.0f, 1.25f, 0.7f));
        cabinModel = glm::scale(cabinModel, glm::vec3(1.0f, 0.8f, 0.8f));
        shader.setMat4("model", cabinModel);
        shader.setVec3("objectColor", glm::vec3(0.3f, 0.3f, 0.8f));
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void drawWheels(Shader& shader, glm::mat4& model)
    {
        std::vector<glm::vec3> wheelPositions = {
            glm::vec3(-1.2f, -0.25f,  1.0f),
            glm::vec3(1.2f, -0.25f,  1.0f),
            glm::vec3(-1.2f, -0.25f, -1.0f),
            glm::vec3(1.2f, -0.25f, -1.0f)
        };

        shader.setVec3("objectColor", glm::vec3(0.1f, 0.1f, 0.1f));
        glBindVertexArray(cylinderVAO);
        for (const auto& pos : wheelPositions) {
            glm::mat4 wheelModel = glm::translate(model, pos);
            wheelModel = glm::scale(wheelModel, glm::vec3(0.3f, 0.3f, 0.3f));
            wheelModel = glm::rotate(wheelModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            shader.setMat4("model", wheelModel);
            glDrawArrays(GL_TRIANGLES, 0, 36); // Adjust if cylinder has different vertex count
        }
    }

    void drawLaunchers(Shader& shader, glm::mat4& model, float turretElevation)
    {
        std::vector<glm::vec3> launcherPositions = {
            glm::vec3(1.5f, 1.0f, 0.7f),
            glm::vec3(1.5f, 1.0f,-0.7f),
            glm::vec3(-1.5f, 1.0f, 0.7f),
            glm::vec3(-1.5f, 1.0f,-0.7f)
        };

        shader.setVec3("objectColor", glm::vec3(0.8f, 0.2f, 0.2f));
        glBindVertexArray(cubeVAO);
        for (const auto& pos : launcherPositions) {
            glm::mat4 launcherModel = glm::translate(model, pos);
            launcherModel = glm::rotate(launcherModel, glm::radians(turretElevation), glm::vec3(1.0f, 0.0f, 0.0f));
            launcherModel = glm::scale(launcherModel, glm::vec3(0.2f, 0.5f, 0.2f));
            shader.setMat4("model", launcherModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void drawAntenna(Shader& shader, glm::mat4& model)
    {
        glm::mat4 antennaModel = glm::translate(model, glm::vec3(0.0f, 1.75f, 0.0f));
        antennaModel = glm::scale(antennaModel, glm::vec3(0.05f, 0.5f, 0.05f));
        shader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 0.0f));
        glBindVertexArray(cylinderVAO);
        shader.setMat4("model", antennaModel);
        glDrawArrays(GL_TRIANGLES, 0, 36); // Adjust if cylinder has different vertex count
    }
};

#endif
