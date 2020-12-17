#version 110

uniform sampler2D texsampler;

varying vec2 texcoord;

void main()
{
    vec4 color = texture2D(texsampler, texcoord);
    gl_FragData[0] = vec4(color.x, color.y, color.z, 1.0);
}

