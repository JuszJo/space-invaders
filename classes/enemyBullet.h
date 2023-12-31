#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../libs/shader.h"

// #include "../libs/stb_image.h"

struct EnemyActiveBullet {
    int index;

    bool active = false;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

    float acceleration = 5.0;

    glm::mat4 model = glm::mat4(1.0f);
};

class EnemyBullet {
    public:
        unsigned int VAO, VBO, TBO;

        float width = 10.0;
        float height = 10.0;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

        float acceleration = 5.0;

        glm::mat4 model = glm::mat4(1.0f);

        int maxBullets = 10;

        EnemyActiveBullet activeBullets[10];
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

        EnemyBullet(Shader* shader) {
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

        void setPosition(EnemyActiveBullet* currentBullet, glm::vec3 newPosition) {
            // currentBullet -> model = glm::translate(currentBullet -> model, -position);

            newPosition.x = (newPosition.x + 25) - (width / 2);
            newPosition.y -= 40;

            currentBullet -> model = glm::translate(currentBullet -> model, newPosition);

            currentBullet -> position = newPosition;
        };

        void activateBullet(glm::vec3 shipPosition) {
            if(!(currentIndex > (maxBullets - 1))) {
                EnemyActiveBullet newBullet;

                setPosition(&newBullet, shipPosition);

                newBullet.index = currentIndex;
                newBullet.active = true;

                activeBullets[currentIndex] = newBullet;

                ++currentIndex;
            }
        }

        void checkBulletBoundary(EnemyActiveBullet* currentBullet) {
            if(currentBullet -> position.y < 0) {
                currentBullet -> active = false;
                if(currentBullet -> index < currentIndex) {
                    currentIndex = currentBullet -> index;
                }
            }
        }

        void update(EnemyActiveBullet* currentBullet) {
            currentBullet -> model = glm::translate(currentBullet -> model, glm::vec3(0.0f, -10.0f, 0.0f));
            currentBullet -> position += glm::vec3(0.0f, -10.0f, 0.0f);
        }

        void render() {
            int modelLocation = glGetUniformLocation(myShader -> shaderProgram, "model");

            int numActive = 0;

            for(int i = 0; i < sizeof(activeBullets) / sizeof(activeBullets[0]); ++i) {
                EnemyActiveBullet* currentBullet = &activeBullets[i];

                if(currentBullet -> active) {
                    ++numActive;

                    update(currentBullet);

                    checkBulletBoundary(currentBullet);

                    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(currentBullet -> model));

                    glBindVertexArray(VAO);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                }
            }
        }

        void reset() {
            for(int i = 0; i < sizeof(activeBullets) / sizeof(activeBullets[0]); ++i) {
                EnemyActiveBullet* currentBullet = &activeBullets[i];

                currentBullet -> active = false;
            }

            currentIndex = 0;
        }
};

#endif