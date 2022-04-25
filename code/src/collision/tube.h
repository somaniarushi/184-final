#ifndef COLLISIONOBJECT_TUBE_H
#define COLLISIONOBJECT_TUBE_H

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "collisionObject.h"

using namespace CGL;
using namespace std;

struct Tube : public CollisionObject {
public:
  Tube(const Vector3D &origin, double radius, double friction, double height)
      : origin(origin), radius(radius), radius2(radius * radius),
        friction(friction)) {}

  void render(GLShader &shader);
  void collide(PointMass &pm);

private:
  Vector3D origin;
  double radius;
  double radius2;
  double height; 
  double friction;
  //find if theres one for cylinder
  Misc::SphereMesh m_sphere_mesh;
};

#endif 
