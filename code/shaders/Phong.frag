#version 330

uniform vec4 u_color;
uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_uv;

out vec4 out_color;

void main() {
  // YOUR CODE HERE
  //at one point in debugging the cloth was completely white. adjusted constants
  //was getting error that turns out missing ; 
  // (Placeholder code. You will want to replace it.)
    float r = distance(v_position.xyz, u_light_pos);
    vec3 view = normalize(u_light_pos.xyz - u_cam_pos);
    vec3 light = normalize(u_light_pos.xyz - v_position.xyz);
    vec3 h = normalize(view + light);
    vec3 term1 = (u_light_intensity/dot(r,r))*.5*max(0,dot(v_normal.xyz,light));
    vec3 term2 = (u_light_intensity/dot(r,r))*pow(max(0,dot(v_normal.xyz,h)),3);
    out_color.xyz = .4 + term1 + term2;
    out_color.a = 1;
}

