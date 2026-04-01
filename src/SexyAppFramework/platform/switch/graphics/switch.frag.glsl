#version 310 es
precision mediump float;

layout(location = 0) in vec4 v_color;
layout(location = 1) in vec2 v_uv;

layout(location = 0) out vec4 FRAG_OUT;

uniform sampler2D u_texture;
layout(location = 1) uniform int u_useTexture;
layout(location = 2) uniform vec4 u_uvBounds;

void main() {
    if (u_useTexture == 1)
        FRAG_OUT = texture(u_texture, clamp(v_uv, u_uvBounds.xy, u_uvBounds.zw)) * v_color;
    else
        FRAG_OUT = v_color;
}
