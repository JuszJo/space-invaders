#ifndef BULLET_H
#define BULLET_H

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include "../libs/shader.h"

struct ActiveBullet {
    int index;

    bool active = false;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

    float acceleration = 5.0;

    glm::mat4 model = glm::mat4(1.0f);
};

class Bullet {
    public:
        unsigned int VAO, VBO, TBO;

        float width = 10.0;
        float height = 10.0;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 5.0;

        glm::mat4 model = glm::mat4(1.0f);

        ActiveBullet activeBullets[3];
        int currentIndex = 0;

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

        Shader* myShader;

        Bullet(Shader* shader) {
            myShader = shader;

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

            // setPosition();
        }

        void setPosition() {
            glm::vec3 newPosition = glm::vec3(100.0f, 0.0f, 0.0f);

            model = glm::translate(model, -position);

            model = glm::translate(model, newPosition);

            position = newPosition;
        };

        void activateBullet() {
            std::cout << "bullet activated\n";

            ActiveBullet newBullet;

            newBullet.index = currentIndex;
            newBullet.active = true;

            activeBullets[currentIndex] = newBullet;

            ++currentIndex;
        }

        void update(ActiveBullet* currentBullet) {
            currentBullet -> model = glm::translate(currentBullet -> model, glm::vec3(0.0f, 10.0f, 0.0f));
        }

        void render() {
            int modelLocation = glGetUniformLocation(myShader -> shaderProgram, "model");

            int numActive = 0;

            for(int i = 0; i < sizeof(activeBullets) / sizeof(activeBullets[0]); ++i) {
                ActiveBullet* currentBullet = &activeBullets[i];

                if(currentBullet -> active) {
                    ++numActive;

                    std::cout << "Num Active: " << numActive << std::endl;
            
                    // std::cout << "model: " << currentBullet.model << std::endl;

                    // update(&currentBullet);

                    currentBullet -> model = glm::translate(currentBullet -> model, glm::vec3(0.0f, 10.0f, 0.0f));
                    currentBullet -> position += glm::vec3(0.0f, 10.0f, 0.0f);

                    std::cout << currentBullet -> position.y << std::endl;

                    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(currentBullet -> model));

                    glBindVertexArray(VAO);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                }
            }

            // std::cout << "Num Active: " << numActive << std::endl;

            // std::cout << curr.active << std::endl;

        }
};

#endif