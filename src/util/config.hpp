#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "manimath/matvec.hpp"
#include <vector>

// Define physics constants
const double G = 6.6743e-11; // m^3 kg^-1 s^-2
const float c = 299792458.0; // m/s
const double pi = 3.14159265359;


// Window Settings Config
const char* winName = "Momentum2D";    // Window Name as a pointer to a char array
const Vec2i winSize = {800, 600};      // Window size
const float fps = 60;                 // Window frames per second
const float dt = 1 / fps;              // Time-step
float accumalator = 0;


// ========= OpenGL window based functions ===========
// Create window pointer var
GLFWwindow* window;
inline int MTMwin(GLFWwindow*& window, Vec2i winSize)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(winSize.x, winSize.y, winName, NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    //glViewport(0.0, 0.0, winSize.x, winSize.y);     // Map coordinates to size of window, instead of (-1, 1)

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    return 0;
}

// Boiler plate start frame func for momentum
void MTMstartframe() {
    glClearColor(0.12f, 0.12f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Boiler plate end frame func for momentum
void MTMendframe() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}