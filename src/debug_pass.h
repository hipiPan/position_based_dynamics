#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <rhi/ez_vulkan.h>

class Renderer;

class DebugPass
{
public:
    DebugPass(Renderer* renderer);

    ~DebugPass();

    void render();

    void draw_line(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& c);
    void draw_point(const glm::vec3& p, const glm::vec4& c);

private:
    void resize_line_buffer(uint32_t size);
    void resize_point_buffer(uint32_t size);

    Renderer* _renderer;
    uint32_t _line_num;
    uint32_t _point_num;
    std::vector<float> _lines;
    std::vector<float> _points;
    EzBuffer _line_buffer = VK_NULL_HANDLE;
    EzBuffer _point_buffer = VK_NULL_HANDLE;
};