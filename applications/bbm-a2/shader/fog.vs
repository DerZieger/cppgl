#version 150
in vec3 in_pos;
in vec2 in_tc;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec4 pos_wc;
out vec2 tc;

void main() {
    pos_wc = model * vec4(in_pos, 1.0);
    tc = in_tc;
    gl_Position = proj * view * pos_wc;
}
