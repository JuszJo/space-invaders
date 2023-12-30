#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "libs/shader.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include "libs/stb_image.h"

#include "classes/ship.h"
#include "classes/enemy.h"
#include "classes/bullet.h"

bool checkEnemyCollision(Enemy enemy, ActiveBullet currentBullet) {
    if(
        currentBullet.position.y + 10 > enemy.position.y - enemy.height &&
        currentBullet.position.y < enemy.position.y &&
        currentBullet.position.x + 10 > enemy.position.x &&
        currentBullet.position.x < enemy.position.x + enemy.width
    ) {
        // std::cout << "Bullet Hit" << std::endl;
        return true;
    }
    else {
        return false;
    }
}

void checkEnemyBulletHit(Enemy* enemy, Bullet* bullet) {
    for(int i = 0; i < sizeof(bullet -> activeBullets) / sizeof(bullet -> activeBullets[0]); ++i) {
        ActiveBullet currentBullet = bullet -> activeBullets[i];

        if(currentBullet.active) {
            if(checkEnemyCollision(*enemy, currentBullet)) {
                enemy -> currentState = enemy -> DESTROYED;
            }
        }
    }
}

bool checkPlayerCollision(Ship ship, EnemyActiveBullet currentBullet) {
    // std::cout << currentBullet.position.x << "\t" << currentBullet.position.y << "\t" << ship.position.x << "\t" << ship.position.y << std::endl;

    if(
        currentBullet.position.y + 10 < ship.position.y + ship.height &&
        currentBullet.position.y > ship.position.y &&
        currentBullet.position.x + 10 > ship.position.x &&
        currentBullet.position.x < ship.position.x + ship.width
    ) {
        // std::cout << "Bullet Hit" << std::endl;
        return true;
    }
    else {
        return false;
    }
}

void checkPlayerBulletHit(Ship* ship, EnemyBullet* bullet) {
    for(int i = 0; i < sizeof(bullet -> activeBullets) / sizeof(bullet -> activeBullets[0]); ++i) {
        EnemyActiveBullet currentBullet = bullet -> activeBullets[i];

        if(currentBullet.active) {
            if(checkPlayerCollision(*ship, currentBullet)) {
                // std::cout << "PLAYER HIT" << std::endl;
                ship -> currentState = ship -> OVER;
            }
        }
    }
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 600;
    int height = 600;

    GLFWwindow* window = glfwCreateWindow(width, height, "TITLE", NULL, NULL);

    if(!window) {
        std::cout << "Window failed to create";

        return -1;
    }

    glfwMakeContextCurrent(window);

    int glewError = glewInit();

    if(glewError) {
        std::cout << "Glew init error";

        return -1;
    }

    glfwSwapInterval(1);

    double FPS = 120.0;

    double FPSInterval = 1000 / FPS;

    double lastFrameTime = glfwGetTime();

    double elapsed = 0.0;

    Shader shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    Ship ship(&shader);

    Enemy enemy(&shader);

    // int textWidth, textHeight, numberOfChannels;

    // unsigned char* imageData = stbi_load("wall.jpg", &textWidth, &textHeight, &numberOfChannels, 0);

    // glGenTextures(1, &ship.TBO);
    // glBindTexture(GL_TEXTURE_2D, ship.TBO);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textWidth, textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(imageData);

    shader.use();

    glm::mat4 projection = glm::mat4(1.0f);

    projection = glm::ortho(0.0f, 600.0f, 0.0f, 600.0f, -1.0f, 1.0f);

    int projectionLocation = glGetUniformLocation(shader.shaderProgram, "projection");

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    int gameOverBuffer = 100;
    int elapsedFramesOver = 1;

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.7f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ship.processInput(window);

        enemy.processInput(window);

        double currentFrameTime = glfwGetTime();

        double delta = currentFrameTime - lastFrameTime;

        elapsed += delta * 1000;

        lastFrameTime = currentFrameTime;

        if(elapsed >= FPSInterval) {
            elapsed = 0;

            ship.update();
        }

        ship.render();

        enemy.update();
        
        enemy.render();

        ship.bullet->render();

        checkEnemyBulletHit(&enemy, ship.bullet);

        if(enemy.currentState != enemy.DESTROYED) {
            enemy.bullet->render();

            checkPlayerBulletHit(&ship, enemy.bullet);
        }

        if(enemy.currentState == enemy.DESTROYED) {
            if(elapsedFramesOver % gameOverBuffer == 0) {
                enemy.reset();

                elapsedFramesOver = 1;
            }

            ++elapsedFramesOver;
        }

        if(ship.currentState == ship.OVER) {
            if(elapsedFramesOver % gameOverBuffer == 0) {
                ship.reset();

                elapsedFramesOver = 1;
            }

            ++elapsedFramesOver;
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 1;
}