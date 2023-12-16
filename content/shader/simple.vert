#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 0) out vec4 out_color;

layout(std140, set = 0, binding = 0) uniform ViewBuffer
{
    mat4 view_matrix;
    mat4 proj_matrix;
} view_buffer;

layout(push_constant) uniform ConstantBlock
{
    float point_size;
} constant;

void main()
{
    gl_PointSize = constant.point_size;
    gl_Position = view_buffer.proj_matrix * view_buffer.view_matrix * vec4(in_position, 1.0);
    out_color = in_color;
}