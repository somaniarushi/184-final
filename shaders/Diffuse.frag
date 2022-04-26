#version 330

// The camera's position in world-space
uniform vec3 u_cam_pos;

// Color
uniform vec4 u_color;

// Properties of the single point light
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

// We also get the uniform texture we want to use.
uniform sampler2D u_texture_1;

// These are the inputs which are the outputs of the vertex shader.
in vec4 v_position;
in vec4 v_normal;

// This is where the final pixel color is output.
// Here, we are only interested in the first 3 dimensions (xyz).
// The 4th entry in this vector is for "alpha blending" which we
// do not require you to know about. For now, just set the alpha
// to 1.
out vec4 out_color;

void main() {
  // YOUR CODE HERE
  vec3 alt_v_normal = normalize(v_normal.xyz);
  vec3 alt_v_pos = normalize(v_position.xyz);
  vec3 alt_light_pos = normalize(u_light_pos);
  vec3 alt_light_int = normalize(u_light_intensity);

  vec3 k_d = vec3(1, 1, 1);
  vec3 r = normalize(alt_v_pos - alt_light_pos);
  vec3 illumination = normalize(alt_light_int / pow(length(r), 2));
  float max_zero_nl = max(0.0, dot(alt_v_normal, alt_light_int));

  out_color = vec4(k_d * illumination * max_zero_nl * u_color.rgb, 1);
  out_color.a = 1;
}
