#version 460

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_texcoord;

uniform mat4 u_proj;
uniform mat4 u_model;

out vec2 s_texcoord;

void main(){
    gl_Position = u_proj *  u_model * vec4(i_position, 0.0, 1.0);
    s_texcoord = i_texcoord;
}
