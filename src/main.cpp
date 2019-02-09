#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "common/ShaderManager.hpp"
#include "common/GraphicsProgram.h"
#include "LaplaceAnimator.h"

void
die(const char* msg, int exitCode) {
    std::cerr << msg << std::endl;

    exit(exitCode);
}

void setWindowSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0,0,width, height);
}

// GLOBAL VARIABLES
GLint windowWidth = 800;
GLint windowHeight = 600;

int main(int argc, const char * argv[]) {
    if(!glfwInit()) {
        die("Could not initialize glfw! Aborting...", -1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Window", NULL, NULL);

    if(window == NULL) {
        die("Could not create window! Aborting...", -1);
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, setWindowSizeCallback);

    glewExperimental = true;

    if(glewInit() != GLEW_OK) {
        die("Could not initialize glew! Aborting...", -1);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glViewport(0,0,windowWidth, windowHeight);

    GraphicsProgram *program = (GraphicsProgram*) new LaplaceAnimator(window);
    program->Initialise(windowWidth, windowHeight);
    do {
        program->Update();
        program->Draw();
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

    delete program;
    glfwTerminate();
    return 0;
}
