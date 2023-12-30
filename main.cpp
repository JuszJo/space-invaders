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
#include "classes/bullet.h"

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

    double FPS = 60.0;

    double FPSInterval = 1000 / FPS;

    double lastFrameTime = glfwGetTime();

    double elapsed = 0.0;

    Shader shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    Ship ship(&shader);

    int textWidth, textHeight, numberOfChannels;

    unsigned char* imageData = stbi_load("wall.jpg", &textWidth, &textHeight, &numberOfChannels, 0);

    glGenTextures(1, &ship.TBO);
    glBindTexture(GL_TEXTURE_2D, ship.TBO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textWidth, textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);

    shader.use();

    glm::mat4 projection = glm::mat4(1.0f);

    projection = glm::ortho(0.0f, 600.0f, 0.0f, 600.0f, -1.0f, 1.0f);

    int projectionLocation = glGetUniformLocation(shader.shaderProgram, "projection");

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.7f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ship.processInput(window);

        double currentFrameTime = glfwGetTime();

        double delta = currentFrameTime - lastFrameTime;

        elapsed += delta * 1000;

        lastFrameTime = currentFrameTime;

        if(elapsed >= FPSInterval) {
            elapsed = 0;

            ship.update();
        }

        ship.render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 1;
}