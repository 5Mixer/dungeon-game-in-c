#version 110

uniform mat3 mvp;

attribute vec2 pos;
varying vec2 texcoord;
attribute vec2 tex;

void main()
{
    gl_Position = vec4(mvp * vec3(pos.x, pos.y, 0.5), 1.0);
    texcoord = tex;
}

