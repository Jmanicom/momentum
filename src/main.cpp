#include "util/config.hpp"

int main()
{
    // Create GLFW window and corresponding context
    MTMwin(window, winSize);


    // Keep track of time for sim
    float previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // MtmStartframe returns dt, use this to update sim
        float dt = MTMstartframe(previousTime);

        // Run boilerplate end frame func
        MTMendframe();
    }

    glfwTerminate();
    return 0;
}