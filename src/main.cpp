#include "simulation.h"
#include "simulation_model.h"
#include "particle_data.h"
#include "triangle_model.h"
#include "camera.h"
#include "camera_controller.h"
#include "renderer.h"
#include "scene/scene.h"
#include "scene/scene_loader.h"
#include <core/path.h>
#include <core/io/file_access.h>
#include <input/input.h>
#include <rhi/ez_vulkan.h>
#include <rhi/shader_manager.h>
#include <rhi/shader_compiler.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

static void window_size_callback(GLFWwindow* window, int w, int h)
{
}

static void cursor_position_callback(GLFWwindow* window, double pos_x, double pos_y)
{
    Input::get()->set_mouse_position((float)pos_x, (float)pos_y);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Input::get()->set_mouse_button(button, action);
}

static void mouse_scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
{
    Input::get()->set_mouse_scroll((float)offset_y);
}

int main()
{
    // Path settings
    Path::register_protocol("content", std::string(PROJECT_DIR) + "/content/");
    Path::register_protocol("scene", std::string(PROJECT_DIR) + "/content/scene/");
    Path::register_protocol("shader", std::string(PROJECT_DIR) + "/content/shader/");

    ez_init();
    ShaderManager::get()->setup();
    ShaderCompiler::get()->setup();
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* glfw_window = glfwCreateWindow(1024, 768, "pbd", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(glfw_window, window_size_callback);
    glfwSetCursorPosCallback(glfw_window, cursor_position_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    glfwSetScrollCallback(glfw_window, mouse_scroll_callback);
    EzSwapchain swapchain = VK_NULL_HANDLE;
    ez_create_swapchain(glfwGetWin32Window(glfw_window), swapchain);

    Camera* camera = new Camera();
    camera->set_aspect(1024.0f/768.0f);
    CameraController* camera_controller = new CameraController();
    camera_controller->set_camera(camera);

    Renderer* renderer = new Renderer();
    renderer->set_camera(camera);

    Simulation* simulation = new Simulation();

    // Scene load
    Scene* scene = load_scene(SceneType::CLOTH);

    while (!glfwWindowShouldClose(glfw_window))
    {
        glfwPollEvents();

        EzSwapchainStatus swapchain_status = ez_update_swapchain(swapchain);

        if (swapchain_status == EzSwapchainStatus::NotReady)
            continue;

        if (swapchain_status == EzSwapchainStatus::Resized)
        {
            camera->set_aspect(swapchain->width/swapchain->height);
        }

        ez_acquire_next_image(swapchain);

        simulation->step(scene->get_simulation_model());

        scene->draw(renderer);

        renderer->render(swapchain);

        VkImageMemoryBarrier2 present_barrier[] = { ez_image_barrier(swapchain, EZ_RESOURCE_STATE_PRESENT) };
        ez_pipeline_barrier(0, 0, nullptr, 1, present_barrier);

        ez_present(swapchain);

        ez_submit();

        // Reset input
        Input::get()->reset();
    }

    destroy_scene(scene);

    delete simulation;
    delete renderer;
    delete camera;
    delete camera_controller;

    ez_destroy_swapchain(swapchain);
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
    ShaderManager::get()->cleanup();
    ShaderCompiler::get()->cleanup();
    ez_flush();
    ez_terminate();

    return 0;
}