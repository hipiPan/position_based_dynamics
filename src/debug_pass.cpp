#include "debug_pass.h"
#include "renderer.h"
#include <rhi/shader_manager.h>

#define DEBUG_LINE_STRUCT_SIZE  14
#define DEBUG_POINT_STRUCT_SIZE  7

DebugPass::DebugPass(Renderer* renderer)
{
    _renderer = renderer;
    _line_num = 0;
    _point_num = 0;
    resize_line_buffer(256);
    resize_point_buffer(256);
}

DebugPass::~DebugPass()
{
    if (_line_buffer)
        ez_destroy_buffer(_line_buffer);
    if (_point_buffer)
        ez_destroy_buffer(_point_buffer);
}

void DebugPass::draw_line(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& c)
{
    if ((_line_num + 1) > (_lines.size() / DEBUG_LINE_STRUCT_SIZE))
    {
        resize_line_buffer(_line_num * 2);
    }

    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 0] = p0.x;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 1] = p0.y;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 2] = p0.z;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 3] = c.x;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 4] = c.y;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 5] = c.z;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 6] = c.w;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 7] = p1.x;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 8] = p1.y;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 9] = p1.z;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 10] = c.x;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 11] = c.y;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 12] = c.z;
    _lines[_line_num * DEBUG_LINE_STRUCT_SIZE + 13] = c.w;
    _line_num++;
}

void DebugPass::draw_point(const glm::vec3& p, const glm::vec4& c)
{
    if ((_point_num + 1) > (_points.size() / DEBUG_POINT_STRUCT_SIZE))
    {
        resize_line_buffer(_point_num * 2);
    }

    _points[_point_num * DEBUG_POINT_STRUCT_SIZE + 0] = p.x;
    _points[_point_num * DEBUG_POINT_STRUCT_SIZE + 1] = p.y;
    _points[_point_num * DEBUG_POINT_STRUCT_SIZE + 2] = p.z;
    _points[_point_num * DEBUG_POINT_STRUCT_SIZE + 3] = c.x;
    _points[_point_num * DEBUG_POINT_STRUCT_SIZE + 4] = c.y;
    _points[_point_num * DEBUG_POINT_STRUCT_SIZE + 5] = c.z;
    _points[_point_num * DEBUG_POINT_STRUCT_SIZE + 6] = c.w;
    _point_num++;
}

void DebugPass::render()
{
    if (_line_num == 0 && _point_num == 0)
        return;

    VkBufferMemoryBarrier2 barrier;
    if (_line_num > 0)
    {
        barrier = ez_buffer_barrier(_line_buffer, EZ_RESOURCE_STATE_COPY_DEST);
        ez_pipeline_barrier(0, 1, &barrier, 0, nullptr);
        ez_update_buffer(_line_buffer, _line_num * DEBUG_LINE_STRUCT_SIZE * sizeof(float), 0, _lines.data());
        barrier = ez_buffer_barrier(_line_buffer, EZ_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        ez_pipeline_barrier(0, 1, &barrier, 0, nullptr);
    }
    if (_point_num > 0)
    {
        barrier = ez_buffer_barrier(_point_buffer, EZ_RESOURCE_STATE_COPY_DEST);
        ez_pipeline_barrier(0, 1, &barrier, 0, nullptr);
        ez_update_buffer(_point_buffer, _point_num * DEBUG_POINT_STRUCT_SIZE * sizeof(float), 0, _points.data());
        barrier = ez_buffer_barrier(_point_buffer, EZ_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        ez_pipeline_barrier(0, 1, &barrier, 0, nullptr);
    }

    ez_reset_pipeline_state();
    VkImageMemoryBarrier2 rt_barriers[1];
    rt_barriers[0] = ez_image_barrier(_renderer->_color_rt, EZ_RESOURCE_STATE_RENDERTARGET);
    ez_pipeline_barrier(0, 0, nullptr, 1, rt_barriers);

    EzRenderingAttachmentInfo color_info{};
    color_info.texture = _renderer->_color_rt;
    color_info.clear_value.color = {0.0f, 0.0f, 0.0f, 1.0f};
    EzRenderingInfo rendering_info{};
    rendering_info.width = _renderer->_width;
    rendering_info.height = _renderer->_height;
    rendering_info.colors.push_back(color_info);
    ez_begin_rendering(rendering_info);
    ez_set_viewport(0, 0, (float)_renderer->_width, (float)_renderer->_height);
    ez_set_scissor(0, 0, (int32_t)_renderer->_width, (int32_t)_renderer->_height);

    ez_set_vertex_shader(ShaderManager::get()->get_shader("shader://simple.vert"));
    ez_set_fragment_shader(ShaderManager::get()->get_shader("shader://simple.frag"));

    ez_set_vertex_binding(0, 28);
    ez_set_vertex_attrib(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0);
    ez_set_vertex_attrib(0, 1, VK_FORMAT_R32G32B32A32_SFLOAT, 12);

    ez_bind_buffer(0, _renderer->_view_buffer, sizeof(ViewBufferType));

    // Lines
    if (_line_num > 0)
    {
        float point_size = 2.0f;
        ez_push_constants(&point_size, sizeof(float), 0);
        ez_set_primitive_topology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
        ez_bind_vertex_buffer(_line_buffer);
        ez_draw(_line_num * 2, 0);
    }

    // Points
    if (_point_num > 0)
    {
        float point_size = 8.0f;
        ez_push_constants(&point_size, sizeof(float), 0);
        ez_set_primitive_topology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
        ez_bind_vertex_buffer(_point_buffer);
        ez_draw(_point_num, 0);
    }

    ez_end_rendering();

    _line_num = 0;
    _point_num = 0;
}

void DebugPass::resize_line_buffer(uint32_t size)
{
    _lines.resize(DEBUG_LINE_STRUCT_SIZE * size);

    if (_line_buffer)
        ez_destroy_buffer(_line_buffer);
    EzBufferDesc buffer_desc = {};
    buffer_desc.size = DEBUG_LINE_STRUCT_SIZE * size * sizeof(float);
    buffer_desc.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffer_desc.memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    ez_create_buffer(buffer_desc, _line_buffer);
}

void DebugPass::resize_point_buffer(uint32_t size)
{
    _points.resize(DEBUG_POINT_STRUCT_SIZE * size);

    if (_point_buffer)
        ez_destroy_buffer(_point_buffer);
    EzBufferDesc buffer_desc = {};
    buffer_desc.size = DEBUG_POINT_STRUCT_SIZE * size * sizeof(float);
    buffer_desc.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffer_desc.memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    ez_create_buffer(buffer_desc, _point_buffer);
}