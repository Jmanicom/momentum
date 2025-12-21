#include "util/config.hpp"
#include "physics/engine.hpp"
#include "util/render.hpp"

Engine sim; // Init the simulation engine

static void MTMEvents(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        sim.MtmAdd(MTMCircle({0.10f, 0.5f}, 10.0f, 0.1f));
    }
}

int main()
{
    // Create GLFW window and corresponding context
    MTMwin(window, winSize);
    glfwSetKeyCallback(window, MTMEvents);
    
    sim.MtmAdd(MTMCircle({0.0f, 0.5f}, 10.0f, 0.1f));
    sim.MtmAdd(MTMCircle({0.0f, 1.0f}, 10.0f, 0.1f));
    sim.MtmAdd(MTMRect({0.5f, 0.5f}, 10.0f, {0.1f, 0.1f}));

    // In units of seconds
    float frameStart = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // Run boilerplate start frame func
        MTMstartframe();
        const float currentTime = glfwGetTime();

        // +++++ Time Tracking for updating sim physics +++++
        // Store the time elapsed since the last frame began
        accumalator += currentTime - frameStart;
        // Record the starting of this frame
        frameStart = currentTime;
        if (accumalator > 0.2f)
            accumalator = 0.2f; // Clamp value to avoid rendering issues
        while (accumalator >= dt) {
            sim.step(dt);
            accumalator -= dt;
        }

        Render(sim.Objects());
        
        // Run boilerplate end frame func
        MTMendframe();
    }

    glfwTerminate();
    return 0;
}