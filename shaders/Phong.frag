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
  float k_a = 0.1;
  float k_s = 1.0;
  int p = 50;
  vec3 k_d = u_color.rgb;
  vec3 i_a = vec3(1, 1, 1);

  vec3 alt_v_normal = normalize(v_normal.xyz);
    vec3 alt_v_pos = normalize(u_cam_pos.xyz - v_position.xyz);
    vec3 alt_light_pos = normalize(u_light_pos);
    vec3 alt_light_int = normalize(u_light_intensity);

    vec3 r = normalize(alt_v_pos - alt_light_pos);
    vec3 illumination = normalize(alt_light_int / pow(length(r), 2));
    float max_zero_nl = max(0.0, dot(alt_v_normal, alt_light_int));
  vec3 h = normalize(alt_light_int + alt_v_pos);
  float max_zero_nh_p = pow(max(0.0, dot(alt_v_normal, h)), p);

  vec3 diffuse = k_d * illumination * max_zero_nl;
  vec3 ambient = k_a * i_a;
  vec3 specular = k_s * illumination * max_zero_nh_p;

  out_color = vec4((diffuse + ambient + specular) * u_color.rgb, 1);
  out_color.a = u_color.a;
}

