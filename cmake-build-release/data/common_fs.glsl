#version 460

out vec4 fragcolor;

in vec4 s_viewspace;
in vec2 s_texcoord;

uniform sampler2D tex;

void main(){
    fragcolor = texture(tex, s_texcoord);
    if (fragcolor.a < 0.5){
        discard;
    }

    float distance = length(s_viewspace);
    float x = distance;
    float attenuation = -tanh(x * 0.23 - 1.8) * 0.5 + 0.49;
    fragcolor.xyz *= clamp(attenuation, 0.05, 1.0);
}
