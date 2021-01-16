#version 420

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec2 i_texcoord;
layout(location = 2) in vec3 i_normal;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

out vec4 s_viewspace;
out vec2 s_texcoord;

void main(){
    vec4 viewspace = u_view * u_model * vec4(i_position, 1.0);
    gl_Position = u_proj * viewspace;

    s_viewspace = viewspace;
    s_texcoord = i_texcoord;
}
