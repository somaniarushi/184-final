#version 330

uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

uniform vec4 u_color;

uniform sampler2D u_texture_3;
uniform vec2 u_texture_3_size;

uniform float u_normal_scaling;
uniform float u_height_scaling;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;
in vec2 v_uv;

out vec4 out_color;

float h(vec2 uv) {
  // You may want to use this helper function...
  return texture(u_texture_3, uv).r;
}

void main() {
  // YOUR CODE HERE
  vec3 b = normalize(cross(vec3(v_normal), vec3(v_tangent)));
  mat3 m = mat3(
    v_tangent.x, v_tangent.y, v_tangent.z,
    b.x, b.y, b.z,
    v_normal.x, v_normal.y, v_normal.z
  );
  float dU = (h(vec2((v_uv.x+(1.0/u_texture_3_size.x)), v_uv.y))-h(v_uv))*u_height_scaling*u_normal_scaling;
  float dV = (h(vec2(v_uv.x, (v_uv.y+(1.0/u_texture_3_size.y))))-h(v_uv))*u_height_scaling*u_normal_scaling;
  vec3 n = m*vec3(-1*dU, -1*dV, 1.0);

  float r = distance(v_position.xyz, u_light_pos);
  vec3 light = normalize(u_light_pos.xyz - v_position.xyz);
  out_color.xyz = (u_light_intensity/dot(r,r))*max(0,dot(n.xyz,light));
  out_color.a = 1;
}

