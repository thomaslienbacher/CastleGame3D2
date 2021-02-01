#version 420

out vec4 fragcolor;

in vec2 s_texcoord;

uniform sampler2D tex;
uniform float u_alpha = 1.f;

void main(){
    fragcolor = texture(tex, s_texcoord);
    fragcolor.a = fragcolor.r * u_alpha;
    fragcolor.rgb = vec3(1);
}
