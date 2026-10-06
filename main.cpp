#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <array>
#include "GPXReader.h"

namespace imgui_panel
{
    bool open_gpx = false;
    bool export_to_csv = false;
    std::string filename = "";
    float distance = 0.f;

};

namespace implot_data
{
    std::vector<double> longitudes;
    std::vector<double> latitudes;
    std::vector<double> elevations;
    std::vector<double> y;
};

int main(int argc, char *argv[])
{

    // read in our gpx file -----
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename.gpx>\n";
        return 1;
    }

    auto trekPoints = readGPXFile(argv[1]);

    // we have 2 key graphs
    // 1.) an elevation plot
    // 2.) our route plot (this i'd like to add colour too to emphasize the plot size)
    
    double i {};

    for(auto const point : trekPoints)
    {
        implot_data::longitudes.push_back(point.longitude);
        implot_data::latitudes.push_back(point.latitude);
        implot_data::elevations.push_back(point.elevation);
        implot_data::y.push_back(i++);
    }
    
    

    // initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    // Tell GLFW which version of OpenGL we want.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window ------

    GLFWwindow* window = glfwCreateWindow(
        1280,
        1280,
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

    //ImGui and ImPlot set up -----

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // GLFW backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    // OpenGL backend
    ImGui_ImplOpenGL3_Init("#version 330");


    ImPlot::CreateContext();

    // Main loop -------------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        // GUI and plots ------------------

        ImGui::Begin("GPX Reader");

        ImGui::Text("Hello from Dear ImGui!");

        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Open GPX file"))
        {
            static std::array<char, 64> buffer;
            ImGui::InputText("(.gpx) file", buffer.data(), buffer.size());

            imgui_panel::open_gpx = ImGui::Button("Load");
            
            if (imgui_panel::open_gpx){

                imgui_panel::filename = buffer.data();
                trekPoints = readGPXFile((imgui_panel::filename).c_str());
            } 

        }

        ImGui::Spacing();
        if(ImGui::CollapsingHeader("Export to CSV file"))
        {
            static std::array<char, 64> buff;
            ImGui::InputText("(.cvs) file name", buff.data(), buff.size());

            imgui_panel::export_to_csv = ImGui::Button("Export");

            if(imgui_panel::export_to_csv)
            {
                std::string filename = buff.data();
                filename += ".csv";
                exportCSV(filename.c_str(), trekPoints);
            }
        }

        // our spec object to change plot style
        ImPlotSpec routeSpec;
        routeSpec.LineColor = {0.0f, 1.0f, 0.5f, 1.0f};

        ImPlotSpec elevationSpec;
        elevationSpec.LineColor = {1.0f, 0.07f, 0.576f, 1.f};
        // ----------------------------------------

        if (ImPlot::BeginPlot("Route map"))
        {
            ImPlot::PlotLine(
                "Coordinates",
                implot_data::longitudes.data(),
                implot_data::latitudes.data(),
                implot_data::longitudes.size(),
                routeSpec
            );

            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Elevation Plot"))
        {
            ImPlot::PlotLine(
                "elevation (m)",
                implot_data::y.data(),
                implot_data::elevations.data(),
                implot_data::elevations.size(),
                elevationSpec
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