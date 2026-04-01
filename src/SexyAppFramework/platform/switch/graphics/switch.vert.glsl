#version 310 es
precision mediump float;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_uv;

layout(location = 0) out vec4 v_color;
layout(location = 1) out vec2 v_uv;

layout(location = 0) uniform mat4 u_viewProj;

void main() {
    v_color = a_color;
    v_uv = a_uv;
    gl_Position = u_viewProj * vec4(a_position, 1.0);
}
