#version 330

uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

uniform vec4 u_color;

uniform sampler2D u_texture_2;
uniform vec2 u_texture_2_size;

uniform float u_normal_scaling;
uniform float u_height_scaling;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;
in vec2 v_uv;

out vec4 out_color;

float h(vec2 uv) {
  // You may want to use this helper function...
  return texture(u_texture_2, uv).r;
}

void main() {
  // YOUR CODE HERE
  vec3 normal = v_normal.xyz;
  vec3 tangent = v_tangent.xyz;
  vec3 bitangent = cross(normal, tangent);
  mat3 tbn = mat3(tangent, bitangent, normal);


  float u = v_uv.x;
  float v = v_uv.y;
  float w = u_texture_2_size.x;
  float ht = u_texture_2_size.y;
  float kh = u_height_scaling;
  float kn = u_normal_scaling;

  //dU = (h(u + 1 / w, v) - h(u, v)) * k_h * k_n
  float hmain = h(v_uv);//texture(u_texture_2, vec2(u, v))[0];
  float hU = h(vec2(u + 1.0 / w, v));//texture(u_texture_2, vec2(u + 1.0 / w, v))[0];
  float dU = (hU - hmain) * kh * kn;

  //dV = (h(u, v + 1 / h) - h(u, v)) * k_h * k_n
  float hV = h(vec2(u, v + 1.0 / ht));//texture(u_texture_2, vec2(u, v + 1.0 / h))[0];
  float dV = (hV - hmain) * kh * kn;

  vec3 no = vec3(-dU, -dV, 1.0);
  vec3 nd = tbn * no;

    // YOUR CODE HERE
    float k_a = 0.1;
    float k_s = 1.0;
    int p = 100;
    vec3 k_d = u_color.rgb;
    vec3 i_a = vec3(1, 1, 1);

    vec3 alt_v_normal = normalize(nd.xyz);
    vec3 alt_v_pos = normalize(u_cam_pos - v_position.xyz);
    vec3 alt_light_pos = normalize(u_light_pos);
    vec3 alt_light_int = normalize(u_light_intensity);

    vec3 r = normalize(alt_v_pos - alt_light_pos);
    vec3 illumination = normalize(alt_light_int) / pow(length(r), 2);
    float max_zero_nl = max(0.0, dot(alt_v_normal, alt_light_int));
    vec3 hnorm = normalize(alt_light_int + alt_v_pos);
    float max_zero_nh_p = pow(max(0.0, dot(alt_v_normal, hnorm)), p);

    vec3 diffuse = k_d * illumination * max_zero_nl;
    vec3 ambient = k_a * i_a;
    vec3 specular = k_s * illumination * max_zero_nh_p;

    out_color = vec4((diffuse + ambient + specular) * u_color.rgb, 1);
    // out_color.a = u_color.a;
    out_color.a = 1.0;
}

