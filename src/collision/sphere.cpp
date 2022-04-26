#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace nanogui;
using namespace CGL;

void Sphere::collide(PointMass &pm) {
  // TODO (Part 3): Handle collisions with spheres
  auto dist_from_origin = (pm.position - origin).norm();

  // Point is within sphere
  if (dist_from_origin < radius) {
    auto origin_dir = (pm.position - origin);
    origin_dir.normalize();
    auto tangent_point = origin + origin_dir * radius;
    auto correction_vector = tangent_point - pm.last_position;
    pm.position = pm.last_position + correction_vector * (1 - friction);
  }
}

void Sphere::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  m_sphere_mesh.draw_sphere(shader, origin, radius * 0.92);
}
