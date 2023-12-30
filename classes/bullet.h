#ifndef BULLET_H
#define BULLET_H

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include "../libs/shader.h"

class Bullet {
    public:
        unsigned int VAO, VBO, TBO;

        float width = 10.0;
        float height = 10.0;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 10.0;

        glm::mat4 model = glm::mat4(1.0f);

        float vertices[20] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            width, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, height, 0.0f, 0.0f, 0.0f,
            width, height, 0.0f, 0.5f, 1.0f
        };

        enum STATES {
            LEFT,
            RIGHT,
            IDLE,
            SHOT,
        };

        STATES currentState = IDLE;

        // Shader* myShader;

        Bullet() {
            // myShader = shader;

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void render() {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif