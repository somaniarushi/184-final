#include "iostream"
#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../clothSimulator.h"
#include "plane.h"

using namespace std;
using namespace CGL;

#define SURFACE_OFFSET 0.0001

void Plane::collide(PointMass &pm) {
  // TODO (Part 3): Handle collisions with planes.

  // You know the point cross the plane if the direction of the normals between last position are current
  // positions are opposites.
  if (dot(normal, pm.last_position - point) * dot(normal, pm.position - point) <= 0.0) { //todo: changed to <=
      // The point is on the plane

      auto dist_to_plane = dot(normal, pm.position - point);
      auto tangent_point = pm.position - normal * dist_to_plane;
      auto distance_to_last_pos = dot(normal, pm.last_position - point);
      auto dir = distance_to_last_pos > 0 ? -1 : 1;
      auto correction_vector = tangent_point - pm.last_position - dir * SURFACE_OFFSET * normal;
      pm.position = pm.last_position + correction_vector * (1 - friction);
    }
  }

void Plane::render(GLShader &shader) {
  nanogui::Color color(0.7f, 0.7f, 0.7f, 1.0f);

  Vector3f sPoint(point.x, point.y, point.z);
  Vector3f sNormal(normal.x, normal.y, normal.z);
  Vector3f sParallel(normal.y - normal.z, normal.z - normal.x,
                     normal.x - normal.y);
  sParallel.normalize();
  Vector3f sCross = sNormal.cross(sParallel);

  MatrixXf positions(3, 4);
  MatrixXf normals(3, 4);

  positions.col(0) << sPoint + 2 * (sCross + sParallel);
  positions.col(1) << sPoint + 2 * (sCross - sParallel);
  positions.col(2) << sPoint + 2 * (-sCross + sParallel);
  positions.col(3) << sPoint + 2 * (-sCross - sParallel);

  normals.col(0) << sNormal;
  normals.col(1) << sNormal;
  normals.col(2) << sNormal;
  normals.col(3) << sNormal;

  if (shader.uniform("u_color", false) != -1) {
    shader.setUniform("u_color", color);
  }
  shader.uploadAttrib("in_position", positions);
  if (shader.attrib("in_normal", false) != -1) {
    shader.uploadAttrib("in_normal", normals);
  }
  shader.drawArray(GL_TRIANGLE_STRIP, 0, 4);
}


