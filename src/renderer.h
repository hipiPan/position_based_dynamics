#pragma once
#include <rhi/ez_vulkan.h>
#include <glm/glm.hpp>

class Camera;

struct ViewBufferType
{
    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;
};

class Renderer
{
public:
    Renderer();

    ~Renderer();

    void render(EzSwapchain swapchain);

    void set_camera(Camera* camera);

    void draw_line(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& c);
    void draw_point(const glm::vec3& p, const glm::vec4& c);

private:
    void update_rendertarget();

    void update_scene_buffer();

    void update_view_buffer();

    uint32_t _width = 0;
    uint32_t _height = 0;
    uint64_t _frame_number = 0;
    Camera* _camera;
    EzBuffer _view_buffer = VK_NULL_HANDLE;
    EzTexture _color_rt = VK_NULL_HANDLE;
    EzTexture _depth_rt = VK_NULL_HANDLE;
    friend class DebugPass;
    DebugPass* _debug_pass;
};