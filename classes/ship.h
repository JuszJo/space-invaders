#ifndef SHIP_H
#define SHIP_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../libs/shader.h"

#include "../classes/bullet.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

class Ship {
    public:
        unsigned int VAO, VBO, TBO;

        float width = 50.0;
        float height = 50.0;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 10.0;

        glm::mat4 model = glm::mat4(1.0f);

        float vertices[16] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            width, 0.0f, 0.0f, 1.0f, 0.0f,
            width / 2, height, 0.0f, 0.5f, 1.0f
        };

        enum STATES {
            LEFT,
            RIGHT,
            IDLE,
            OVER
        };

        STATES currentState = IDLE;

        Shader* myShader;

        Bullet* bullet;

        int shootBuffer = 10;
        int elapsedFrames = 0;

        Ship(Shader* shader) {
            myShader = shader;

            bullet = new Bullet(shader);

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

            int textWidth, textHeight, numberOfChannels;

            unsigned char* imageData = stbi_load("wall.jpg", &textWidth, &textHeight, &numberOfChannels, 0);

            bullet -> loadImage(textWidth, textHeight, imageData);

            glGenTextures(1, &TBO);
            glBindTexture(GL_TEXTURE_2D, TBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textWidth, textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(imageData);

            setPosition(glm::vec3(300.0f - (width / 2), 0.0f, 0.0f));
        };

        void processInput(GLFWwindow* window) {
            if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }

            if(currentState != OVER) {
                if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                    if(elapsedFrames % shootBuffer == 0) {
                        shoot();

                        elapsedFrames = 0;
                    }

                    ++elapsedFrames;
                }

                if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                    currentState = LEFT;
                }
                else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                    currentState = RIGHT;
                }
                else {
                    currentState = IDLE;
                }
            }

        };

        void stateChecker() {
            switch (currentState) {
                case LEFT:
                    speed = glm::vec3(-acceleration, 0.0f, 0.0f);

                    break;

                case RIGHT:
                    speed = glm::vec3(acceleration, 0.0f, 0.0f);

                    break;

                case IDLE:
                    speed = glm::vec3(0.0f, 0.0f, 0.0f);

                    break;
                
                default:
                    break;
            }
        };

        void shoot() {
            bullet -> activateBullet(position);
        };

        void setPosition(glm::vec3 newPosition) {
            model = glm::translate(model, -position);

            model = glm::translate(model, newPosition);

            position = newPosition;
        };

        void checkWallCollision() {
            if(position.x < 0) {
                glm::vec3 newPosition = glm::vec3(0.0f, 0.0f, 0.0f);

                setPosition(newPosition);
            }
            if(position.x + width > 600) {
                glm::vec3 newPosition = glm::vec3(600.0f - width, 0.0f, 0.0f);

                setPosition(newPosition);
            }
        }

        void applySpeed() {
            model = glm::translate(model, speed);

            // int modelLocation = glGetUniformLocation(myShader -> shaderProgram, "model");

            // glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            position += speed;
        };

        void render() {
            if(currentState != OVER) {
                // glBindTexture(GL_TEXTURE_2D, TBO);
                int modelLocation = glGetUniformLocation(myShader -> shaderProgram, "model");

                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
        };

        void update() {
            if(currentState != OVER) {
                stateChecker();

                applySpeed();

                checkWallCollision();
            }
        };

        void reset() {
            currentState = IDLE;

            glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

            setPosition(glm::vec3(300.0f - (width / 2), 0.0f, 0.0f));

            int shootBuffer = 10;

            int elapsedFrames = 0;
        }
};

#endif