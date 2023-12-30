#ifndef BULLET_H
#define BULLET_H

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include "../libs/shader.h"

#include "../libs/stb_image.h"

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

        void loadImage(int textWidth, int textHeight, unsigned char* imageData) {
            glGenTextures(1, &TBO);
            glBindTexture(GL_TEXTURE_2D, TBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textWidth, textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        void setPosition(ActiveBullet* currentBullet, glm::vec3 newPosition) {
            currentBullet -> model = glm::translate(currentBullet -> model, -position);

            newPosition.x = (newPosition.x + 25) - (width / 2);
            newPosition.y += 40;

            currentBullet -> model = glm::translate(currentBullet -> model, newPosition);

            currentBullet -> position = newPosition;
        };

        void activateBullet(glm::vec3 shipPosition) {
            if(!(currentIndex > 2)) {
                // std::cout << "bullet activated\n";

                ActiveBullet newBullet;

                setPosition(&newBullet, shipPosition);

                newBullet.index = currentIndex;
                newBullet.active = true;

                activeBullets[currentIndex] = newBullet;

                ++currentIndex;
            }
        }

        void checkBulletBoundary(ActiveBullet* currentBullet) {
            if(currentBullet -> position.y > 600) {
                currentBullet -> active = false;
                if(currentBullet -> index < currentIndex) {
                    currentIndex = currentBullet -> index;
                }
            }
        }

        void update(ActiveBullet* currentBullet) {
            currentBullet -> model = glm::translate(currentBullet -> model, glm::vec3(0.0f, 10.0f, 0.0f));
            currentBullet -> position += glm::vec3(0.0f, 10.0f, 0.0f);
        }

        void render() {
            int modelLocation = glGetUniformLocation(myShader -> shaderProgram, "model");

            int numActive = 0;

            for(int i = 0; i < sizeof(activeBullets) / sizeof(activeBullets[0]); ++i) {
                ActiveBullet* currentBullet = &activeBullets[i];

                if(currentBullet -> active) {
                    ++numActive;

                    // std::cout << "Num Active: " << numActive << std::endl;
            
                    // std::cout << "model: " << currentBullet.model << std::endl;

                    update(currentBullet);

                    checkBulletBoundary(currentBullet);

                    // currentBullet -> model = glm::translate(currentBullet -> model, glm::vec3(0.0f, 10.0f, 0.0f));
                    // currentBullet -> position += glm::vec3(0.0f, 10.0f, 0.0f);

                    // std::cout << currentBullet -> position.y << std::endl;

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