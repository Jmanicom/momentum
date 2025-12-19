#include "util/config.hpp"
#include "util/engine.hpp"
#include "util/render.hpp"

int main()
{
    // Create GLFW window and corresponding context
    MTMwin(window, winSize);

    Engine sim;

    sim.MtmAdd(MTMCircle({0.0f, 0.5f}, 10.0f, 0.05f));
    sim.MtmAdd(MTMRect({0.5f, 0.5f}, 10.0f, {0.05f, 0.05f}));

    // In units of seconds
    float frameStart = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        // Run boilerplate start frame func
        MTMstartframe();
        const float currentTime = glfwGetTime();

        // Store the time elapsed since the last frame began
        accumalator += currentTime - frameStart;
        // Record the starting of this frame
        frameStart = currentTime;
        if (accumalator > 0.2f)
            accumalator = 0.2f; // Clamp value to avoid rendering issues
        while (accumalator > dt)
            accumalator -= dt;

        sim.step(dt);

        Render(sim.Objects());
        
        // Run boilerplate end frame func
        MTMendframe();
    }

    glfwTerminate();
    return 0;
}