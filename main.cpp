#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "GPXReader.h"

int main(int argc, char *argv[])
{

    // read in our gpx file -----
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename.gpx>\n";
        return 1;
    }

    auto trekPoints = readGPXFile(argv[1]);


    // -------------------------
    // Initialize GLFW
    // -------------------------

    double x[] = {
        0.0,
        1.0,
        2.0,
        3.0,
        4.0
    };

    double y[] = {
        0.0,
        2.0,
        1.0,
        4.0,
        3.0
    };

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    // Tell GLFW which version of OpenGL we want.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // -------------------------
    // Create window
    // -------------------------

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "GPX Reader",
        nullptr,
        nullptr
    );

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // Enable VSync
    glfwSwapInterval(1);

    // -------------------------
    // Initialize Dear ImGui
    // -------------------------

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // GLFW backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    // OpenGL backend
    ImGui_ImplOpenGL3_Init("#version 330");

    // -------------------------
    // Initialize ImPlot
    // -------------------------

    ImPlot::CreateContext();

    // -------------------------
    // Main loop
    // -------------------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        // -------------------------
        // Our GUI
        // -------------------------

        ImGui::Begin("GPX Reader");

        ImGui::Text("Hello from Dear ImGui!");

        if (ImPlot::BeginPlot("Test Plot"))
        {
            ImPlot::PlotLine(
                "Data",
                x,
                y,
                5
            );

            ImPlot::EndPlot();
        }

        ImGui::End();

        // -------------------------
        // Rendering
        // -------------------------

        ImGui::Render();

        int displayWidth;
        int displayHeight;

        glfwGetFramebufferSize(
            window,
            &displayWidth,
            &displayHeight
        );

        glViewport(
            0,
            0,
            displayWidth,
            displayHeight
        );

        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }

    // -------------------------
    // Cleanup
    // -------------------------

    ImPlot::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}