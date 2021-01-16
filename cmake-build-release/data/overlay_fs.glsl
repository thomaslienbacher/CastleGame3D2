#version 420

out vec4 fragcolor;

in vec4 s_viewspace;
in vec2 s_texcoord;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D overlay;

uniform vec2 u_offset;

void main(){
    fragcolor = texture(tex, s_texcoord);
    float overlay = texture(overlay, s_texcoord + u_offset).r;
    fragcolor.rgb *= overlay;

    if (fragcolor.a < 0.5){
        discard;
    }

    float distance = length(s_viewspace);
    float x = distance;
    float attenuation = -tanh(x * 0.23 - 1.8) * 0.5 + 0.49;
    fragcolor.xyz *= clamp(attenuation + (1.0 - overlay) * 1.3, 0, 1);
}
