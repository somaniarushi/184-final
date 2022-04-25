#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace nanogui;
using namespace CGL;

void Sphere::collide(PointMass &pm) {
    Vector3D forTan = pm.position - origin;
    if (forTan.norm() < radius)
    {
        Vector3D tangent_point = origin + forTan.unit() * radius; //if tangential it needs to be dist radius away
        Vector3D correction_vector = tangent_point - pm.last_position;
        pm.position = pm.last_position + correction_vector * (1 - friction);
    }

}

void Sphere::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  m_sphere_mesh.draw_sphere(shader, origin, radius * 0.92);
}
