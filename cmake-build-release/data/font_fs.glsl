#version 420

out vec4 fragcolor;

in vec2 s_texcoord;

uniform sampler2D tex;

void main(){
    fragcolor = texture(tex, s_texcoord);
    fragcolor.a = fragcolor.r;
    fragcolor.rgb = vec3(1);
}
